///
/// member functions of the loader class which loads values and returns them to
/// a reference
/// @file loader_qmp2_from_qchem.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 11-01-2022
//

#include "loader_qmp2_from_qchem.h"

// Q-Chem libraries
#include <qchem.h>
#include <libqints/basis/basis_1e1c_cgto.h>
#include <libqints/qchem/aobasis.h>
#include <libqints/types.h>
#include <libqints/ftype/ftype_cgto.h>

#include <libdftn/xcclass.h>
#include <libgrid/libgrid/eval_cgto.h>
#include <libqints/arrays/array_view.h>
#include <libqints/exec/dev_omp.h>

#include <iostream>

using namespace std;

namespace libqqc {

    ///
    /// @brief fills an array with 1.0
    ///
    /// @details This takes an array pointer an a length and fill each cell
    /// with a dummy value. This function is used for testing only!
    ///
    /// @param[in] array pointer to array
    /// @param[in] size size of array
    void fill_array (size_t size, double* array);

    void fill_array (size_t size, double* array) {

       for (size_t i = 0; i < size; i++) {
           array[i] = 1.0;
       } 

    }

    void Loader_qmp2_from_qchem :: load_nocc (size_t &nocc){

        //nocc = 1; //
        nocc = rem_read(REM_NALPHA);

    } //Loader_qmp2_from_qchem::load_nocc

    void Loader_qmp2_from_qchem :: load_nvirt(size_t &nvirt) {

        size_t nmo = rem_read(REM_NLINOR);
        size_t occ = 0;
        load_nocc(occ);
        nvirt = nmo - occ;
    
    }

    void Loader_qmp2_from_qchem :: load_nao(size_t &nao) {
        libqints::basis_1e1c_cgto<double> basis = libqints::qchem::aobasis.b1;

        nao = basis.get_nbsf();
    
    }

    void Loader_qmp2_from_qchem :: load_prnt_lvl(int &prnt_lvl) {

        int prnt_lvl_qchem = rem_read(REM_QQC_PRINT_LEVEL);
        prnt_lvl = (prnt_lvl_qchem > 0) ? prnt_lvl_qchem : 0;
    
    }

    void Loader_qmp2_from_qchem :: load_1Dgrid(Grid &grid) {

        size_t npts = 2;
        size_t ndim = 1;
        double pts[npts * ndim];
        double wts[npts];

        fill_array (npts * ndim, pts);
        fill_array (npts, wts);

        grid.set_grid(npts, ndim, pts, wts);
    
    }

    void Loader_qmp2_from_qchem :: load_3Dgrid(Grid &grid) {
#if 1
        XCAtoms xcatom; 
        int jobID0 = 0; 
        XCFunctional Func(0,0); 
        XCJobPara xcpara(Func, xcatom, jobID0);
        double thresh = xcpara.thresh; 
        int nAtoms = xcatom.getNAtoms();

        int grdTyp = rem_read(REM_IGRDTY);

        int IBCode = rem_read(REM_IBASIS);
        SET_QC_FCOMMON(&IBCode); 

        XCBasisSet basDen(IBCode, nAtoms, thresh); 
        xcatom.setSize(basDen); 

        MoleGrid mgrid(xcatom, grdTyp, xcpara.nDrvNuc, thresh);

        int *iPtBat= new int[mgrid.getNBatch()+1]; 
        int npts=mgrid.getNPts();
        int nbatch=mgrid.getNBatch();

        double pts[npts * 3];
        double wts[npts];

        mgrid.getMolGrid(pts, wts, iPtBat, npts, nbatch); 

        grid.set_grid(npts, 3, pts, wts);

        delete[] iPtBat; 
#endif
    }

    void Loader_qmp2_from_qchem :: load_mat_fock(double* mat_fock) {
        size_t nbsf = 0;
        load_nao(nbsf);

        FileMan(FM_READ, FILE_FOCK_MATRIX, FM_DP, nbsf * nbsf, 0, FM_BEG, mat_fock); 

    }

    void Loader_qmp2_from_qchem :: load_mat_cgto(double* mat_cgto) {
#if 1
        Grid grid;
        load_3Dgrid(grid);

        size_t nao = 0;
        load_nao(nao);

        size_t npts = grid.get_mnpts();

        libqints::array_view<double> av_pts(grid.get_mpts(), npts * 3);
        libqints::basis_1e1c_cgto<double> basis = libqints::qchem::aobasis.b1;
        libqints::basis_1e1c_delta bg(npts, av_pts);
        cout << "Step 1" << endl;
        libqints::dev_omp dev;
        cout << "Step 2" << endl;
        dev.init(32 * 1024UL * 1024);
        cout << "Step 3" << endl;
        libqints::array_view<double> av_cgto(mat_cgto, npts * nao);
        int error = libgrid::eval_cgto(basis, bg, dev, av_cgto);
        cout << "Step 4" << endl;
#endif
    }

    void Loader_qmp2_from_qchem :: load_cube_coul(double* cube_coul) {
#if 1
        Grid grid;
        load_3Dgrid(grid);

        size_t nbsf = 0;
        load_nao(nbsf);

        size_t npts = grid.get_mnpts();
        std::vector< libqints::ftype_multipole > v_pts;

        for (int i = 0; i < npts; i++){
            libqints::ftype_multipole f_mult;
            f_mult.x = grid.get_mpts()[i + 0];
            f_mult.y = grid.get_mpts()[i + 1];
            f_mult.z = grid.get_mpts()[i + 2];
            f_mult.k = 0;
            v_pts.push_back(f_mult);
        }//for i
        libqints::basis_1e1c_multipole bm (v_pts);

        double mom[npts];
        for (int i = 0; i < npts; i++) mom[i] = 1;
        libqints::array_view<double> av_mom((double*) &mom[0], npts);
        libqints::dev_omp dev;

        arma::cube coulomb(nbsf, nbsf, npts);
        libqints::array_view<double> av_vmul(cube_coul, npts*nbsf*nbsf);
        dev.init(256UL * 1024 * 1024);


#endif
    }

} //namespace libqqc

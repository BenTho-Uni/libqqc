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
        occ = rem_read(REM_NALPHA);

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

//        size_t npts = 2;
//        size_t ndim = 3;
//        double pts[npts * ndim];
//        double wts[npts];

//        fill_array (npts * ndim, pts);
//        fill_array (npts, wts);

//        grid.set_grid(npts, ndim, pts, wts);
        
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

        double pts[npts];
        double wts[npts];

        mgrid.getMolGrid(pts, wts, iPtBat, npts, nbatch); 

        grid.set_grid(npts, 3, pts, wts);

        delete[] iPtBat; 

    }

    void Loader_qmp2_from_qchem :: load_mat_fock(double* mat_fock) {
        size_t nsbf = 0;
        load_nao(nbsf);

        FileMan(FM_READ, FILE_FOCK_MATRIX, FM_DP, nbsf * nbsf, 0, FM_BEG, mat_fock); 

    }

    void Loader_qmp2_from_qchem :: load_mat_cgto(double* mat_cgto) {

        size_t npts = 2;
        size_t nmo = 3;

        fill_array (npts * nmo, mat_cgto); 
    
    }

    void Loader_qmp2_from_qchem :: load_cube_coul(double* cube_coul) {

        size_t npts = 2;
        size_t nocc = 1;
        size_t nvirt = 2;

        fill_array (npts * nocc * nvirt, cube_coul);
    
    }

} //namespace libqqc

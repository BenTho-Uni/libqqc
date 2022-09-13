///
/// member functions of the loader class which loads values and returns them to
/// a reference
/// @file loader_qmp2_from_qchem.cpp
/// @author Benjamin Thomitzni and Isabel Vinterbladh
/// @version 0.2 12-09-2022
///

#include "loader_qmp2_from_qchem.h"
#include "../qchem/integrals/qqc_vmul.h"
#include "../utils/ttimer.h"

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
#include <typeinfo>
#include <numeric>
#include <vector>

using namespace std;

namespace libqqc {

    void Loader_qmp2_from_qchem :: load_nocc (size_t &nocc){

        //ream_read is a Qchem function to read in the input
        //parameters from the file
        nocc = rem_read(REM_NALPHA);

    }

    void Loader_qmp2_from_qchem :: load_nvirt(size_t &nvirt) {

        //ream_read is a Qchem function to read in the input
        //parameters from the file
        size_t nmo = rem_read(REM_NLINOR);
        size_t occ = 0;
        load_nocc(occ);
        nvirt = nmo - occ;
    
    }

    void Loader_qmp2_from_qchem :: load_nao(size_t &nao) {
        //here we grab our nao through the setup of the 1e1c basis
        //trhough libqints. This can also be done through a rem_read
        //but we need the basis later so this is just copy paste 
        libqints::basis_1e1c_cgto<double> basis = libqints::qchem::aobasis.b1;

        nao = basis.get_nbsf();
    
    }

    void Loader_qmp2_from_qchem :: load_prnt_lvl(int &prnt_lvl) {

        int prnt_lvl_qchem = rem_read(REM_QQC_PRINT_LEVEL);
        prnt_lvl = (prnt_lvl_qchem > 0) ? prnt_lvl_qchem : 0;
    
    }

    void Loader_qmp2_from_qchem :: load_1Dgrid(Grid &grid) {

        //for now we have hardcoded the 1D grid until 
        //we have a full implementation
        //bool Grid = rem_read(REM_QQC_GRID);
	//if (Grid ) cout<< "recognized \n";
	size_t npts = 4;
        size_t ndim = 1;
        double pts[npts * ndim];
        double wts[npts];

        pts[0] = 3.30009478207572e-01;
        pts[1] = 6.94318442029737e-02;
        pts[2] = 9.30568155797026e-01;
        pts[3] = 6.69990521792428e-01;

        wts[0] = 3.26072577431273e-01;
        wts[1] = 1.73927422568727e-01;
        wts[2] = 1.73927422568727e-01;
        wts[3] = 3.26072577431273e-01;

        grid.set_grid(npts, ndim, pts, wts);
    
    }

    void Loader_qmp2_from_qchem :: load_3Dgrid(Grid &grid, Grid3D<vector<double>, size_t> &grid3D) {
       	bool Becke = rem_read(REM_QQC_GRID);
        // when bool is True - use coded Becke grid
	if (Becke==true ){
	XCAtoms xcatom;
	int n_points=1000; // give number of points
        size_t dim =3.0; // give dimension
	int nAtoms = xcatom.getNAtoms(); // getting atom coordinates
	double bohr_to_ang = 0.529177249; // unit transform factor for bohr to Angstrom 
	int nr_bonds = 2; //number of bonds for molecule, 2 for H2O
	vector<double> x;
	vector<double> y;
	vector<double> z;
	for( int a = 0; a < nAtoms ; a++){ // getting coordinates and saving in respective vector
		double* xyz= new double[dim];
        	xcatom.getXYZ(a, xyz);
		x.insert(x.end(), xyz[0]);
                y.insert(y.end(), xyz[1]);
                z.insert(z.end(), xyz[2]);
		delete[] xyz;
	}
	//adding the midpoints of bonds - for H2O five center method
        //x.insert(x.end(), {(x[0]+x[1])/2,(x[1]+x[2])/2});
        //y.insert(y.end(), {(y[0]+y[1])/2,(y[1]+y[2])/2});
        //z.insert(z.end(), {-0.5, 0.5});
	//z.insert(z.end(), {(z[0]+z[1])/2,(z[1]+z[2])/2});
        // change unit from Bohr to Angstroem
	transform(x.begin(), x.end(), x.begin(), [bohr_to_ang](double c){ return c*bohr_to_ang;});
	transform(y.begin(), y.end(), y.begin(), [bohr_to_ang](double c){ return c*bohr_to_ang;});
	transform(z.begin(), z.end(), z.begin(), [bohr_to_ang](double c){ return c*bohr_to_ang;});
	
	// For elliptical grid uncomment code
	//vector<vector<vector<double>>> foci;
	//vector<vector<double>> center;
	//for( int b = 0; b <nr_bonds ; b++){
	//	vector<vector<double>> foc;
	//	vector<double> f0{x[b], y[b], z[b]};
	//       vector<double> f1{x[b+1], y[b+1], z[b+1]};
	//	foc.push_back(f0);
	//	foc.push_back(f1);
	//      vector<double> cen{(f0[0]+f1[0])/2,(f0[1]+f1[1])/2, (f0[2]+f1[2])/2 };
	//	center.push_back(cen);	
	//	foci.push_back(foc);  
	//}
	//grid3D.set_ellgrid_wrapper(center,foci,x,y,z,n_points, dim);

	grid3D.set_grid_wrapper(x, y, z, n_points, dim);
	}
	//when bool False - use grid from qchem
	if(Becke==false){    
	//here we use the grid from libdftn, which is a becke
        //weighted grid. This will also be replaced by our own 
        //implementation

        // This is setup for the molecular grid class,
        // from where we will get our 3D grid points. 
        // Most names are self explanatory, the rest ist 
        // QChem magic.
	 
        XCAtoms xcatom; 
        int jobID0 = 0; 
        XCFunctional Func(0,0); 
        XCJobPara xcpara(Func, xcatom, jobID0);
        double thresh = xcpara.thresh; 
        int nAtoms = xcatom.getNAtoms();

        //Grid type, in Q-Chem this can be one of the standard
        //grids (SG-1) or your own definition of the number
        //of angular and lebedev points. Howerver, you write
        //it in a very awkward string, e.g. 20,38 grid points become
        //20000038....
        int grdTyp = rem_read(REM_IGRDTY);

        //basis set
        int IBCode = rem_read(REM_IBASIS);
        SET_QC_FCOMMON(&IBCode); 

        XCBasisSet basDen(IBCode, nAtoms, thresh); 
        xcatom.setSize(basDen); 

        MoleGrid mgrid(xcatom, grdTyp, xcpara.nDrvNuc, thresh);

        //not sure why we need the batching, but it is required...
        int *iPtBat= new int[mgrid.getNBatch()+1]; 
        int npts=mgrid.getNPts();
        int nbatch=mgrid.getNBatch();

        //finally, we init our own memory for pts and wts, 
        //hand it over to the molecular grid, get back our data
        //and set then our grid, where the data is copied
        double pts[npts * 3];
        double wts[npts];

        mgrid.getMolGrid(pts, wts, iPtBat, npts, nbatch); 

        grid.set_grid(npts, 3, pts, wts);

        delete[] iPtBat;
	}	
    }

    void Loader_qmp2_from_qchem :: load_mat_fock(double* mat_fock) {
        size_t nao = 0;
        load_nao(nao);

        size_t nocc = 0;
        load_nocc(nocc);
        size_t nvirt = 0;
        load_nvirt(nvirt);
        size_t nmo = nocc + nvirt;

        double coeff[nao * nmo];
        //FileMan is one of those very confusion Q-Chem functions which
        //has a lot of Fortran left in it. Basically,
        //the constats like FILE_MO_COEFS are an integer declared in 
        //some file, so we need to set the right integer to read in the 
        //matrix, here it is the fock MO coefficient and fock matrix
        FileMan(FM_READ, FILE_MO_COEFS, FM_DP, nao * nmo, 0, FM_BEG, coeff);
        double fock_ao[nao * nao];

        FileMan(FM_READ, FILE_FOCK_MATRIX, FM_DP, nao * nao, 0, FM_BEG, fock_ao); 

        //and then we do the AO to MO transformation

#pragma omp parallel for schedule(dynamic) default(none)\
        shared(nmo, nao, fock_ao, coeff, mat_fock)\
        collapse(2)
        for (size_t q = 0; q < nmo; q++){
            for (size_t p = 0; p < nmo; p++){
                mat_fock[q * nmo + p] = 0;
                for (size_t l = 0; l < nao; l++){
                    double temp = 0;
                    for (size_t k = 0; k < nao; k++){
                        //temp += fock_ao[l * nao + k] * coeff_trans[k * nmo + q];
                        temp += fock_ao[l * nao + k] * coeff[q * nao + k];
                    }
                    //mat_fock[q * nmo + p] += coeff[l * nmo + p] * temp;
                    mat_fock[q * nmo + p] += coeff[p * nao + l] * temp;
                }
            }
        } 

    }

    void Loader_qmp2_from_qchem :: load_mat_cgto(double* mat_cgto) {
        Ttimer timings(0);
        timings.start_new_clock("Timings AoToMo CGTO: ", 0, 0);
        
	bool Becke = rem_read(REM_QQC_GRID);
	Grid3D<vector<double>, size_t> grid3D;
	Grid grid;
	load_3Dgrid(grid, grid3D); // should load my grid when Bool is true
	// when bool is True - use coded Becke grid
	size_t npts;
	if (Becke==true) npts = grid3D.get_mnpts() ; // Grid3D.get_mnpts();
	// when bool False - use grid from Qchem
	if(Becke==false) npts = grid.get_mnpts();
	
        size_t nao = 0;
        load_nao(nao);

        size_t nocc = 0;
        load_nocc(nocc);
        size_t nvirt = 0;
        load_nvirt(nvirt);
        size_t nmo = nocc + nvirt;

        double coeff[nao * nmo];
        //FileMan is one of those very confusion Q-Chem functions which
        //has a lot of Fortran left in it. Basically,
        //the constats like FILE_MO_COEFS are an integer declared in 
        //some file, so we need to set the right integer to read in the 
        //matrix, here it is the fock MO coefficient and fock matrix
        FileMan(FM_READ, FILE_MO_COEFS, FM_DP, nao * nmo, 0, FM_BEG, coeff);

        double* cgto_ao = new double[npts * nao];


        //first we set what is basically vector fiew of our grid points
        double* mpts;
	if(Becke==true) mpts = grid3D.get_mpts();
	if(Becke==false) mpts = grid.get_mpts();

	libqints::array_view<double> av_pts(mpts, npts * 3);

        //we need both the cgto basis and the delta matrix
        libqints::basis_1e1c_cgto<double> basis = libqints::qchem::aobasis.b1;
        libqints::basis_1e1c_delta bg(npts, av_pts);
        //omp definitions of Q-Chem, required
        libqints::dev_omp dev;
        dev.init(32 * 1024UL * 1024); 
        //setup an vector view on our data holder
        libqints::array_view<double> av_cgto(cgto_ao, npts * nao);
        //use libgrid to evaluate the basis function on our grid points
        int error = libgrid::eval_cgto(basis, bg, dev, av_cgto);

        // Q-Chem evaluates each basis function for every point first, we need
        // the transposed of that to stay in cache better (looping over
        // the AOs as the fast index). The object is small enough
        // that the copy won't hurt us.
        double* cgto_trans = new double[npts * nao];

        for (int i = 0; i < npts; i ++){
                for (int j = 0; j < nao; j++){
                        cgto_trans[i * nao + j] = cgto_ao [j * npts + i];
                }
        }
        delete[] cgto_ao;

        timings.start_new_clock("    -- Transformation: ", 2, 0);
        // Orbitals O $O_{MO} = O * C$
        //
        size_t pos = 0; // Position on virtual orbital space
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(npts, nmo, nao, nocc, nvirt, cgto_trans, coeff, mat_cgto)\
        private(pos) \
        collapse(2)
        for (size_t p = 0; p < npts; p++){
            for (size_t q = 0; q < nmo; q++){
                mat_cgto[p * nmo + q] = 0;
                for (size_t k = 0; k < nao; k++){
                        mat_cgto[p * nmo + q] += cgto_trans[p * nao + k] 
                            * coeff[q * nao + k];
                        //mat_cgto[p * nmo + q] += cgto_ao[p * nao + k] 
                         //   * coeff[k * nmo + q];
                }
            }
        }
        timings.stop_clock(2);
        timings.stop_clock(0);
        cout << timings.print_all_clocks() << endl;
        
        delete[] cgto_trans;
    }

    void Loader_qmp2_from_qchem :: load_cube_coul(double* cube_coul) {
        Ttimer  timings(0);
        timings.start_new_clock("Timings AoToMo Coulomb Integral: ", 0, 0);
        bool Becke = rem_read(REM_QQC_GRID);
	Grid grid;
        Grid3D<vector<double>, size_t> grid3D;
	load_3Dgrid(grid, grid3D);
        libqints::basis_1e1c_cgto<double> basis = libqints::qchem::aobasis.b1;

        size_t nao = 0;
        load_nao(nao);

        size_t nocc = 0;
        load_nocc(nocc);
        size_t nvirt = 0;
        load_nvirt(nvirt);
        size_t nmo = nocc + nvirt;

        double coeff[nao * nmo];
        //FileMan is one of those very confusion Q-Chem functions which
        //has a lot of Fortran left in it. Basically,
        //the constats like FILE_MO_COEFS are an integer declared in 
        //some file, so we need to set the right integer to read in the 
        //matrix, here it is the fock MO coefficient and fock matrix
        FileMan(FM_READ, FILE_MO_COEFS, FM_DP, nao * nmo, 0, FM_BEG, coeff);
	
	size_t npts;
        if (Becke==true) npts = grid3D.get_mnpts() ; // Grid3D.get_mnpts();
        // when bool False - use grid from Qchem
        if(Becke==false) npts = grid.get_mnpts();
	//First we need to set up the multipole object for each point which we
        //will pass to the digester for the integral
        std::vector< libqints::ftype_multipole > v_pts;
	if(Becke==true){
	 for (int i = 0; i < npts; i++){
            libqints::ftype_multipole f_mult;
            f_mult.x = grid3D.get_mpts()[i * 3 + 0];
            f_mult.y = grid3D.get_mpts()[i * 3 + 1];
            f_mult.z = grid3D.get_mpts()[i * 3 + 2];
            f_mult.k = 0;
            v_pts.push_back(f_mult);
        }//for i
	}
	if(Becke==false){
        for (int i = 0; i < npts; i++){
            libqints::ftype_multipole f_mult;
            f_mult.x = grid.get_mpts()[i * 3 + 0];
            f_mult.y = grid.get_mpts()[i * 3 + 1];
            f_mult.z = grid.get_mpts()[i * 3 + 2];
            f_mult.k = 0;
            v_pts.push_back(f_mult);
        }//for i
	}
	libqints::basis_1e1c_multipole bm (v_pts);

        double mom[npts];
        for (int i = 0; i < npts; i++) mom[i] = 1;
        libqints::array_view<double> av_mom((double*) &mom[0], npts);
        libqints::dev_omp dev;
        dev.init(256UL * 1024 * 1024);

        double* coul_ao = new double[npts * nao * nao];

        libqints::array_view<double> av_vmul(coul_ao, npts*nao*nao);

        //This is the digester, which we grabbed from libfock and modified 
        //it for out integral. This gives us back the evaluated AO integrals
        qqc_vmul<double>(basis, bm, dev).compute(av_mom, av_vmul);

        timings.start_new_clock("    -- Transformation: ", 2, 0);
        // Coulomb Integral U_{MO}^P: for each slice P 
        // $U_{MO} = C_{occpuid}^T * (u_{AO}^P * C_{virtuals}
        //
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(npts, nocc, nvirt, nao, nmo, coul_ao, coeff, cube_coul)\
        collapse(3)
        for (size_t p = 0; p < npts; p++){
            for (size_t i = 0; i < nocc; i++){
                for (size_t a = 0; a < nvirt; a++){
                    cube_coul[p * nvirt * nocc + i * nvirt + a] = 0;
                    size_t pos_a = nocc + a;
                    for (size_t l = 0; l < nao; l++){
                        double temp = 0;
                        for (size_t k = 0; k < nao; k++){
                            //temp += coul_ao[p * nao * nao + l * nao + k] 
                            //    * coeff[k * nmo + pos_a];
                            temp += coul_ao[p * nao * nao + l * nao + k] 
                                * coeff[pos_a * nao + k];
                        }
                        //cube_coul [p * nvirt * nocc + i * nvirt + a] += 
                        //    coeff[l * nmo + i] * temp;
                        cube_coul [p * nvirt * nocc + i * nvirt + a] += 
                            coeff[i * nao + l] * temp;
                    }
                }
            }
        }
        timings.stop_clock(2);
        timings.stop_clock(0);
        cout << timings.print_all_clocks() << endl;

        delete[] coul_ao;
    }

} //namespace libqq

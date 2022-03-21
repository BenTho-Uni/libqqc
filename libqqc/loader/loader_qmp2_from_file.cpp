///
/// member functions of the loader class which loads values and returns them to
/// a reference
/// @TODO this is just a dummy implementation, needs to be fully implemented
/// @file loader_qmp2_from_file.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 03-02-2022
//

#include "loader_qmp2_from_file.h"
#include "../utils/load_from_file.h"

using namespace std;

namespace libqqc {

    void Loader_qmp2_from_file :: load_nocc (size_t &nocc){

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);

        nocc = array[12];

    } //Loader_qmp2_from_file::load_nocc

    void Loader_qmp2_from_file :: load_nvirt(size_t &nvirt) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);

        nvirt = array[13];
    
    }

    void Loader_qmp2_from_file :: load_nao(size_t &nao) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);

        nao = array[15];
    
    }


    void Loader_qmp2_from_file :: load_prnt_lvl(int &prnt_lvl) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);

        prnt_lvl = array[11];
    
    }

    void Loader_qmp2_from_file :: load_grid(string filename_pts, 
            string filename_wts, Grid &grid) {

        // Loading in the points
        vector<size_t> dim_pts = load_dim_from_file(msrc_folder+filename_pts, ' ', 1);
        double pts[dim_pts.at(0) * dim_pts.at(1) * dim_pts.at(2)];
        load_array_from_file(msrc_folder+filename_pts, dim_pts, pts, ' ', 1);

        // Loading in the weights
        vector<size_t> dim_wts = load_dim_from_file(msrc_folder+filename_wts, ' ', 1);

        // Check if number of points are the same
        if (dim_pts.at(1) != dim_wts.at(1)) 
            throw invalid_argument("Number of points of pts and wts differ.");

        double wts[dim_wts.at(0) * dim_wts.at(1) * dim_wts.at(2)];
        load_array_from_file(msrc_folder+filename_wts, dim_wts, wts, ' ', 1);

        grid.set_grid(dim_pts.at(1), dim_pts.at(0), pts, wts);
    
    }

    void Loader_qmp2_from_file :: load_mat_fock(double* mat_fock) {

        size_t nao = 0;
        load_nao(nao);

        size_t nocc = 0;
        load_nocc(nocc);

        size_t nvirt = 0;
        load_nvirt(nvirt);

        size_t nmo = nocc + nvirt;

        vector<size_t> dim_fock = {nao, nao, 1};
        double fock_ao[nao * nao];
        load_array_from_file(msrc_folder+mfname_fock, dim_fock, fock_ao, ' ', 1);

        vector<size_t> dim_coeff = {nao, nmo, 1};
        double coeff[nao * nmo];
        load_array_from_file(msrc_folder+mfname_coeff, dim_coeff, coeff, ' ', 1);

#pragma omp parallel for schedule(dynamic) default(none)\
        shared(nmo, nao, fock_ao, coeff, mat_fock)\
        collapse(2)
        for (size_t q = 0; q < nmo; q++){
            for (size_t p = 0; p < nmo; p++){
                mat_fock[q * nmo + p] = 0;
                for (size_t l = 0; l < nao; l++){
                    double temp = 0;
                    for (size_t k = 0; k < nao; k++){
                        temp += fock_ao[l * nao + k] * coeff[k * nmo + q];
                    }
                    mat_fock[q * nmo + p] += coeff[l * nmo + p] * temp;
                }
            }
        } 
    }

    void Loader_qmp2_from_file :: load_mat_coeff(double* mat_coeff) {
        size_t nao = 0;
        load_nao(nao);

        size_t nocc = 0;
        load_nocc(nocc);

        size_t nvirt = 0;
        load_nvirt(nvirt);

        size_t nmo = nocc + nvirt;

        vector<size_t> dim = {nao, nmo, 1};

        load_array_from_file(msrc_folder+mfname_coeff, dim, mat_coeff, ' ', 1);
    
    }

    void Loader_qmp2_from_file :: load_mat_cgto(double* mat_cgto) {
        size_t nao = 0;
        load_nao(nao);

        size_t nocc = 0;
        load_nocc(nocc);

        size_t nvirt = 0;
        load_nvirt(nvirt);

        size_t nmo = nocc + nvirt;

        Grid p3Dgrid;
        load_3Dgrid(p3Dgrid);
        size_t npts = p3Dgrid.get_mnpts();

        vector<size_t> dim_ao = {npts, nao, 1};
        vector<size_t> dim_mo = {npts, nmo, 1};

        vector<size_t> dim_coeff = {nao, nmo, 1};
        double coeff[nao * nmo];
        load_array_from_file(msrc_folder+mfname_coeff, dim_coeff, coeff, ' ', 1);

        double* cgto_ao = new double[npts * nao];
        load_array_from_file(msrc_folder+mfname_cgto, dim_ao, cgto_ao, ' ', 1);

        // Orbitals O $O_{MO} = O * C$
        //
        size_t pos = 0; // Position on virtual orbital space
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(npts, nmo, nao, nocc, nvirt, cgto_ao, coeff, mat_cgto)\
        private(pos) \
        collapse(2)
        for (size_t p = 0; p < npts; p++){
            for (size_t q = 0; q < nmo; q++){
                mat_cgto[p * nmo + q] = 0;
                for (size_t k = 0; k < nao; k++){
                        mat_cgto[p * nmo + q] += cgto_ao[p * nao + k] 
                            * coeff[k * nmo + q];
                }
            }
        }
    
        delete[] cgto_ao;
    }

    void Loader_qmp2_from_file :: load_cube_coul(double* cube_coul) {
        size_t nao = 0;
        load_nao(nao);

        size_t nocc = 0;
        load_nocc(nocc);

        size_t nvirt = 0;
        load_nvirt(nvirt);

        size_t nmo = nocc + nvirt;

        Grid p3Dgrid;
        load_3Dgrid(p3Dgrid);
        size_t npts = p3Dgrid.get_mnpts();

        vector<size_t> dim_ao = {nao, nao, npts};
        vector<size_t> dim_mo = {nocc, nvirt, npts};

        vector<size_t> dim_coeff = {nao, nmo, 1};
        double coeff[nao * nmo];
        load_array_from_file(msrc_folder+mfname_coeff, dim_coeff, coeff, ' ', 1);

        double* coul_ao = new double[npts * nao * nao];

        load_array_from_file(msrc_folder+mfname_coul, dim_ao, coul_ao, ' ', 1);

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
                            temp += coul_ao[p * nao * nao + l * nao + k] 
                                * coeff[k * nmo + pos_a];
                        }
                        cube_coul [p * nvirt * nocc + i * nvirt + a] += 
                            coeff[l * nmo + i] * temp;
                    }
                }
            }
        }

        delete[] coul_ao; 
    }

} //namespace libqqc

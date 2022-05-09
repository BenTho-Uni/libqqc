///
/// member functions of the loader class which loads values and returns them to
/// a reference
/// @TODO this is just a dummy implementation, needs to be fully implemented
/// @file loader_qmp2_from_qchem.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 11-01-2022
//

#include "loader_qmp2_from_qchem.h"

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

        cout << "Loader reached!\n";
        nocc = 1; //

    } //Loader_qmp2_from_qchem::load_nocc

    void Loader_qmp2_from_qchem :: load_nvirt(size_t &nvirt) {

        nvirt = 2;
    
    }

    void Loader_qmp2_from_qchem :: load_nao(size_t &nao) {

        nao = 3;
    
    }

    void Loader_qmp2_from_qchem :: load_prnt_lvl(int &prnt_lvl) {

        prnt_lvl = 0;
    
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

        size_t npts = 2;
        size_t ndim = 3;
        double pts[npts * ndim];
        double wts[npts];

        fill_array (npts * ndim, pts);
        fill_array (npts, wts);

        grid.set_grid(npts, ndim, pts, wts);

    }

    void Loader_qmp2_from_qchem :: load_mat_fock(double* mat_fock) {

        size_t nao = 3;

        fill_array (nao * nao, mat_fock); 

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

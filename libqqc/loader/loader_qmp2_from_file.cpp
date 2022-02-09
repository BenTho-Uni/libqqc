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

    void Loader_qmp2_from_file :: load_nocc (string filename, size_t &nocc){

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);

        nocc = array[12];

    } //Loader_qmp2_from_file::load_nocc

    void Loader_qmp2_from_file :: load_nvirt(string filename, size_t &nvirt) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);

        nvirt = array[13];
    
    }

    void Loader_qmp2_from_file :: load_nao(string filename, size_t &nao) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);

        nao = array[15];
    
    }

    void Loader_qmp2_from_file :: load_1Dtol(string filename, double &p1Dtol) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);

        p1Dtol = array[9];

    
    }

    void Loader_qmp2_from_file :: load_prnt_lvl(string filename, int &prnt_lvl) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);

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

    void Loader_qmp2_from_file :: load_mat_fock(string filename, 
            double* mat_fock, size_t dim1, size_t dim2) {

        vector<size_t> dim = {dim1, dim2, 1};

        load_array_from_file(msrc_folder+filename, dim, mat_fock, ' ', 1);
    
    }

    void Loader_qmp2_from_file :: load_mat_coeff(string filename, 
            double* mat_coeff, size_t dim1, size_t dim2) {

        vector<size_t> dim = {dim1, dim2, 1};

        load_array_from_file(msrc_folder+filename, dim, mat_coeff, ' ', 1);
    
    }

    void Loader_qmp2_from_file :: load_mat_cgto(string filename, 
            double* mat_cgto, size_t dim1, size_t dim2) {

        vector<size_t> dim = {dim1, dim2, 1};

        load_array_from_file(msrc_folder+filename, dim, mat_cgto, ' ', 1);
    
    }

    void Loader_qmp2_from_file :: load_cube_coul(string filename, 
            double* cube_coul, size_t dim1, size_t dim2, size_t dim3) {

        vector<size_t> dim = {dim1, dim2, dim3};

        load_array_from_file(msrc_folder+filename, dim, cube_coul, ' ', 1);
    
    }

} //namespace libqqc

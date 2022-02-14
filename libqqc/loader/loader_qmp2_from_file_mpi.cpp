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

#include <mpi.h>
#include <iostream>


using namespace std;

namespace libqqc {

    void Loader_qmp2_from_file :: load_nocc (string filename, size_t &nocc){

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI

        nocc = array[12];

    } //Loader_qmp2_from_file::load_nocc

    void Loader_qmp2_from_file :: load_nvirt(string filename, size_t &nvirt) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI

        nvirt = array[13];
    
    }

    void Loader_qmp2_from_file :: load_nao(string filename, size_t &nao) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI

        nao = array[15];
    
    }

    void Loader_qmp2_from_file :: load_1Dtol(string filename, double &p1Dtol) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI

        p1Dtol = array[9];

    
    }

    void Loader_qmp2_from_file :: load_prnt_lvl(string filename, int &prnt_lvl) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI

        prnt_lvl = array[11];
    
    }

    void Loader_qmp2_from_file :: load_grid(string filename_pts, 
            string filename_wts, Grid &grid) {

        // Create empty dimension vectors
        vector<size_t> dim_pts = {};
        vector<size_t> dim_wts = {};

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 


        size_t dim_pts_size = 0;
        size_t dim_wts_size = 0;
        if (pid == 0){
            // On the master node with pid 0 load in the input file
            dim_pts = load_dim_from_file(msrc_folder+filename_pts, ' ', 1);
            dim_wts = load_dim_from_file(msrc_folder+filename_wts, ' ', 1);
            if (dim_pts.at(1) != dim_wts.at(1)) 
                throw invalid_argument("Number of points of pts and wts differ.");
            dim_pts_size = dim_pts.size();
            dim_wts_size = dim_wts.size();
        }
        //Send sizes of vectors, so we can set them up for broadcast
        MPI_Bcast(&dim_pts_size, 1, MPI_COUNT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&dim_wts_size, 1, MPI_COUNT, 0, MPI_COMM_WORLD);
        // set vector size
        dim_pts.resize(dim_pts_size);
        dim_wts.resize(dim_wts_size);
        // Now we can broadcast the data
        MPI_Bcast(dim_pts.data(), dim_pts.size(), MPI_COUNT, 0, MPI_COMM_WORLD);
        MPI_Bcast(dim_wts.data(), dim_wts.size(), MPI_COUNT, 0, MPI_COMM_WORLD);
        
        double pts[dim_pts.at(0) * dim_pts.at(1) * dim_pts.at(2)];
        double wts[dim_wts.at(0) * dim_wts.at(1) * dim_wts.at(2)];

        if (pid == 0){
            // Loading in the points
            load_array_from_file(msrc_folder+filename_pts, dim_pts, pts, ' ', 1);
            // Loading in the weights
            load_array_from_file(msrc_folder+filename_wts, dim_wts, wts, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(pts, dim_pts.at(0) * dim_pts.at(1) * dim_pts.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(wts, dim_wts.at(0) * dim_wts.at(1) * dim_wts.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI

        // Check if number of points are the same

        grid.set_grid(dim_pts.at(1), dim_pts.at(0), pts, wts);
    
    }

    void Loader_qmp2_from_file :: load_mat_fock(string filename, 
            double* mat_fock, size_t dim1, size_t dim2) {

        vector<size_t> dim = {dim1, dim2, 1};

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, mat_fock, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(mat_fock, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI
    
    }

    void Loader_qmp2_from_file :: load_mat_coeff(string filename, 
            double* mat_coeff, size_t dim1, size_t dim2) {

        vector<size_t> dim = {dim1, dim2, 1};

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, mat_coeff, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(mat_coeff, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI
    
    }

    void Loader_qmp2_from_file :: load_mat_cgto(string filename, 
            double* mat_cgto, size_t dim1, size_t dim2) {

        vector<size_t> dim = {dim1, dim2, 1};

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, mat_cgto, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(mat_cgto, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI
    
    }

    void Loader_qmp2_from_file :: load_cube_coul(string filename, 
            double* cube_coul, size_t dim1, size_t dim2, size_t dim3) {

        vector<size_t> dim = {dim1, dim2, dim3};

        // Open the MPI environment to load the file 
        //MPI_Init(NULL, NULL);
        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+filename, dim, cube_coul, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(cube_coul, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //MPI_Finalize(); // Close MPI
    
    }

} //namespace libqqc

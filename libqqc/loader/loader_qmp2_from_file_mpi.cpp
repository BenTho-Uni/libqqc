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
#include "../utils/ttimer.h"

#include <mpi.h>
#include <iostream>



using namespace std;

namespace libqqc {

    void Loader_qmp2_from_file :: load_nocc (size_t &nocc){

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        nocc = array[12];
    } //Loader_qmp2_from_file::load_nocc

    void Loader_qmp2_from_file :: load_nvirt(size_t &nvirt) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        nvirt = array[13];
    }

    void Loader_qmp2_from_file :: load_nao(size_t &nao) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        nao = array[15];
    }

    void Loader_qmp2_from_file :: load_prnt_lvl(int &prnt_lvl) {

        vector<size_t> dim = {16, 1, 1};

        double array[dim.at(0) * dim.at(1) * dim.at(2)];

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        if (pid == 0){
            // On the master node with pid 0 load in the input file
            load_array_from_file(msrc_folder+mfname_inputs, dim, array, ' ', 1);
        }
        // Now distribute the data to all nodes
        MPI_Bcast(array, dim.at(0) * dim.at(1) * dim.at(2),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

        prnt_lvl = array[11];

    }

    void Loader_qmp2_from_file :: load_grid(string filename_pts, 
            string filename_wts, Grid &grid) {

        // Create empty dimension vectors
        vector<size_t> dim_pts = {};
        vector<size_t> dim_wts = {};

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

        grid.set_grid(dim_pts.at(1), dim_pts.at(0), pts, wts);
    }

    void Loader_qmp2_from_file :: load_mat_fock(double* mat_fock) {

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        // First lets init the dimensions, load them in on master and distribute
        // them
        size_t nao = 0;
        size_t nocc = 0;
        size_t nvirt = 0;

        load_nao(nao);
        load_nocc(nocc);
        load_nvirt(nvirt);
        size_t nmo = nocc + nvirt;

        //Now we calculate the AOtoMO transformed Fock Matrix on master, as this
        //will be very small

        if (pid == 0){

            // read in the AO fock matrix
            vector<size_t> dim_fock = {nao, nao, 1};
            double fock_ao[nao * nao];
            load_array_from_file(msrc_folder+mfname_fock, dim_fock, 
                    fock_ao, ' ', 1);

            // read in the coefficient C matrix
            vector<size_t> dim_coeff = {nao, nmo, 1};
            double coeff[nao * nmo];
            load_array_from_file(msrc_folder+mfname_coeff, dim_coeff, 
                    coeff, ' ', 1);

            //transform the Fock matrix
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
        MPI_Bcast(mat_fock, nmo * nmo,
                MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    void Loader_qmp2_from_file :: load_mat_coeff(double* mat_coeff) {

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        // First lets init the dimensions, load them in on master and distribute
        // them
        size_t nao = 0;
        size_t nocc = 0;
        size_t nvirt = 0;

        load_nao(nao);
        load_nocc(nocc);
        load_nvirt(nvirt);
        size_t nmo = nocc + nvirt;

        //Then load the coeff C matrix in on master
        vector<size_t> dim_coeff = {nao, nmo, 1};
        if (pid == 0){
            // read in the coefficient C matrix
            load_array_from_file(msrc_folder+mfname_coeff, dim_coeff, 
                    mat_coeff, ' ', 1);
        }

        // Now distribute the data to all nodes
        MPI_Bcast(mat_coeff, nao * nmo,
                MPI_DOUBLE, 0, MPI_COMM_WORLD);
 
    }

    void Loader_qmp2_from_file :: load_mat_cgto(double* mat_cgto) {

        Ttimer timings(0);
        timings.start_new_clock("Timings AoToMo CGTO: ", 0, 0);

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        // First lets init the dimensions, load them in on master and distribute
        // them
        size_t nao = 0;
        size_t nocc = 0;
        size_t nvirt = 0;
        load_nao(nao);
        load_nocc(nocc);
        load_nvirt(nvirt);
        size_t nmo = nocc + nvirt;

        //for the number of 3D points we need to load in the grid 
        size_t p3Dnpts = 0;
        Grid p3Dgrid;
        load_3Dgrid(p3Dgrid);
        p3Dnpts = p3Dgrid.get_mnpts();
        
        // Now load in the coeff C matrix and the AO CGTO matrix on master,
        // distribute the CMatrix and the part of the AO matrix that matches
        // the batch load. For this, we have to first calculate the batch. 
        size_t remaining_elements = p3Dnpts % max_id;
        size_t npts_to_proc = p3Dnpts / max_id 
            + ((pid != 0) ? 0 : remaining_elements);
        size_t offset = pid * npts_to_proc
            + ((pid != 0) ? remaining_elements : 0);

        // Set up the coefficienct C matrix
        vector<size_t> dim_coeff = {nao, nmo, 1};
        double coeff[nao * nmo];

        if (pid == 0){
            //read in on master
            load_array_from_file(msrc_folder+mfname_coeff, dim_coeff, 
                    coeff, ' ', 1);
        }
        //Distribute the Coefficient matrix
        MPI_Bcast(coeff, nao * nmo, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Now set up the AO part of the cgto matrix for this node
        double* cgto_ao_node = new double[npts_to_proc * nao];
        vector<size_t> dim_ao = {p3Dnpts, nao, 1};

        // Setting up a maximum number of items to send/receiv to identify when
        // batching is needed
        size_t max_items = 4294967294; //max n-1 represented in 32bit
        size_t n_items = (npts_to_proc + ((pid != 0) ? remaining_elements : 0)) 
            * nao; // largest number of items to send
        size_t n_n_items = n_items / max_items + 1; // number of send/resc batches
        size_t remaining_to_send = 0;
        size_t npts_to_send = 0;
        size_t offset_to_send = 0;

        if (pid == 0) {
            //on master, read in the full cgto matrix
            double* cgto_ao_full = new double[p3Dnpts * nao];
            timings.start_new_clock("    -- Loading in: ", 1, 0);
            load_array_from_file(msrc_folder+mfname_cgto, dim_ao, cgto_ao_full,
                    ' ', 1);
            timings.stop_clock(1);

            timings.start_new_clock("    -- Distribute Batch: ", 2, 0);
            //now distribute the batch part of the AO matrix 
            //
            for (int i = 1; i < max_id; i++){
                size_t npts_to_proc_on_i = p3Dnpts / max_id 
                    + ((i != 0) ? 0 : remaining_elements);
                size_t offset_on_i = i * npts_to_proc_on_i
                    + ((i != 0) ? remaining_elements : 0);
            //    MPI_Send(cgto_ao_full + (offset_on_i * nao), 
             //           (npts_to_proc_on_i * nao), 
              //          MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                for (int iter = 0; iter < n_n_items; iter++){
                    remaining_to_send = npts_to_proc_on_i % n_n_items;
                    npts_to_send = npts_to_proc_on_i / n_n_items
                         + ((iter == 0) ? remaining_to_send : 0);
                    offset_to_send = iter * npts_to_send 
                        + ((iter == 0) ? 0 : remaining_to_send); 
                    MPI_Send(cgto_ao_full 
                            + ((offset_to_send + offset_on_i) * nao), 
                            (npts_to_send * nao), 
                            MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                }
            }
            timings.stop_clock(2);
            // Delete Full array from before
            for (size_t p = 0; p < npts_to_proc; p++){
                for (size_t a = 0; a < nao; a++){
                    cgto_ao_node[p * nao + a] = cgto_ao_full[p * nao + a];
                }
            }
            delete[] cgto_ao_full;
        }
        else {
           // MPI_Recv(cgto_ao_node, (npts_to_proc * nao), MPI_DOUBLE, 0, 0, 
           //         MPI_COMM_WORLD, &status);
            for (int iter = 0; iter < n_n_items; iter++){
                remaining_to_send = npts_to_proc % n_n_items;
                npts_to_send = npts_to_proc / n_n_items
                     + ((iter == 0) ? remaining_to_send : 0);
                offset_to_send = iter * npts_to_send 
                    + ((iter == 0) ? 0 : remaining_to_send); 
                MPI_Recv(cgto_ao_node + (offset_to_send * nao), 
                        (npts_to_send * nao), MPI_DOUBLE, 0, 0, 
                        MPI_COMM_WORLD, &status);
            }
        }


        timings.start_new_clock("    -- Tranforming Batch: ", 3, 0);
        //Now we do the transformation
        // Orbitals O $O_{MO} = O * C$
        //
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(npts_to_proc, nmo, nao, nocc, nvirt, cgto_ao_node, coeff, mat_cgto, offset)\
        collapse(2)
        for (size_t p = 0; p < npts_to_proc; p++){
            for (size_t q = 0; q < nmo; q++){
                size_t p_offset = p + offset;
                mat_cgto[p_offset * nmo + q] = 0;
                for (size_t k = 0; k < nao; k++){
                    mat_cgto[p_offset * nmo + q] += cgto_ao_node[p * nao + k] 
                        * coeff[k * nmo + q];
                }
            }
        }
        timings.stop_clock(3);

        timings.start_new_clock("    -- Distribute Batch Results: ", 4, 0);
        //Now get all the Data that we didnt calculate on this node
        //
        for (int i = 0; i < max_id; i++){
            if (i == pid){
                /* MPI_Bcast(mat_cgto + (offset * nmo), (npts_to_proc * nmo), MPI_DOUBLE, */ 
                /*         pid, MPI_COMM_WORLD); */
                for (int iter = 0; iter < n_n_items; iter++){
                    remaining_to_send = npts_to_proc % n_n_items;
                    npts_to_send = npts_to_proc / n_n_items
                        + ((iter == 0) ? remaining_to_send : 0);
                    offset_to_send = iter * npts_to_send 
                        + ((iter == 0) ? 0 : remaining_to_send); 
                    MPI_Bcast(mat_cgto + ((offset + offset_to_send) * nmo), 
                            (npts_to_send * nmo), MPI_DOUBLE, 
                            pid, MPI_COMM_WORLD);
                }
            }
            else {
                size_t npts_to_proc_on_i = p3Dnpts / max_id 
                    + ((i != 0) ? 0 : remaining_elements);
                size_t offset_on_i = i * npts_to_proc_on_i
                    + ((i != 0) ? remaining_elements : 0);
                /* MPI_Bcast(mat_cgto + (offset_on_i * nmo), (npts_to_proc_on_i * nmo), */ 
                /*         MPI_DOUBLE, i, MPI_COMM_WORLD); */
                for (int iter = 0; iter < n_n_items; iter++){
                    remaining_to_send = npts_to_proc_on_i % n_n_items;
                    npts_to_send = npts_to_proc_on_i / n_n_items
                        + ((iter == 0) ? remaining_to_send : 0);
                    offset_to_send = iter * npts_to_send 
                        + ((iter == 0) ? 0 : remaining_to_send); 
                    MPI_Bcast(mat_cgto + ((offset_on_i + offset_to_send) * nmo), 
                            (npts_to_send * nmo), 
                            MPI_DOUBLE, i, MPI_COMM_WORLD);
                }
            }
        }
        timings.stop_clock(4);

        delete[] cgto_ao_node;

        timings.stop_clock(0);
        if (pid == 0) cout << timings.print_all_clocks() << endl;
    }

    void Loader_qmp2_from_file :: load_cube_coul(double* cube_coul) {

        Ttimer  timings(0);
        timings.start_new_clock("Timings AoToMo Coulomb Integral: ", 0, 0);

        int pid, max_id; //pid is id of current node, max_id number of max. nodes
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); //Grab the current node id
        MPI_Comm_size(MPI_COMM_WORLD, &max_id); //Grab the max number of nodes
        MPI_Status status; 

        // First lets init the dimensions, load them in on master and distribute
        // them
        size_t nao = 0;
        size_t nocc = 0;
        size_t nvirt = 0;
        load_nao(nao);
        load_nocc(nocc);
        load_nvirt(nvirt);

        size_t nmo = nocc + nvirt;

        size_t p3Dnpts = 0;
        //for the number of 3D points we need to load in the grid 
        Grid p3Dgrid;
        load_3Dgrid(p3Dgrid);
        p3Dnpts = p3Dgrid.get_mnpts();
        
        // Now load in the coeff C matrix and the AO CGTO matrix on master,
        // distribute the CMatrix and the part of the AO matrix that matches
        // the batch load. For this, we have to first calculate the batch. 
        size_t remaining_elements = p3Dnpts % max_id;
        size_t npts_to_proc = p3Dnpts / max_id 
            + ((pid != 0) ? 0 : remaining_elements);
        size_t offset = pid * npts_to_proc
            + ((pid != 0) ? remaining_elements : 0);

        // Set up the coefficienct C matrix
        vector<size_t> dim_coeff = {nao, nmo, 1};
        double coeff[nao * nmo];

        if (pid == 0){
            //read in on master
            load_array_from_file(msrc_folder+mfname_coeff, dim_coeff, 
                    coeff, ' ', 1);
        }
        //Distribute the Coefficient matrix
        MPI_Bcast(coeff, nao * nmo, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Now set up the AO part of the coulomb matrix for this node
        double* coul_ao_node = new double[npts_to_proc * nao * nao];
        vector<size_t> dim_ao = {nao, nao, p3Dnpts};

        // Setting up a maximum number of items to send/receiv to identify when
        // batching is needed
        size_t max_items = 4294967294/8; //max n-1 represented in 32bit
        size_t n_items = (npts_to_proc + ((pid != 0) ? remaining_elements : 0)) 
            * nao * nao; // largest number of items to send
        size_t n_n_items = n_items / max_items + 1; // number of send/resc batches
        size_t remaining_to_send = 0;
        size_t npts_to_send = 0;
        size_t offset_to_send = 0;

        if (pid == 0) {
            double* coul_ao_full = new double[p3Dnpts * nao * nao];
            //on master, read in the full coulomb matrix
            //
            timings.start_new_clock("    -- Loading in: ", 1, 0);
            load_array_from_file(msrc_folder+mfname_coul, dim_ao, coul_ao_full,
                    ' ', 1);
            timings.stop_clock(1);

            timings.start_new_clock("    -- Distribute Batch: ", 4, 0);
            //now distribute the batch part of the AO matrix 
            //
            for (int i = 1; i < max_id; i++){
                size_t npts_to_proc_on_i = p3Dnpts / max_id 
                    + ((i != 0) ? 0 : remaining_elements);
                size_t offset_on_i = i * npts_to_proc_on_i
                    + ((i != 0) ? remaining_elements : 0);
                /* MPI_Send(coul_ao_full + (offset_on_i * nao * nao), */ 
                /*         (npts_to_proc_on_i * nao * nao), */ 
                /*         MPI_DOUBLE, i, 0, MPI_COMM_WORLD); */
                for (int iter = 0; iter < n_n_items; iter++){
                    remaining_to_send = npts_to_proc_on_i % n_n_items;
                    npts_to_send = npts_to_proc_on_i / n_n_items
                        + ((iter == 0) ? remaining_to_send : 0);
                    offset_to_send = iter * npts_to_send 
                        + ((iter == 0) ? 0 : remaining_to_send); 
                    MPI_Send(coul_ao_full 
                            + ((offset_on_i + offset_to_send) * nao * nao), 
                            (npts_to_send * nao * nao), 
                            MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                }
            }
            timings.stop_clock(4);

            // Delete Full array from before
            for (size_t p = 0; p < npts_to_proc; p++){
                for (size_t a = 0; a < nao; a++){
                    for (size_t b = 0; b < nao; b++){
                        coul_ao_node[p * nao * nao + a * nao + b] = 
                            coul_ao_full[p * nao * nao + a * nao + b];
                    }
                }
            }
            delete[] coul_ao_full;
        }
        else {
            /* MPI_Recv(coul_ao_node, (npts_to_proc * nao * nao), MPI_DOUBLE, 0, 0, */ 
            /*         MPI_COMM_WORLD, &status); */
            for (int iter = 0; iter < n_n_items; iter++){
                remaining_to_send = npts_to_proc % n_n_items;
                npts_to_send = npts_to_proc / n_n_items
                    + ((iter == 0) ? remaining_to_send : 0);
                offset_to_send = iter * npts_to_send 
                    + ((iter == 0) ? 0 : remaining_to_send); 
                MPI_Recv(coul_ao_node + (offset_to_send * nao * nao), 
                        (npts_to_send * nao * nao), MPI_DOUBLE, 0, 0, 
                        MPI_COMM_WORLD, &status);
            }
        }

        timings.start_new_clock("    -- Transforming Batch: ", 2, 0);
        // Coulomb Integral U_{MO}^P: for each slice P 
        // $U_{MO} = C_{occpuid}^T * (u_{AO}^P * C_{virtuals}
        //
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(offset, npts_to_proc, nocc, nvirt, nao, nmo, coul_ao_node, coeff, cube_coul)\
        collapse(3)
        for (size_t p = 0; p < npts_to_proc; p++){
            for (size_t i = 0; i < nocc; i++){
                for (size_t a = 0; a < nvirt; a++){
                    size_t p_offset = p + offset;
                    cube_coul[p_offset * nvirt * nocc + i * nvirt + a] = 0;
                    size_t pos_a = nocc + a;
                    for (size_t l = 0; l < nao; l++){
                        double temp = 0;
                        for (size_t k = 0; k < nao; k++){
                            temp += coul_ao_node[p * nao * nao + l * nao + k] 
                                * coeff[k * nmo + pos_a];
                        }
                        cube_coul [p_offset * nvirt * nocc + i * nvirt + a] += 
                            coeff[l * nmo + i] * temp;
                    }
                }
            }
        }

        timings.stop_clock(2);

        timings.start_new_clock("    -- Distribute Batch Results: ", 3, 0);

        //Now get all the Data that we didnt calculate on this node
        //
        for (int i = 0; i < max_id; i++){
            if (i == pid){
                /* MPI_Bcast(cube_coul + (offset * nocc * nvirt), */ 
                /*         (npts_to_proc * nocc * nvirt), MPI_DOUBLE, */ 
                /*         pid, MPI_COMM_WORLD); */
                for (int iter = 0; iter < n_n_items; iter++){
                    remaining_to_send = npts_to_proc % n_n_items;
                    npts_to_send = npts_to_proc / n_n_items
                        + ((iter == 0) ? remaining_to_send : 0);
                    offset_to_send = iter * npts_to_send 
                        + ((iter == 0) ? 0 : remaining_to_send); 
                    MPI_Bcast(cube_coul 
                            + ((offset +offset_to_send) * nocc * nvirt), 
                            (npts_to_send * nocc * nvirt), MPI_DOUBLE, 
                            pid, MPI_COMM_WORLD);
                }
            }
            else {
                size_t npts_to_proc_on_i = p3Dnpts / max_id 
                    + ((i != 0) ? 0 : remaining_elements);
                size_t offset_on_i = i * npts_to_proc_on_i
                    + ((i != 0) ? remaining_elements : 0);
                /* MPI_Bcast(cube_coul + (offset_on_i * nocc * nvirt), */ 
                /*         (npts_to_proc_on_i * nocc * nvirt), */ 
                /*         MPI_DOUBLE, i, MPI_COMM_WORLD); */
                for (int iter = 0; iter < n_n_items; iter++){
                    remaining_to_send = npts_to_proc_on_i % n_n_items;
                    npts_to_send = npts_to_proc_on_i / n_n_items
                        + ((iter == 0) ? remaining_to_send : 0);
                    offset_to_send = iter * npts_to_send 
                        + ((iter == 0) ? 0 : remaining_to_send); 
                    MPI_Bcast(cube_coul 
                            + ((offset_on_i + offset_to_send) * nocc * nvirt), 
                            (npts_to_send * nocc * nvirt), 
                            MPI_DOUBLE, i, MPI_COMM_WORLD);
                }
            }
        }
        timings.stop_clock(3);

        delete[] coul_ao_node;    

        timings.stop_clock(0);
        if (pid == 0) cout << timings.print_all_clocks() << endl; 
    }

} //namespace libqqc

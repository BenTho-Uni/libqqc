///
/// Member functions for the vaults used by Q-MP2 MPI Variant
/// @file vault_qmp2.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 10-02-2022
//

#include "vault_qmp2.h"
#include <mpi.h>

using namespace std;

namespace libqqc {

    bool Vault_qmp2 :: check_data_validity() {

        if (mnocc == 0) throw invalid_argument(
                "Number of occupied orbitals shouldn't be 0.");
        if (mnnmo == 0) throw invalid_argument(
                "Number of molecular orbitals shouldn't be 0.");
        if (mnao == 0) throw invalid_argument(
                "Number of atomic orbitals shouldn't be 0.");
        if (!m1Dgrid.check_data_validity()) throw invalid_argument(
                "1D Grid data not valid.");
        if (!m3Dgrid.check_data_validity()) throw invalid_argument(
                "3D Grid data not valid.");
        if (!mmat_fock) throw invalid_argument(
                "Fock matrix pointer cannot be NULL.");
        if (!mmat_coeff) throw invalid_argument(
                "Coeff matrix pointer cannot be NULL.");
        if (!mmat_cgto) throw invalid_argument(
                "Cgto matrix pointer cannot be NULL.");
        if (!mcube_coul) throw invalid_argument(
                "Coulomb integral matrix pointer cannot be NULL.");

        return true;
    }

    Vault_qmp2 :: Vault_qmp2 (Loader_qmp2 loader) {

        // check if loader is empty
        //TODO: do this if loader is done
        
        // loading meta information which are are the same for each node
        loader.load_nocc (mnocc);
        loader.load_nvirt (mnvirt);
        mnnmo = mnocc + mnvirt;
        loader.load_nao (mnao);

        // loading input information which are the same for each node
        loader.load_1Dtol (m1Dtol);
        loader.load_prnt_lvl (mprnt_lvl);

        // load grid object which are the same for each node
        loader.load_1Dgrid (m1Dgrid);

        loader.load_3Dgrid (m3Dgrid);

        // loading in matrices
        size_t nao2 = mnao * mnao;
        size_t npts = m3Dgrid.get_mnpts();
        mmat_fock = new double[nao2];
        loader.load_mat_fock (mmat_fock);
        mmat_coeff = new double[mnao * mnnmo];
        loader.load_mat_coeff (mmat_coeff);
        mmat_cgto = new double[npts * mnao];
        loader.load_mat_cgto (mmat_cgto);
        mcube_coul = new double[npts * nao2];
        loader.load_cube_coul (mcube_coul);

        check_data_validity();
    }


// This doubling isn't very elegant, would be better to do it with templating
// or something the like
    Vault_qmp2 :: Vault_qmp2 (Loader_qmp2_from_file loader) {

        // check if loader is empty
        //TODO: do this if loader is done
        

        // Set up MPI to devide the 3D grid
        MPI_Init(NULL, NULL); // MPI Initialize environment
        int pid, max_id; // ID for the current node, maximum id number
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); // get ID of current node
        MPI_Comm_rank(MPI_COMM_WORLD, &max_id); // get max ID 
        MPI_Status status; 

        if (pid == 0) {
        //Get the grid overall on node 0, master node
        // loading meta information for each node
        loader.load_nocc ("inputs.vec", mnocc);
        loader.load_nvirt ("inputs.vec", mnvirt);
        loader.load_nao ("inputs.vec", mnao);

        // loading input information for each node
        loader.load_1Dtol ("inputs.vec", m1Dtol);
        loader.load_prnt_lvl ("inputs.vec", mprnt_lvl);

        // load grid object used by each node
        loader.load_grid ("tpts.mat", "twts.mat", m1Dgrid);

        // Load grid object to be parted for each node
        Grid whole_Grid;
        loader.load_grid ("rpts.mat", "rwts.mat", whole_Grid);
        
        }
        else {
        //Do this on each other node
        
        }

        //Bcast the values we have
        MPI_Bcast(mnocc, 1, MPI_Count, 0, MPI_COMM_WORLD);
        MPI_Bcast(mnvirt, 1, MPI_Count, 0, MPI_COMM_WORLD);
        mnnmo = mnocc + mnvirt;
        MPI_Bcast(mnao, 1, MPI_Count, 0, MPI_COMM_WORLD);
        MPI_Bcast(m1Dtol, 1, MPI_Double, 0, MPI_COMM_WORLD);
        MPI_Bcast(mprnt_lvl, 1, MPI_Int, 0, MPI_COMM_WORLD);

        // loading in matrices
        size_t nao2 = mnao * mnao;
        size_t npts = m3Dgrid.get_mnpts();
        mmat_fock = new double[nao2];
        loader.load_mat_fock ("f.mat", mmat_fock, mnao, mnao);
        mmat_coeff = new double[mnao * mnnmo];
        loader.load_mat_coeff ("c.mat", mmat_coeff, mnao, mnnmo);
        mmat_cgto = new double[npts * mnao];
        loader.load_mat_cgto ("cgto.mat", mmat_cgto, npts, mnao);
        mcube_coul = new double[npts * nao2];
        loader.load_cube_coul ("coulomb.cube", mcube_coul, mnao, mnao, npts);

        check_data_validity();
    }

} //namespace libqqc

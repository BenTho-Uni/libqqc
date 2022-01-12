///
/// Member functions for the vaults used by Q-MP2
/// @file vault_mp2.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 11-01-2022
//

#include "vault_mp2.h"

using namespace std;

namespace libqqc {

    bool Vault_mp2 :: check_data_validity() {

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

    Vault_mp2 :: Vault_mp2 (Loader_mp2 loader) {

        // check if loader is empty
        //TODO: do this if loader is done
        
        // loading meta information
        loader.load_nocc (mnocc);
        loader.load_nvirt (mnvirt);
        mnnmo = mnocc + mnvirt;
        loader.load_nao (mnao);

        // loading input information
        loader.load_1Dtol (m1Dtol);
        loader.load_prnt_lvl (mprnt_lvl);

        // load grid object
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

    Vault_mp2 :: Vault_mp2(size_t nocc, size_t nvirt, size_t nao, double p1Dtol,
            int prnt_lvl, Grid p1Dgrid, Grid p3Dgrid, double* mat_fock, 
            double* mat_coeff, double* mat_cgto, double* cube_coul) :
        mnocc(nocc), mnvirt(nvirt), mnao(nao), m1Dtol(p1Dtol), 
        mprnt_lvl(prnt_lvl), m1Dgrid(p1Dgrid), m3Dgrid(p3Dgrid) {

            if (nocc == 0) throw invalid_argument(
                    "Number of occupied orbitals cannot be 0.");
            mnnmo = mnocc + mnvirt;

            if (nao == 0) throw invalid_argument(
                    "Number of atomiv orbitals cannot be 0.");
            if (!p1Dgrid.check_data_validity()) throw invalid_argument(
                    "1D Grid data not valid.");
            if (!p3Dgrid.check_data_validity()) throw invalid_argument(
                    "3D Grid data not valid.");
            // get number of points to be looped over, used later
            size_t npts = m3Dgrid.get_mnpts();
            if (npts != p1Dgrid.get_mnpts()) throw invalid_argument(
                    "Both grids have different number of points.");
            if (!mat_fock) throw invalid_argument(
                    "Fock matrix pointer cannot be NULL.");
            if (!mat_coeff) throw invalid_argument(
                    "Coefficient matrix pointer cannot be NULL.");
            if (!mat_cgto) throw invalid_argument(
                    "CGTO matrix pointer cannot be NULL.");
            if (!cube_coul) throw invalid_argument(
                    "Coulomb matrix pointer cannot be NULL.");

            if (m1Dtol < 0) m1Dtol *= -1; //ensuring that tolerance is positive

            //looping through the given array, copying the dat
            size_t mnao2 = mnao * mnao;

            mmat_fock = new double[mnao2];
            for (size_t i = 0; i < mnao; i++) {
                for (size_t j = 0; j < mnao; j++){
                    mmat_fock[i * mnao + j] = mat_fock[i * mnao + j];
                }
            }

            mmat_coeff = new double[mnao * mnnmo];
            for (size_t i = 0; i < nao; i++) {
                for (size_t j = 0; j < mnnmo; j++){
                    mmat_coeff[i * mnnmo + j] = mat_coeff[i * mnnmo + j];
                }
            }

            mmat_cgto = new double[npts * mnao];
            for (size_t i = 0; i < npts; i++) {
                for (size_t j = 0; j < mnao; j++){
                    mmat_cgto[i * mnao + j] = mat_cgto[i * mnao + j];
                }
            }

            mcube_coul = new double[npts * mnao2];
            for (size_t i = 0; i < npts; i++) {
                for (size_t j = 0; j < mnao; j++){
                    for (size_t k = 0; k < mnao; k++){
                        mcube_coul[i * mnao2 + j * mnao + k] = 
                            cube_coul[i * mnao2 + j * mnao + k];
                    }
                }
            } 

            check_data_validity();
        }

} //namespace libqqc

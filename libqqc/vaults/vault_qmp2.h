#ifndef LIBQQC_VAULT_QMP2_H
#define LIBQQC_VAULT_QMP2_H

#include "../grids/grid.h"

//Additional libraries
#include <stdexcept>
#include <iostream>

#include "../loader/loader_qmp2.h"
#include "../loader/loader_qmp2_from_file.h"

#if QCHEM
#include "../loader/loader_qmp2_from_qchem.h"
#endif

using namespace std; 

namespace libqqc {

    ///
    /// @brief Holder class for data used by Q-QMP2 method
    ///
    /// @details This class is used to hold all neccessary data used by the 
    /// Q-QMP2 method. 
    ///

    class Vault_qmp2 {
        private:

            // Meta information
            size_t mnocc = 0; ///< Number of occupied orbitals
            size_t mnvirt = 0; ///< Number of virtual orbitals
            size_t mnmo = 0; ///< Number of molecular orbitals
            size_t mnao = 0; ///< Number of atomic orbitals

            // Input informations
            int mprnt_lvl = 0; ///< Selected print level

            // Grid objects
            Grid m1Dgrid; ///< 1D grid object holding the points and weights
            Grid m3Dgrid; ///< 3D grid object holding the points and weights

            // Matrices 
            double* mmat_fock = NULL; ///< Fock matrix in AO $F_{\nu \mu}$
            double* mmat_cgto = NULL; ///< Contracted gaussian type AO evaluated at grid point $\phi_\nu (r_p$)
            double* mcube_coul = NULL; ///< Coulomb type integral $u_{\nu \mu}$ in AO 

        public: 

            ///
            /// @brief Checks if vault object data is vaild
            ///
            /// @details This function checks if the data which the vault holds
            /// is valid, e.g. not empty and returns TRUE if so, or throws an 
            /// exception if a problem is found 
            ///
            /// @return [return]
            ///
            bool check_data_validity() {

                if (mnocc == 0) throw invalid_argument(
                        "Number of occupied orbitals shouldn't be 0.");
                if (mnmo == 0) throw invalid_argument(
                        "Number of molecular orbitals shouldn't be 0.");
                if (mnao == 0) throw invalid_argument(
                        "Number of atomic orbitals shouldn't be 0.");
                if (!m1Dgrid.check_data_validity()) throw invalid_argument(
                        "1D Grid data not valid.");
                if (!m3Dgrid.check_data_validity()) throw invalid_argument(
                        "3D Grid data not valid.");
                if (!mmat_fock) throw invalid_argument(
                        "Fock matrix pointer cannot be NULL.");
                if (!mmat_cgto) throw invalid_argument(
                        "Cgto matrix pointer cannot be NULL.");
                if (!mcube_coul) throw invalid_argument(
                        "Coulomb integral matrix pointer cannot be NULL.");

                return true;

            };

            ///
            /// @brief Default constructor of vault class with loader object
            ///
            /// @details Constructor of vault class, setting the variables
            /// through the loader given in its argument
            ///
            /// @param[in] loader template for Loader for the qmp2 calculation
            ///
            Vault_qmp2(Loader_qmp2 loader) {

                // check if loader is empty
                // loading meta information
                loader.load_nocc (mnocc);
                loader.load_nvirt (mnvirt);
                mnmo = mnocc + mnvirt;
                loader.load_nao (mnao);

                // loading input information
                loader.load_prnt_lvl (mprnt_lvl);

                // load grid object
                loader.load_1Dgrid (m1Dgrid);
                loader.load_3Dgrid (m3Dgrid);

                // loading in matrices
                size_t nao2 = mnao * mnao;
                size_t npts = m3Dgrid.get_mnpts();
                mmat_fock = new double[mnmo * mnmo];
                loader.load_mat_fock (mmat_fock);
                mmat_cgto = new double[npts * mnmo];
                loader.load_mat_cgto (mmat_cgto);
                mcube_coul = new double[npts * mnocc * mnvirt];
                loader.load_cube_coul (mcube_coul);

                check_data_validity();
            }; 

            ///
            /// @brief Default constructor of vault class with loader object
            ///
            /// @details Constructor of vault class, setting the variables
            /// through the loader given in its argument
            ///
            /// @param[in] loader template for Loader for the qmp2 calculation
            ///
            Vault_qmp2(Loader_qmp2_from_file loader) {

                // check if loader is empty
                //TODO: do this if loader is done

                // loading meta information
                loader.load_nocc (mnocc);
                loader.load_nvirt (mnvirt);
                mnmo = mnocc + mnvirt;
                loader.load_nao (mnao);

                // loading input information
                loader.load_prnt_lvl (mprnt_lvl);

                // load grid object
                loader.load_1Dgrid (m1Dgrid);
                loader.load_3Dgrid (m3Dgrid);

                // loading in matrices
                size_t nao2 = mnao * mnao;
                size_t npts = m3Dgrid.get_mnpts();
                mmat_fock = new double[mnmo * mnmo];
                loader.load_mat_fock (mmat_fock);
                mmat_cgto = new double[npts * mnmo];
                loader.load_mat_cgto (mmat_cgto);
                mcube_coul = new double[npts * mnocc * mnvirt];
                loader.load_cube_coul (mcube_coul);

                check_data_validity();
            }
            ; 
#if QCHEM
            ///
            /// @brief Constructor of vault class with loader object
            ///
            /// @details Constructor of vault class, setting the variables
            /// through the loader given in its argument, uses in qchem
            ///
            /// @param[in] loader template for Loader for the qmp2 calculation
            ///
            Vault_qmp2(Loader_qmp2_from_qchem loader) {

                // check if loader is empty
                //TODO: do this if loader is done

                // loading meta information
                loader.load_nocc (mnocc);
                loader.load_nvirt (mnvirt);
                mnmo = mnocc + mnvirt;
                loader.load_nao (mnao);

                // loading input information
                loader.load_prnt_lvl (mprnt_lvl);

                // load grid object
                loader.load_1Dgrid (m1Dgrid);
                loader.load_3Dgrid (m3Dgrid);

                // loading in matrices
                size_t nao2 = mnao * mnao;
                size_t npts = m3Dgrid.get_mnpts();
                mmat_fock = new double[mnmo * mnmo];
                loader.load_mat_fock (mmat_fock);
                mmat_cgto = new double[npts * mnmo];
                loader.load_mat_cgto (mmat_cgto);
                mcube_coul = new double[npts * mnocc * mnvirt];
                loader.load_cube_coul (mcube_coul);

                check_data_validity();
            }; 
#endif

            ///
            /// @brief Destructor of vault class
            ///
            /// @details Destructor of vault class, deleting arrays 
            ///
            ~Vault_qmp2() {
                delete[] mmat_fock;
                delete[] mmat_cgto;
                delete[] mcube_coul;
            };

            ///
            /// @brief Get the number of occupied orbitals 
            ///
            /// @return mnocc number of occupied orbitals
            ///
            size_t get_mnocc() {
                if (mnocc == 0) throw invalid_argument(
                        "Number of occupied orbitals shouldn't be 0.");
                return mnocc;
            };

            ///
            /// @brief Get the number of virtual orbitals 
            ///
            /// @return mnvirt number of virtual orbitals
            ///
            size_t get_mnvirt() {
                return mnvirt;
            };

            ///
            /// @brief Get the number of molecular orbitals 
            ///
            /// @return mnmo number of molecular orbitals
            ///
            size_t get_mnmo() {
                if (mnmo == 0) throw invalid_argument(
                        "Number of molecular orbitals shouldn't be 0.");
                return mnmo;
            };

            ///
            /// @brief Get the number of atomic orbitals 
            ///
            /// @return mnao number of atomic orbitals
            ///
            size_t get_mnao() {
                if (mnao == 0) throw invalid_argument(
                        "Number of atomic orbitals shouldn't be 0.");
                return mnao;
            };

            ///
            /// @brief Get the printing level of the program
            ///
            /// @return mprnt_lvl printing level of program
            ///
            size_t get_mprnt_lvl() {
                return mprnt_lvl;
            };

            ///
            /// @brief Get the 1D grid as a reference
            ///
            /// @return pass the 1D grid as a reference
            ///
            Grid& get_m1Dgrid() {
                if (!m1Dgrid.check_data_validity()) throw invalid_argument(
                        "1D Grid data not valid.");
                return m1Dgrid;
            };

            ///
            /// @brief Get the 3D grid as a reference
            ///
            /// @return pass the 3D grid as a reference
            ///
            Grid& get_m3Dgrid() {
                if (!m3Dgrid.check_data_validity()) throw invalid_argument(
                        "3D Grid data not valid.");
                return m3Dgrid;
            };

            ///
            /// @brief Get the pointer to Fock matrix
            ///
            /// @return pass the pointer to Fock matrix
            ///
            double* get_mmat_fock() {
                if (!mmat_fock) throw invalid_argument(
                        "Fock matrix pointer cannot be NULL.");
                return mmat_fock;
            };

            ///
            /// @brief Get the pointer to CGTO matrix
            ///
            /// @return pass the pointer to CGTO matrix
            ///
            double* get_mmat_cgto() {
                if (!mmat_cgto) throw invalid_argument(
                        "Cgto matrix pointer cannot be NULL.");
                return mmat_cgto;
            };

            ///
            /// @brief Get the pointer to coulomb integral tensor
            ///
            /// @return pass the pointer to coulomb integral tensor
            ///
            double* get_mcube_coul() {
                if (!mcube_coul) throw invalid_argument(
                        "Coulomb integral matrix pointer cannot be NULL.");
                return mcube_coul;
            };
    }; // class Vault_qmp2

} // namespace libqqc

#endif //LIBQQC_VAULT_QMP2_H

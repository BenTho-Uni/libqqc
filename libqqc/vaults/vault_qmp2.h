#ifndef LIBQQC_VAULT_QMP2_H
#define LIBQQC_VAULT_QMP2_H

#include "../grids/grid.h"
#include "../loader/loader_qmp2.h"

//Additional libraries
#include <stdexcept>
#include <iostream>

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
            size_t mnnmo = 0; ///< Number of molecular orbitals
            size_t mnao = 0; ///< Number of atomic orbitals

            // Input informations
            double m1Dtol = 0; ///< Tolerance of 1D grid
            int mprnt_lvl = 0; ///< Selected print level

            // Grid objects
            Grid m1Dgrid; ///< 1D grid object holding the points and weights
            Grid m3Dgrid; ///< 3D grid object holding the points and weights

            // Matrices 
            double* mmat_fock = NULL; ///< Fock matrix in AO $F_{\nu \mu}$
            double* mmat_coeff = NULL; ///< Coeffivient matrix $C_{\nu p}$
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
            bool check_data_validity();

            ///
            /// @brief Default constructor of vault class with loader object
            ///
            /// @details Constructor of vault class, setting the variables
            /// through the loader given in its argument
            ///
            Vault_qmp2(Loader_qmp2 loader); 

            ///
            /// @brief Constructor of vault class
            ///
            /// @details Constructor of vault class, setting and checking the 
            /// needed variables
            ///
            /// @param[in] nocc Number of occupied orbitals
            /// @param[in] nvirt Number of virtual orbitals
            /// @param[in] nao Number of atomic orbitals
            /// @param[in] p1Dtol Tolerance of 1D grid
            /// @param[in] prnt_lvl Print level of the program
            /// @param[in] p1Dgrid Holder object for 1D grid
            /// @param[in] p3Dgrid Holder object for 3d grid
            /// @param[in,out] mat_fock pointer to Fock matrix to be copied
            /// @param[in,out] mat_coeff pointer to coefficient matrix to be copied
            /// @param[in,out] mat_cgto pointer to CGTO matrix to be copied
            /// @param[in,out] mat_coul pointer to coulomb integral matrix to be copied
            ///
            Vault_qmp2(size_t nocc, size_t nvirt, size_t nao, double p1Dtol, 
                    int prnt_lvl, Grid p1Dgrid, Grid p3Dgrid, double* mat_fock, 
                    double* mat_coeff, double* mat_cgto, double* mat_coul); 

            ///
            /// @brief Destructor of vault class
            ///
            /// @details Destructor of vault class, deleting arrays 
            ///
            ~Vault_qmp2() {
                delete[] mmat_fock;
                delete[] mmat_coeff;
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
            /// @return mnnmo number of molecular orbitals
            ///
            size_t get_mnnmo() {
                if (mnnmo == 0) throw invalid_argument(
                        "Number of molecular orbitals shouldn't be 0.");
                return mnnmo;
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
            /// @brief Get the tolerance of the 1D grid 
            ///
            /// @return m1Dtol tolerance of 1D grid
            ///
            size_t get_m1Dtol() {
                return m1Dtol;
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
            /// @brief Get the pointer to Coeff matrix
            ///
            /// @return pass the pointer to Coeff matrix
            ///
            double* get_mmat_coeff() {
            if (!mmat_coeff) throw invalid_argument(
                    "Coeff matrix pointer cannot be NULL.");
                return mmat_coeff;
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

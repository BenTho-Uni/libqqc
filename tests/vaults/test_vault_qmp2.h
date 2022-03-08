#ifndef LIBQQC_TEST_VAULT_QMP2_H
#define LIBQQC_TEST_VAULT_QMP2_H

//include needed libraries
#include "../../libqqc/grids/grid.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace libqqc {

    ///
    /// @brief Holding class for testing Vault_mp2 functions
    ///
    /// @details This class holds the setup for testing the 
    /// Vault_mp2 functions
    ///

    class Test_Vault_qmp2 {
        private: 
            size_t mnocc = 1; ///< Number of occupied orbitals
            size_t mnvirt = 2; ///< Number of virtual orbitals
            size_t mnnmo = mnocc + mnvirt; ///< Number of molecular orbitals
            size_t mnao = 3; ///< Number of atomic orbitals
            double m1Dtol = 0; ///< Tolerance of 1D quadrature
            int mprnt_lvl = 0; ///< Print level of the program

            Grid m1Dgrid; ///< one dimensional quadrature grid points and weights
            Grid m3Dgrid; ///< three dimensional quadrature grid points and weights

            double mmat_fock[3 * 3] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 
                7.0, 8.0, 9.0}; ///< dummy Fock matrix
            double mmat_coeff[3 * 3] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 
                7.0, 8.0, 9.0}; ///< dummy coefficient matrix
            double mmat_cgto[2 * 3] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}; ///< dummy cgto matrix
            double mcube_coul[2 * 1  * 2] = {1.0, 2.0, 3.0, 4.0}; ///< dummy coulomb tensor

            ///
            /// @brief method for checking both constructors
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_Vault_qmp2();

            ///
            /// @brief method for testing check_data_validity
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_check_data_validity();

            ///
            /// @brief method for testing get_mnocc
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mnocc();

            ///
            /// @brief method for testing get_mnvirt
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mnvirt();

            ///
            /// @brief method for testing get_mnnmo
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mnnmo();

            ///
            /// @brief method for testing get_mnao
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mnao();

            ///
            /// @brief method for get_m1Dtol
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_m1Dtol();

            ///
            /// @brief method for testing get_mprnt_lvl
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mprnt_lvl();

            ///
            /// @brief method for testing get_m1Dgrid
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_m1Dgrid();

            ///
            /// @brief method for testing get_m3Dgrid
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_m3Dgrid();

            ///
            /// @brief method for testing get_mmat_fock
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mmat_fock();

            ///
            /// @brief method for testing get_mmat_coeff
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mmat_coeff();

            ///
            /// @brief method for testing get_mmat_cgto
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mmat_cgto();

            ///
            /// @brief method for testing get_mcube_coul
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mcube_coul();

        public:

            ///
            /// @brief constructor of the testing class
            ///
            Test_Vault_qmp2() {
                size_t npts = 2;
                size_t ndim = 2;
                double pts[2 * 2] = {1.0, 1.5, 2.0, 2.5};
                double wts[2 * 2] = {1.0, 2.0};

                m1Dgrid.set_grid(npts, ndim, pts, wts);
                m3Dgrid.set_grid(npts, ndim, pts, wts);

            };

            ///
            /// @brief method for running all tests
            ///
            /// @details This method runs all tests and returns
            /// TRUE or FALSE if all tests were successfull
            ///
            /// @param[in/out] stringstream for output of test results
            /// @return bool result of comparison to reference
            ///
            bool run_all_tests(ostringstream &out);
    };


} // namespace

#endif //LIBQQC_TEST_VAULT_MP2_H

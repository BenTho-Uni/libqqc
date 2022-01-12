#ifndef LIBQQC_TEST_LOADER_MP2_H
#define LIBQQC_TEST_LOADER_MP2_H

//include needed libraries
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace libqqc {

    ///
    /// @brief Holding class for testing Loader_mp2 functions
    ///
    /// @details This class holds the setup for testing the 
    /// Loader_mp2 functions
    ///

    class Test_Loader_mp2 {
        private: 

            ///
            /// @brief method for testing load_nocc
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_nocc();

            ///
            /// @brief method for testing load_nvirt
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_nvirt();

            ///
            /// @brief method for testing load_nao
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_nao();

            ///
            /// @brief method for testing load_1Dtol
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_1Dtol();

            ///
            /// @brief method for testing load_prnt_lvl
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_prnt_lvl();

            ///
            /// @brief method for testing load_1Dgrid
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_1Dgrid();

            ///
            /// @brief method for testing load_3Dgrid
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_3Dgrid();

            ///
            /// @brief method for testing load_mat_fock
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_mat_fock();

            ///
            /// @brief method for testing load_mat_coeff
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_mat_coeff();

            ///
            /// @brief method for testing load_mat_cgto
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_mat_cgto();

            ///
            /// @brief method for testing load_cube_coul
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_cube_coul();

        public:

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

#endif //LIBQQC_TEST_LOADER_MP2_H

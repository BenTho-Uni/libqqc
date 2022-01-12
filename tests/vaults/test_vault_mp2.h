#ifndef LIBQQC_TEST_VAULT_MP2_H
#define LIBQQC_TEST_VAULT_MP2_H

//include needed libraries
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

    class Test_Vault_mp2 {
        private: 

            ///
            /// @brief method for testing function
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_function();

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

#endif //LIBQQC_TEST_VAULT_MP2_H

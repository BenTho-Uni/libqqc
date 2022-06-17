#ifndef LIBQQC_TEST_LOAD_FROM_FILE_H
#define LIBQQC_TEST_LOAD_FROM_FILE_H

//include needed libraries
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace libqqc {

    ///
    /// @brief Holding class for testing utils/load_from_file.h functions
    ///
    /// @details This class holds the setup for testing the 
    /// Load_from_file functions
    ///

    class Test_load_from_file {
        private: 

            ///
            /// @brief method for testing function load_dim_from_file
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_dim_from_file();

            ///
            /// @brief method for testing function load_array_from_file
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_load_array_from_file();

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

#endif //LIBQQC_TEST_LOAD_FROM_FILE_H

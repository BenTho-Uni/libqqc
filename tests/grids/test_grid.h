#ifndef LIBQQC_TEST_GRID_H
#define LIBQQC_TEST_GRID_H

//include needed libraries
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace libqqc {

    ///
    /// @brief Holding class for testing Grid functions
    ///
    /// @details This class holds the setup for testing the 
    /// Grid functions
    ///

    class Test_Grid {

        size_t npts = 5; ///< Number of points
        size_t ndim = 2; ///< Number of dimensions
        double pts[10] = {1.0, 2.1, 3.2, 4.3, 5.4, 6.5, 
            7.6, 8.7, 9.8, 10.9}; ///< Array of points
        double wts[5] = {0.1, 0.15, 0.2, 0.15, 0.1}; ///<  Array of Weights

        private: 

        ///
        /// @brief method for testing set_grid(..)
        ///
        /// @details This method tests the function and returns TRUE or 
        /// false depending on correct behaviour
        ///
        /// @return bool result of comparison to reference
        ///
        bool test_set_grid();

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
        /// @brief method for testing get_mnpts
        ///
        /// @details This method tests the function and returns TRUE or 
        /// false depending on correct behaviour
        ///
        /// @return bool result of comparison to reference
        ///
        bool test_get_mnpts();

        ///
        /// @brief method for testing get_mndim
        ///
        /// @details This method tests the function and returns TRUE or 
        /// false depending on correct behaviour
        ///
        /// @return bool result of comparison to reference
        ///
        bool test_get_mndim();

        ///
        /// @brief method for testing get_mpts
        ///
        /// @details This method tests the function and returns TRUE or 
        /// false depending on correct behaviour
        ///
        /// @return bool result of comparison to reference
        ///
        bool test_get_mpts();

        ///
        /// @brief method for testing get_mwts
        ///
        /// @details This method tests the function and returns TRUE or 
        /// false depending on correct behaviour
        ///
        /// @return bool result of comparison to reference
        ///
        bool test_get_mwts();

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

#endif //LIBQQC_TEST_GRID_H

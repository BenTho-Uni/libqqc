#ifndef LIBQQC_TEST_TTIMER_H
#define LIBQQC_TEST_TTIMER_H

//include needed libraries
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

namespace libqqc {

    ///
    /// @brief Holding class for testing Tclock functions
    ///
    /// @details This class holds the setup for testing the 
    /// Tclock functions
    ///

    class Test_Tclock {
        private: 
            ///
            /// @brief method for testing get_mrun
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_mrun();

            ///
            /// @brief method for testing stop_clock
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_stop_clock();

            ///
            /// @brief method for testing start_clock
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_start_clock();

            ///
            /// @brief method for testing wall_time
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_wall_time();

            ///
            /// @brief method for testing cpu_time
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_cpu_time();

            ///
            /// @brief method for testing print_time
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_print_time();

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

    ///
    /// @brief Holding class for testing Ttimer functions
    ///
    /// @details This class holds the setup for testing the 
    /// Ttimer functions
    ///

    class Test_Ttimer {
        private: 
            ///
            /// @brief method for testing get_clocks
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_get_clocks();

            ///
            /// @brief method for testing start_new_clock
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_start_new_clock();

            ///
            /// @brief method for testing stop_clock
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_stop_clock();

            ///
            /// @brief method for testing stop_all_clocks
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_stop_all_clocks();

            ///
            /// @brief method for testing print_clocks
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_print_clocks();

            ///
            /// @brief method for testing print_all_clocks
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_print_all_clocks();

        public: 
            ///
            /// @brief method for running all tests
            ///
            /// @details This method runs all tests and returns
            /// TRUE if all tests were successfull
            ///
            /// @param[in/out] stringstream for output of test results
            /// @return bool result of comparison to reference
            ///
            bool run_all_tests(ostringstream &out);
    };

} // namespace

#endif //LIBQQC_TEST_TTIMER_H

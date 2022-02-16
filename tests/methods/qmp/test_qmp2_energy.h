#ifndef LIBQQC_TEST_QMP2_ENERGY_H
#define LIBQQC_TEST_QMP2_ENERGY_H

//include needed libraries
#include <iostream>
#include <sstream>
#include <string>

#if LIBQQC_WITH_EIGEN
    #include "../../../libqqc/methods/qmp/qmp2_energy_eigen.h"
#else
    #include "../../../libqqc/methods/qmp/qmp2_energy.h"
#endif

using namespace std;

namespace libqqc {

    ///
    /// @brief Holding class for testing Qmp2_energy functions
    ///
    /// @details This class holds the setup for testing the 
    /// Qmp2_energy functions
    ///

    class Test_Qmp2_energy {
        private: 

            ///
            /// @brief method for testing the compute() function
            ///
            /// @details This method tests the function and returns TRUE or 
            /// false depending on correct behaviour
            ///
            /// @return bool result of comparison to reference
            ///
            bool test_compute();

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

#endif //LIBQQC_TEST_QMP2_ENERGY_H

#ifndef LIBQQC_TEST_TTIMER_H
#define LIBQQC_TEST_TTIMER_H

namespace libqqc {

    ///
    /// @brief Holding class for testing Ttimer functions
    ///
    /// @details This class holds the setup for testing the 
    /// Ttimer functions
    ///

    class Test_Ttimer {
        public: 
            ///
            /// @brief method for running the test
            ///
            /// @details This method runs the function, 
            /// tests the output and returns TRUE or FALSE
            /// depending on outcome. 
            ///
            /// @return bool result of comparison to reference
            ///
            bool run_test();
    };

} // namespace

#endif //LIBQQC_TEST_TTIMER_H

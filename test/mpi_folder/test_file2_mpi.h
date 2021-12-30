#ifndef LIBQQC_TEST_FILE2_MPI_H
#define LIBQQC_TEST_FILE2_MPI_H

namespace libqqc {

///
/// @brief Holding class for testing test_file2_mpi functions
///
/// @details This class holds the setup for testing the 
/// test_file2_mpi functions
///

class test_file2_mpi {
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

#endif //LIBQQC_TEST_FILE2_MPI_H

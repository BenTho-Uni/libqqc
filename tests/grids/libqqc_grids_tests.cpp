
//include needed libraries
#include <iostream>
#include <string>
#include <sstream>

// includes of the test headers
#include "test_grid.h"

#if LIBQQC_WITH_EIGEN
//#include "eigen_folder/test_file3_eigen.h"
#endif

#if LIBQQC_WITH_MPI
//#include "mpi_folder/test_file2_mpi.h"
#endif

// namespaces

using namespace libqqc;
using namespace std;

int main (){
    ostringstream out; //Stringstream for testing outputs

    cout << "Performing tests for libqqc/testfolder ..." << endl;

    Test_Grid grid;
    bool b_grid = grid.run_all_tests(out);
    cout << endl << "Testing grids/grid.h, Grid::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_grid) ? "all passed" : "some failed") << endl;

#if LIBQQC_WITH_EIGEN
//    test_file3_eigen file3;
//    cout << "Testing eigen_folder/file3_eigen, function_eigen ... " << 
//        flush << ((file3.run_test()) ? "passed" : "failed") << endl;
#endif

#if LIBQQC_WITH_MPI
//    test_file2_mpi file2;
//    cout << "Testing mpi_folder/file2_mpi, function_mpi ... " << flush << 
//        ((file2.run_test()) ? "passed" : "failed") << endl;
#endif

    return ((b_grid) ? 0 : 1);
}


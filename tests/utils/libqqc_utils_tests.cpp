
//include needed libraries
#include <iostream>
#include <string>
#include <sstream>

// includes of the test headers
#include "test_ttimer.h"

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

    cout << "Performing tests for libqqc/utils ..." << endl;

    Test_Tclock clock;
    bool b_clock = clock.run_all_tests(out);
    cout << endl << "Testing utils/ttimer.h, Tclock::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_clock) ? "all passed" : "some failed") << endl;

    Test_Ttimer timer;
    bool b_timer = timer.run_all_tests(out);
    cout << endl << "Testing utils/ttimer.h, Ttimer::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_timer) ? "all passed" : "some failed") << endl;

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

//    return ((b_clock && b_timer) ? 0 : 1);
    return 1;
}


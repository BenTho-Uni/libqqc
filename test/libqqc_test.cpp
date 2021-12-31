
//include needed libraries
#include <iostream>

// includes of the test headers
#include "folder/test_file1.h"

#if LIBQQC_WITH_EIGEN
#include "eigen_folder/test_file3_eigen.h"
#endif

#if LIBQQC_WITH_MPI
#include "mpi_folder/test_file2_mpi.h"
#endif

// namespaces

using namespace libqqc;
using namespace std;

int main (){
        cout << "Performing tests for libqqc..." << endl;

        test_file1 file1;
        cout << "Testing folder/file1, function1 ... " << flush << 
                ((file1.run_test()) ? "passed" : "failed") << endl;

#if LIBQQC_WITH_EIGEN
        test_file3_eigen file3;
        cout << "Testing eigen_folder/file3_eigen, function_eigen ... " << 
            flush << ((file3.run_test()) ? "passed" : "failed") << endl;
#endif

#if LIBQQC_WITH_MPI
        test_file2_mpi file2;
        cout << "Testing mpi_folder/file2_mpi, function_mpi ... " << flush << 
                ((file2.run_test()) ? "passed" : "failed") << endl;
#endif

        return 0;
}


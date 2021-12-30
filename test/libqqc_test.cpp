
//include needed libraries
#include <iostream>

// includes of the test headers
#include "folder/test_file1.h"
#include "mpi_folder/test_file2_mpi.h"

// namespaces

using namespace libqqc;
using namespace std;

int main (){
        cout << "Performing tests for libqqc..." << endl;

        test_file1 file1;
        cout << "Testing folder/file1, function1 ... " << flush << 
                ((file1.run_test()) ? "passed" : "failed") << endl;
#if LIBQQC_WITH_MPI
        test_file2_mpi file2;
        cout << "Testing mpi_folder/file2_mpi, function_mpi ... " << flush << 
                ((file2.run_test()) ? "passed" : "failed") << endl;
#endif
        return 0;
}


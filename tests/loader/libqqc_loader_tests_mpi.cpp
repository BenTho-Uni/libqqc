
//include needed libraries
#include <iostream>
#include <string>
#include <sstream>
#include <mpi.h>

// includes of the test headers
#include "test_loader_qmp2.h"
#include "test_loader_qmp2_from_file.h"

#if LIBQQC_WITH_EIGEN
//#include "eigen_folder/test_file3_eigen.h"
#endif

#if LIBQQC_WITH_MPI
//#include "mpi_folder/test_file2_mpi.h"
#endif

// namespaces

using namespace libqqc;
using namespace std;

int main (int argc, char *argv[]){
    ostringstream out; //Stringstream for testing outputs

#ifdef _OPENMP
    // To savely use MPI with OpenMP we need to check if the supported
    // threading is provided by the system
    // MPI_THREAD_FUNNELD allows multithreaded MPI if only the master calls
    // MPI function which works for us
    int provided = 0; // check threading
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    if (provided < MPI_THREAD_FUNNELED){
        printf("The supported threading is not sufficient.\
                (less than MPI_THREAD_FUNNELED)\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
#else
    MPI_Init(&argc, &argv);
#endif


    cout << "Performing tests for libqqc/loader ..." << endl;

    Test_Loader_qmp2 loader_qmp2;
    bool b_loader_qmp2 = loader_qmp2.run_all_tests(out);
    cout << endl << "Testing loader/loader_qmp2.h, loader_qmp2::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_loader_qmp2) ? "all passed" : "some failed") << endl;

    Test_Loader_qmp2_from_file loader_qmp2_from_file;
    bool b_loader_qmp2_from_file = loader_qmp2_from_file.run_all_tests(out);
    cout << endl << "Testing loader/loader_qmp2_from_file.h, loader_qmp2_from_file::" 
        << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_loader_qmp2_from_file) ? "all passed" : "some failed") << endl;

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

    MPI_Finalize();
    return (!(b_loader_qmp2 && b_loader_qmp2_from_file));

}


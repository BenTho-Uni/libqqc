
//include needed libraries
#include <iostream>
#include <string>
#include <sstream>

// includes of the test headers
#include "folder/test_file1.h"
#include "loader/test_loader_mp2.h"
#include "grids/test_grid.h"
#include "methods/qmp/test_qmp2_energy.h"
#include "utils/test_ttimer.h"
#include "vaults/test_vault_mp2.h"

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
    ostringstream out; //Stringstream for testing outputs

    cout << "Performing tests for libqqc..." << endl;

    test_file1 file1;
    cout << "Testing folder/file1, function1 ... " << flush << 
        ((file1.run_test()) ? "passed" : "failed") << endl;

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

    Test_Loader_mp2 loader_mp2;
    bool b_loader_mp2 = loader_mp2.run_all_tests(out);
    cout << endl << "Testing loader/loader_mp2.h, Loader_mp2::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_loader_mp2) ? "all passed" : "some failed") << endl;

    Test_Grid grid;
    bool b_grid = grid.run_all_tests(out);
    cout << endl << "Testing grids/grid.h, Grid::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_grid) ? "all passed" : "some failed") << endl;

    Test_Qmp2_energy qmp2_energy;
    bool b_qmp2_energy = qmp2_energy.run_all_tests(out);
    cout << endl << "Testing methods/qmp2/qmp2_energy.h, Qmp2_energy::" 
        << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_qmp2_energy) ? "all passed" : "some failed") << endl;

    Test_Vault_mp2 vault_mp2;
    bool b_vault_mp2 = vault_mp2.run_all_tests(out);
    cout << endl << "Testing vaults/vault_mp2.h, Vault_mp2::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_vault_mp2) ? "all passed" : "some failed") << endl;

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

    return ((b_clock && b_timer && b_loader_mp2 && b_grid && b_qmp2_energy
                && b_vault_mp2) ? 0 : 1);
}

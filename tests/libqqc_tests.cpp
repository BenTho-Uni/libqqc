
//include needed libraries
#include <iostream>
#include <string>
#include <sstream>

// includes of the test headers
#include "loader/test_loader_qmp2.h"
#include "loader/test_loader_qmp2_from_file.h"
#include "grids/test_grid.h"
#include "methods/qmp/test_qmp2_energy.h"
#include "utils/test_ttimer.h"
#include "utils/test_load_from_file.h"
#include "vaults/test_vault_qmp2.h"

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

    cout << "Performing tests for libqqc..." << endl;

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

    Test_load_from_file load_from_file;
    bool b_load_from_file = load_from_file.run_all_tests(out);
    cout << endl << "Testing utils/load_from_file.h, " << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_load_from_file) ? "all passed" : "some failed") << endl;

    Test_Loader_qmp2 loader_qmp2;
    bool b_loader_qmp2 = loader_qmp2.run_all_tests(out);
    cout << endl << "Testing loader/loader_qmp2.h, Loader_qmp2::" << flush << endl;
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

    Test_Vault_qmp2 vault_qmp2;
    bool b_vault_qmp2 = vault_qmp2.run_all_tests(out);
    cout << endl << "Testing vaults/vault_qmp2.h, Vault_qmp2::" << flush << endl;
    cout << out.str();
    cout << "... " 
        << ((b_vault_qmp2) ? "all passed" : "some failed") << endl;

/* #if LIBQQC_WITH_EIGEN */
/*     test_file3_eigen file3; */
/*     cout << "Testing eigen_folder/file3_eigen, function_eigen ... " << */ 
/*         flush << ((file3.run_test()) ? "passed" : "failed") << endl; */
/* #endif */

/* #if LIBQQC_WITH_MPI */
/*     test_file2_mpi file2; */
/*     cout << "Testing mpi_folder/file2_mpi, function_mpi ... " << flush << */ 
/*         ((file2.run_test()) ? "passed" : "failed") << endl; */
/* #endif */

    return ((b_clock && b_timer && b_load_from_file && b_loader_qmp2 
               && b_loader_qmp2_from_file 
                && b_grid && b_qmp2_energy
                && b_vault_qmp2) ? 0 : 1);
}

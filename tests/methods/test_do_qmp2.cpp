//include file header
#include "test_do_qmp2.h"


// includes of the to be tested headers
#include "../../libqqc/methods/do_qmp2.h"
#include "../../libqqc/vaults/vault_qmp2.h"
#include "../../libqqc/grids/grid.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Do_qmp2 :: test_run() {
        bool result = false;

    cout << "arrived" <<endl;
        Loader_qmp2_from_file loader("../../data/h2o/");
        Vault_qmp2 vault(loader);

        // Calling do_qmp2
        Do_qmp2 qmp2(vault);
        
        ostringstream out;
        qmp2.run(out);

        string ref =
        "\nQ-MP(2) Ground State Energy (eV): -0.124222";

        result = (out.str() == ref); 
        return result;
    }

    bool Test_Do_qmp2 :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_test_run = test_run();
        out << "    Testing Do_qmp2::run(...)         ... " << flush
            << (b_test_run ? "passed" : "failed") << endl;

        result = b_test_run;
        return result;

    } // run_test

} // namespace

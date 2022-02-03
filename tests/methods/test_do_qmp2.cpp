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

        // Setting up test data, remove this if load_ is done
        //
        size_t nocc = 1;
        size_t nvirt = 2;
        size_t nao = 3;
        double p1Dtol = 10e-3;
        size_t prnt_lvl = 0;

        size_t p1Dnpts = 3;
        double v1Dpts[3] = {1.71, 1.72, 1.73};
        double v1Dwts[3] = {0.25, 0.5, 0.25};

        size_t p3Dnpts = 2;
        double m3Dpts[6] = {1.11, 1.12, 1.13, 1.14, 1.15, 1.16};
        double m3Dwts[6] = {0.1, 0.2, 0.2, 0.2, 0.2, 0.1};

        double mat_fock[9] = {1.21, 1.22, 1.23, 1.24, 1.25, 1.26, 1.27, 1.28, 
            1.29};
        double mat_coeff[9] = {1.41, 1.42, 1.43, 1.44, 1.45, 1.46, 1.47, 1.48, 
            1.49};
        double mat_cgto[5] = {1.51, 1.52, 1.53, 1.54, 1.55};


        double c_c[18] = {1.31, 1.32, 1.33, 1.34, 1.35, 1.36, 1.37, 1.38, 1.39, 
            1.311, 1.312, 1.313, 1.314, 1.315, 1.316, 1.317, 1.318};

        // Setup grid
        Grid p1Dgrid(p1Dnpts, 1, v1Dpts, v1Dwts);
        Grid p3Dgrid(p3Dnpts, 3, m3Dpts, m3Dwts); 

        // Setup vault
        Vault_qmp2 vault(nocc, nvirt, nao, p1Dtol, prnt_lvl, p1Dgrid, p3Dgrid, 
                mat_fock, mat_coeff, mat_cgto, c_c);

        // Calling do_qmp2
        Do_qmp2 qmp2(vault);
        
        ostringstream out;
        qmp2.run(out);

        string ref ="\nQ-MP(2) Ground State Energy : -10401.3\n";

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

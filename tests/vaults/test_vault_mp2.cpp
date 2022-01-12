//include file header
#include "test_vault_mp2.h"


// includes of the to be tested headers
#include "../../libqqc/vaults/vault_mp2.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Vault_mp2 :: test_function() {

    }

    bool Test_Vault_mp2 :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_function = (test_function()) ? true : false;
        out << "    Testing rename::function()    ... " << flush
            << (b_function ? "passed" : "failed") << endl;

        result = b_function;
        return result;

    } // run_test

} // namespace

//include file header
#include "test_rename.h"


// includes of the to be tested headers
#include "../../libqqc/folder/rename.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Rename :: test_function() {
        bool result = false;
        return result;
    }

    bool Test_Rename :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_function = test_function();
        out << "    Testing rename::function()    ... " << flush
            << (b_function ? "passed" : "failed") << endl;

        result = b_function;
        return result;

    } // run_test

} // namespace

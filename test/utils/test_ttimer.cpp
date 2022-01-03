//include file header
#include "test_ttimer.h"

//include needed libraries
#include <iostream>
#include <string>

// includes of the to be tested headers
#include "../../libqqc/utils/ttimer.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Ttimer :: run_test() {

        //testing a clock
        Tclock this_clock("This is an example clock");
        this_clock.stop_clock();
        cout << this_clock.print_time() << endl;

        return true;

    } // run_test

} // namespace

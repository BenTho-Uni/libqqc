//include file header
#include "test_ttimer.h"

//include needed libraries
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// includes of the to be tested headers
#include "../../libqqc/utils/ttimer.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Ttimer :: run_test() {

        //testing a clock
        Tclock this_clock("This is an example clock");
        this_thread::sleep_for(chrono::milliseconds(100)); //sleeps for 100 ms
        this_clock.stop_clock();

        int time = chrono::duration_cast<std::chrono::milliseconds>
                    (this_clock.wall_time()).count();

        return ((time - 100) < 10) ? true : false;

    } // run_test

} // namespace

//include file header
#include "test_ttimer.h"


// includes of the to be tested headers
#include "../../libqqc/utils/ttimer.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Tclock :: test_get_mrun() {
        bool result = false;

        Tclock test ("Testing clock");
        result = (test.get_mrun()) ? true : false;

        return result;
    }

    bool Test_Tclock :: test_stop_clock() {
        bool result = false;

        Tclock test ("Testing clock");
        test.stop_clock();
        result = (!test.get_mrun()) ? true : false;

        return result;
    }

    bool Test_Tclock :: test_start_clock() {
        bool result = false;

        Tclock test ("Testing clock");
        test.stop_clock();
        test.start_clock();
        result = (test.get_mrun()) ? true : false;

        return result;
    }

    bool Test_Tclock :: test_wall_time() {
        bool result = false;

        Tclock test ("Testing clock");
        //making some time difference
        test.start_clock(); //reset clock
        this_thread::sleep_for(chrono::milliseconds(100)); //sleeps for 100 ms
        test.stop_clock();
        //calculating the time difference of walltime
        int time = chrono::duration_cast<std::chrono::milliseconds>
                    (test.wall_time()).count();

        result = ((time - 100) < 10) ? true : false;

        return result;
    }

    bool Test_Tclock :: test_cpu_time() {
        bool result = false;

        Tclock test ("Testing clock");
        //making some time difference
        this_thread::sleep_for(chrono::milliseconds(100)); //sleeps for 100 ms
        test.stop_clock();
        //calculating the time difference of cputime

        result = ((1000.0 * test.cpu_time() / CLOCKS_PER_SEC < 10) ? true : false);

        return result;
    }

    bool Test_Tclock :: test_print_time(){
        bool result = false;

        Tclock test ("Testing clock");
        //making some time difference
        this_thread::sleep_for(chrono::milliseconds(100)); //sleeps for 100 ms
        test.stop_clock();
        //testing the output string
        string reference = "  Testing clock Timer - CPU: 0 ms ; Wall: 100 ms\n";
        string test_output = test.print_time(0);
        result = (reference == test_output) ? true : false;

        return result;
    }

    bool Test_Tclock :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_mrun = (test_get_mrun()) ? true : false;
        out << "    Testing Tclock::get_mrun()    ... " << flush
            << (b_mrun ? "passed" : "failed") << endl;

        bool b_stop = (test_stop_clock()) ? true : false;
        out << "    Testing Tclock::stop_clock()  ... " << flush
            << (b_stop ? "passed" : "failed") << endl;

        bool b_start = (test_start_clock()) ? true : false;
        out << "    Testing Tclock::start_clock() ... " << flush
            << (b_start ? "passed" : "failed") << endl;

        bool b_wall = (test_wall_time()) ? true : false;
        out << "    Testing Tclock::wall_time()   ... " << flush
            << (b_wall ? "passed" : "failed") << endl;

        bool b_cpu = (test_cpu_time()) ? true : false;
        out << "    Testing Tclock::cpu_time()    ... " << flush
            << (b_cpu ? "passed" : "failed") << endl;

        bool b_print = (test_print_time()) ? true : false;
        out << "    Testing Tclock::print_time()  ... " << flush
            << (b_print ? "passed" : "failed") << endl;

        result = b_mrun && b_stop && b_start && b_cpu && b_print;
        return result;

    } // run_test

    bool Test_Ttimer :: test_get_clocks() {
        bool result = false;

        Ttimer test(2);
        test.start_new_clock("Clock 1", 0, 1);
        test.start_new_clock("Clock 2", 0, 1);
        test.start_new_clock("Clock 3", 1, 1);
        vector<Tclock> clocks = test.get_clocks(0);
        result = ((clocks.size() == 2)? true :  false);

        return result;
    }

    bool Test_Ttimer :: test_start_new_clock() {
        bool result = false;

        Ttimer test(2);
        test.start_new_clock("Clock 1", 0, 1);
        vector<Tclock> clocks = test.get_clocks(0);
        result = (clocks[0].get_mrun() ? true : false);

        return result;
    }

    bool Test_Ttimer :: test_stop_clocks() {
        bool result = false;

        Ttimer test(2);
        test.start_new_clock("Clock 1", 0, 1);
        test.start_new_clock("Clock 2", 0, 1);
        test.stop_clocks(0);

        vector<Tclock> clocks = test.get_clocks(0);
        result = ((!clocks[0].get_mrun() && !clocks[1].get_mrun()) ? true : false);

        return result;
    }

    bool Test_Ttimer :: test_stop_all_clocks() {
        bool result = false;

        Ttimer test(2);
        test.start_new_clock("Clock 1", 0, 1);
        test.start_new_clock("Clock 2", 1, 1);
        test.stop_all_clocks();

        vector<Tclock> clocks0 = test.get_clocks(0);
        vector<Tclock> clocks1 = test.get_clocks(0);
        result = ((!clocks0[0].get_mrun() && !clocks1[0].get_mrun()) ? true : false);

        return result;
    }

    bool Test_Ttimer :: test_print_clocks() {
        bool result = false;

        Ttimer test(2);
        test.start_new_clock("Clock 1", 0, 1);
        test.start_new_clock("Clock 2", 0, 1);
        test.start_new_clock("Clock 3", 1, 1);
        test.stop_clocks(0);

        string reference = "  Clock 1 Timer - CPU: 0 ms ; Wall: 0 ms\n";
        reference += "  Clock 2 Timer - CPU: 0 ms ; Wall: 0 ms\n";
        string test_string = test.print_clocks(0);

        result = ((reference == test_string) ? true : false);

        return result;
    }

    bool Test_Ttimer :: test_print_all_clocks() {
        bool result = false;

        Ttimer test(2);
        test.start_new_clock("Clock 1", 0, 1);
        test.start_new_clock("Clock 2", 0, 1);
        test.start_new_clock("Clock 3", 1, 1);
        test.stop_clocks(0);
        test.stop_clocks(1);

        string reference = "  Clock 1 Timer - CPU: 0 ms ; Wall: 0 ms\n";
        reference += "  Clock 2 Timer - CPU: 0 ms ; Wall: 0 ms\n";
        reference += "  Clock 3 Timer - CPU: 0 ms ; Wall: 0 ms\n";
        string test_string = test.print_all_clocks();

        result = ((reference == test_string) ? true : false);

        return result;
    }

    bool Test_Ttimer :: run_all_tests(ostringstream &out) {
        out.str("");
        bool result = false;

        bool b_get = (test_get_clocks()) ? true : false;
        out << "    Testing Ttimer::get_clocks()       ... " << flush
            << (b_get ? "passed" : "failed") << endl;

        bool b_start = (test_start_new_clock()) ? true : false;
        out << "    Testing Ttimer::start_new_clock()  ... " << flush
            << (b_start ? "passed" : "failed") << endl;

        bool b_stop1 = (test_stop_clocks()) ? true : false;
        out << "    Testing Ttimer::stop_clocks()      ... " << flush
            << (b_stop1 ? "passed" : "failed") << endl;

        bool b_stopA = (test_stop_all_clocks()) ? true : false;
        out << "    Testing Ttimer::stop_all_clocks()  ... " << flush
            << (b_stopA ? "passed" : "failed") << endl;

        bool b_print1 = (test_print_clocks()) ? true : false;
        out << "    Testing Ttimer::print_clocks()     ... " << flush
            << (b_print1 ? "passed" : "failed") << endl;

        bool b_printA = (test_print_all_clocks()) ? true : false;
        out << "    Testing Ttimer::print_all_clocks() ... " << flush
            << (b_printA ? "passed" : "failed") << endl;

        result = b_get && b_start && b_stop1 && b_stopA && b_print1 && b_printA;

        return result;

    } // run_test

} // namespace

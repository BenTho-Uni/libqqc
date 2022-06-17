#include <iostream>
#include <sstream>
#include "../libqqc/methods/do_qmp2.h"
#include "../libqqc/utils/ttimer.h"

using namespace std;
using namespace libqqc;

int main (){
    //Set a output stream for our data
    ostringstream out;
    //Set up an outer timer to time the different modules
    //loader, vault, method
    Ttimer timings(0);

    // The Loader provides method and host_program specific loading function
    // here we load from files on disk for the Q-MP2 method
    timings.start_new_clock("Timing Loader_qmp2_from_file:: loader :", 0, 0);
    Loader_qmp2_from_file loader("../data/h2o/");
    timings.stop_clock(0);
    cout << timings.print_clocks(0);

    // The vault calls the loader functions and saves all the necessary data
    // for this method
    timings.start_new_clock("Timing Vault_qmp2:: vault : ", 1, 0);
    Vault_qmp2 vault(loader);
    timings.stop_clock(1);
    cout << timings.print_clocks(1);

    // The vault calls the loader functions and saves all the necessary data
    // for this method
    timings.start_new_clock("Timing Do_qmp2:: qmp2 and run : " , 2, 0);
    Do_qmp2 qmp2(vault);
    // And execute it
    qmp2.run(out);
    timings.stop_clock(2);
    cout << timings.print_clocks(2);

    // Print our output to standard out
    cout << out.str() << endl;

    return 0;
}

///
/// main function called by qchem
/// @file qqc_main.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 28-04-2022


//Internal headers
//#include <qqc.h>
#include "../methods/do_qmp2.h"
#include "../utils/ttimer.h"

//Host headers
#include <qchem.h>
#include <QString.h>

//Extra headers
#include <stdio.h>
#include <iostream>
#include <sstream>

    ///
    /// @file
    /// @brief Main function called by qchem
    ///
    /// @details This function is called by qchem and prepares and executes 
    /// the Q-QC calculation
    ///
using namespace libqqc;
using namespace std;

int qqc_main(){
    try {
        //Set a output stream for our data
        ostringstream out;
        //Set up an outer timer to time the different modules
        //loader, vault, method
        Ttimer timings(0);

        // The Loader provides method and host_program specific loading function
        // here we load from Q-Chem for the Q-MP2 method
        timings.start_new_clock("Timing Loader_qmp2_from_file:: loader :", 0, 0);
        Loader_qmp2_from_qchem loader;
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
        out.str("");

    }
    catch (std::exception &e) {
        QCrashLoc(e.what(), __FILE__, __LINE__);
    }
    catch (...) {
        QCrashLoc("Unknown exception", __FILE__, __LINE__);
    }

	    return EXIT_SUCCESS;
}

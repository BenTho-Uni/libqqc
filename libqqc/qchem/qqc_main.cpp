///
/// main function called by qchem
/// @file qqc_main.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 28-04-2022


//Internal headers
//#include <qqc.h>

//Host headers
#include <qchem.h>
#include <QString.h>

//Extra headers
#include <stdio.h>

    ///
    /// @file
    /// @brief Main function called by qchem
    ///
    /// @details This function is called by qchem and prepares and executes 
    /// the Q-QC calculation
    ///

int qqc_main(){
    try {
        std::cout << "QQC reporting in!";
    }
    catch (stc::exception &e) {
        QCrashLoc(e.what(), __FILE__, __LINE__);
    }
    catch (...) {
        QCrashLoc("Unknown exception", __FILE__, __LINE__);
    }

    return EXIT_SUCCESS;
}

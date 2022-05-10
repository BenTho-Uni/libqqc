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

        size_t mnocc = 0; ///< Number of occupied orbitals
        size_t mnvirt = 0; ///< Number of virtual orbitals
        size_t mnmo = 0; ///< Number of molecular orbitals
        size_t mnao = 0; ///< Number of atomic orbitals

        // Input informations
        int mprnt_lvl = 0; ///< Selected print level

        // Grid objects
        Grid m1Dgrid; ///< 1D grid object holding the points and weights
        Grid m3Dgrid; ///< 3D grid object holding the points and weights

        // Matrices 
        double* mmat_fock = NULL; ///< Fock matrix in AO $F_{\nu \mu}$
        double* mmat_cgto = NULL; ///< Contracted gaussian type AO evaluated at grid point $\phi_\nu (r_p$)
        double* mcube_coul = NULL; ///< Coulomb type integral $u_{\nu \mu}$ in AO 

        // loading meta information
        loader.load_nocc (mnocc);
        loader.load_nvirt (mnvirt);
        mnmo = mnocc + mnvirt;
        loader.load_nao (mnao);
        cout << "nocc ist: " << mnocc << endl;
        cout << "nvirt ist: " << mnvirt << endl;
        cout << "nao ist: " << mnao << endl;

        // loading input information
        loader.load_prnt_lvl (mprnt_lvl);
        cout << "the print level is: " << mprnt_lvl << endl;

        // load grid object
        loader.load_1Dgrid (m1Dgrid);
        cout << "grid 1 done" << endl;
        loader.load_3Dgrid (m3Dgrid);

        // loading in matrices
        size_t nao2 = mnao * mnao;
        size_t npts = m3Dgrid.get_mnpts();
        mmat_fock = new double[mnmo * mnmo];
        loader.load_mat_fock (mmat_fock);
        mmat_cgto = new double[npts * mnmo];
        loader.load_mat_cgto (mmat_cgto);
        mcube_coul = new double[npts * mnocc * mnvirt];
        //loader.load_cube_coul (mcube_coul);


        cout << "First point is " << m3Dgrid.get_mpts()[0]
                << m3Dgrid.get_mpts()[1] << m3Dgrid.get_mpts()[2] << endl;

        cout << "First fock point is " << mmat_fock[0]
                << mmat_fock[1] << mmat_fock[2] << endl;
        cout << "First cgto point is " << mmat_cgto[0]
                << mmat_cgto[1] << mmat_cgto[2] << endl;

#if 0
        cout << "First coul point is " << mcube_coul[0]
                << mcube_coul[1] << mcube_coul[2] << endl;
#endif 
        delete[] mmat_fock;
        delete[] mmat_cgto;
        delete[] mcube_coul;

#if 0
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
#endif
    }
    catch (std::exception &e) {
        QCrashLoc(e.what(), __FILE__, __LINE__);
    }
    catch (...) {
        QCrashLoc("Unknown exception", __FILE__, __LINE__);
    }

    return EXIT_SUCCESS;
}

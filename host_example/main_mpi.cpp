#include <iostream>
#include <sstream>
#include "../libqqc/methods/do_qmp2.h"
#include "../libqqc/utils/ttimer.h"
#include <mpi.h>

using namespace std;
using namespace libqqc;

int main (int argc, char *argv[]){
    //Set a output stream for our data
    ostringstream out;
    //Set up an outer timer to time the different modules
    //loader, vault, method
    Ttimer timings(0);

    // Open MPI environment
#ifdef _OPENMP
    // To savely use MPI with OpenMP we need to check if the supported
    // threading is provided by the system
    // MPI_THREAD_FUNNELD allows multithreaded MPI if only the master calls
    // MPI function which works for us
    int provided = 0; // check threading
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    if (provided < MPI_THREAD_FUNNELED){
        printf("The supported threading is not sufficient.\
                (less than MPI_THREAD_FUNNELED)\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
#else
    MPI_Init(&argc, &argv);
#endif
    int id = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &id); //Get the id of the current node

    // The Loader provides method and host_program specific loading function
    // here we load from files on disk for the Q-MP2 method
    timings.start_new_clock("Timing Loader_qmp2_from_file:: loader :", 0, 0);
    Loader_qmp2_from_file loader("../data/h2o/");
    timings.stop_clock(0);
    if (id == 0) cout << timings.print_clocks(0);

    // The vault calls the loader functions and saves all the necessary data
    // for this method
    timings.start_new_clock("Timing Vault_qmp2:: vault : ", 1, 0);
    Vault_qmp2 vault(loader);
    timings.stop_clock(1);
    if (id == 0) cout << timings.print_clocks(1);

    // And finally we call the method routine to setup the calculation and
    timings.start_new_clock("Timing Do_qmp2:: qmp2 : " , 2, 0);
    Do_qmp2 qmp2(vault);

    // And execute it
    qmp2.run(out);
    timings.stop_clock(2);
    if (id == 0) cout << timings.print_clocks(2);

    // Print our output to standard out
    if (id == 0) cout << out.str() << endl;

    // CLose MPI environment
    MPI_Finalize();

    return 0;
}

#include <iostream>
#include <sstream>
#include "../libqqc/methods/do_qmp2.h"
#include "../libqqc/utils/ttimer.h"
#include <mpi.h>

using namespace std;
using namespace libqqc;

int main (){
    ostringstream out;

    MPI_Init(NULL, NULL);
    int id = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);


    Ttimer timings(0);

    timings.start_new_clock("Timing Loader_qmp2_from_file:: loader", 0, 0);
    Loader_qmp2_from_file loader("../data/h2o/");
    timings.stop_clock(0);
    if (id == 0) cout << timings.print_clocks(0);

    timings.start_new_clock("Timing Vault_qmp2:: vault : ", 1, 0);
    Vault_qmp2 vault(loader);
    timings.stop_clock(1);
    if (id == 0) cout << timings.print_clocks(1);

    timings.start_new_clock("Timing Do_qmp2:: qmp2 and run : " , 2, 0);
    Do_qmp2 qmp2(vault);

    qmp2.run(out);
    timings.stop_clock(2);
    if (id == 0) cout << timings.print_clocks(0);
    out << endl;

    if (id == 0) cout << out.str() << endl;

    MPI_Finalize();

    return 0;
}

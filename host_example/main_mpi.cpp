#include <iostream>
#include <sstream>
#include "../libqqc/methods/do_qmp2.h"
#include <mpi.h>

using namespace std;
using namespace libqqc;

int main (){

    MPI_Init(NULL, NULL);

    Loader_qmp2_from_file loader("../data/h2o/");
    Vault_qmp2 vault(loader);
    Do_qmp2 qmp2(vault);

    ostringstream out;
    qmp2.run(out);

    cout << out.str() << endl;

    MPI_Finalize();

    return 0;
}

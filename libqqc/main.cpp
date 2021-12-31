#include <iostream>
#include <sstream>

#include "folder/file1.h"

#if LIBQQC_WITH_MPI
    #include "mpi_folder/file2_mpi.h"
#endif

using namespace std;

namespace libqqc {
    int main (int argc, char **argv[]){

        cout << "Main reporting in" << endl;

        ostringstream out;
        out.str("");

        function1(out);
        cout <<out.str() <<endl;

#if LIBQQC_WITH_MPI
        function_mpi();
#endif

        return 0;
    }
} // namespace

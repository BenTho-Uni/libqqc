//include file header
#include "test_file2_mpi.h"

//include needed libraries
#include <iostream>
#include <sstream>
#include <string>

// includes of the to be tested headers
#include "../../libqqc/mpi_folder/file2_mpi.h"

// namespaces

using namespace std;

namespace libqqc{
        bool test_file2_mpi :: run_test() {

                // call Test function
                function_mpi(); 
                //

                // pass back bool 
                return 1;

        } // run_test

} // namespace

//include file header
#include "test_file3_eigen.h"

//include needed libraries
#include <iostream>
#include <sstream>
#include <string>

// includes of the to be tested headers
#include "../../libqqc/eigen_folder/file3_eigen.h"

// namespaces

using namespace std;

namespace libqqc{
        bool test_file3_eigen :: run_test() {

                // call Test function
                function_eigen(); 
                //

                // pass back bool 
                return 1;

        } // run_test

} // namespace

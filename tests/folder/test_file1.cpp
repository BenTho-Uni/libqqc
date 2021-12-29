//include file header
#include "test_file1.h"

//include needed libraries
#include <iostream>
#include <sstream>
#include <string>

// includes of the test headers
#include "../../libqqc/folder/file1.h"

// namespaces

using namespace std;

namespace libqqc{
        bool test_file1 :: run_test() {

                // create reference string
                string ref_string = "function1 reporting in";
                // create stringstream to compare to
                ostringstream compare;
                compare.str(""); //clear stringstream

                // call Test function
                function1(compare); 
                //

                // pass back bool 
                return (compare.str() == ref_string);

        } // run_test

} // namespace

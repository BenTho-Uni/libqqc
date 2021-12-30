
//include needed libraries
#include <iostream>

// includes of the test headers
#include "folder/test_file1.h"

// namespaces

using namespace libqqc;
using namespace std;

int main (){
        cout << "Performing tests for libqqc..." << endl;

        test_file1 file1;
        cout << "Testing folder/file1, function1 ... " << flush << 
                ((file1.run_test()) ? "passed" : "failed") << endl;

        return 0;
}


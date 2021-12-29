#include <iostream>
#include <sstream>

#include "folder/file1.h"
#include "folder/file2_mpi.h"

using namespace std;

namespace libqqc {
int main (){

    cout << "Main reporting in" << endl;

    ostringstream out;
    out.str("");

    function1(out);
    cout << out.str() <<endl;

    function2(out);
    cout << out.str() <<endl;

    return 0;
}
} // namespace

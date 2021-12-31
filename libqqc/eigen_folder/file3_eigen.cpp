/**
 * Example Eigen file for testing
 *
 */
#include "file3_eigen.h"

#include <Eigen>
#include <iostream>
#include <cmath>


using namespace std;

namespace libqqc{

    int function_eigen()
    {
        cout << " Eigen version : " << EIGEN_MAJOR_VERSION << " . "
            << EIGEN_MINOR_VERSION;
        return 0;
    }

}

//include file header
#include "test_load_from_file.h"


// includes of the to be tested headers
#include "../../libqqc/utils/load_from_file.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_load_from_file :: test_load_dim_from_file() {
        bool result = false;

        // Checking for 2D matrices
        // Loading in data/h2o/f.mat Fock matrix and checking it against reference
        vector<size_t> dim_ref = {13, 13, 1};
        vector<size_t> dim2D = load_dim_from_file("../../data/h2o/f.mat", 
                ' ', 1);

        // Comparing size and values
        //
        if ((dim_ref.size() == dim2D.size()) &&
                (dim_ref.at(0) == dim2D.at(0)) &&
                (dim_ref.at(1) == dim2D.at(1)) &&
                (dim_ref.at(2) == dim2D.at(2))) result = true;

        // Checking for 3D tensors/cubes
        dim_ref.at(2) = 2280;

        vector<size_t> dim3D = load_dim_from_file("../../data/h2o/coulomb.cube",
                ' ', 1);

        if ((dim_ref.size() == dim3D.size()) &&
                (dim_ref.at(0) == dim3D.at(0)) &&
                (dim_ref.at(1) == dim3D.at(1)) &&
                (dim_ref.at(2) == dim3D.at(2))) result = true;

        //TODO check if different skipping and different delimiters works

        return result;
    }
    bool Test_load_from_file :: test_load_array_from_file() {
        bool result = false;

        // Checking for 2D matrices
        // Setting up references dimensions and residual
        //
        vector<size_t> dim_ref = {13, 13, 1};
        double res_ref = -86.5529705391;

        // Loading in matrix, calculating residual and checking against reference
        double arr2D[dim_ref.at(0) * dim_ref.at(1) * dim_ref.at(2)];

        load_array_from_file("../../data/h2o/f.mat", dim_ref, arr2D, ' ', 1);

        double res = 0;
        for (int i = 0; i < dim_ref.at(0); i++){ 
            for (int j = 0; j < dim_ref.at(1); j++){
                res += arr2D[i * dim_ref.at(1) + j];
            }
        }
        if (abs(res - res_ref) < 1e-8) result = true;

        // Checking for 3D tensors/cubes
        // Setting up references dimensions and residual
        dim_ref.at(2) = 2280;
        res_ref = 35867.5646410161;

        double arr3D[dim_ref.at(0) * dim_ref.at(1) * dim_ref.at(2)];

        load_array_from_file("../../data/h2o/coulomb.cube", 
                dim_ref, arr3D, ' ', 1);

        for (int p = 0; p < dim_ref.at(2); p++){ 
            for (int i = 0; i < dim_ref.at(0); i++){ 
                for (int j = 0; j < dim_ref.at(1); j++){
                    res += arr3D[p * dim_ref.at(0) * dim_ref.at(1) 
                        + i * dim_ref.at(1) + j];
                }
            }
        }
        if (abs(res - res_ref) < 1e-8) result = (result && true);

        return result;
    }

    bool Test_load_from_file :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_load_dim_from_file = test_load_dim_from_file();
        out << "    Testing load_dim_from_file()       ... " << flush
            << (b_load_dim_from_file ? "passed" : "failed") << endl;

        bool b_load_array_from_file = test_load_array_from_file();
        out << "    Testing load_array_from_file()     ... " << flush
            << (b_load_array_from_file ? "passed" : "failed") << endl;

        result = b_load_dim_from_file && b_load_array_from_file;
        return result;

    } // run_test

} // namespace

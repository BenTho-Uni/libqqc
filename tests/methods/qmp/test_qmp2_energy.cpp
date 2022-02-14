//include file header

// includes of the to be tested headers
// TODO: How to do this better?
#include "../../../libqqc/methods/qmp/qmp2_energy_eigen.h"

#include <iomanip>

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Qmp2_energy :: test_compute() {
        bool result = false;

        // Setting up some fake data until load from file is implemented
        // the data we need for our test calculation
        size_t p1Dnpts = 3;
        size_t p3Dnpts = 2;
        size_t nocc = 2;
        size_t nvirt = 3;
        size_t offset = 1;
        size_t npts_to_proc = 1; 

        double mo[4] = {1.1, 1.2, 1.3, 1.4};
        double mv[6] = {2.1, 2.2, 2.3, 2.4, 2.5, 2.6};
        double c_c[12] = {3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 3.11,
                    3.12, 3.13};
        double m1Deps_o[6] = {4.1, 4.2, 4.3, 4.4, 4.5, 4.6};
        double m1Deps_v[9] = {5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9};
        double m1Deps_ov[18] = {9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9,
        9.11, 9.12, 9.13, 9.14, 9.15, 9.16, 9.17, 9.18, 9.19};
        double vf[5] = {6.1, 6.2, 6.3, 6.4, 6.5};
        double v1Dpts[3] = {7.1, 7.2, 7.3};
        double v1Dwts[3] = {8.1, 8.2, 8.3};
        double v3Dwts[2] = {1.11, 1.12};

        Qmp2_energy qmp2_energy(p1Dnpts, p3Dnpts, nocc, nvirt, mo, mv, c_c, m1Deps_o,
                m1Deps_v, m1Deps_ov, vf, v1Dpts, v1Dwts, v3Dwts, offset, npts_to_proc);

        double ene = qmp2_energy.compute();
        double ref = -83854163.66417963;
        double tol = 10e-8;
        result = (abs(ene - ref) < tol);

        return result;
    }

    bool Test_Qmp2_energy :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_compute = test_compute();
        out << "    Testing Qmp2_energy::compute()    ... " << flush
            << (b_compute ? "passed" : "failed") << endl;

        result = b_compute;
        return result;

    } // run_test

} // namespace

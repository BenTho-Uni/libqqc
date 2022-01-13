//include file header
#include "test_grid.h"


// includes of the to be tested headers
#include "../../libqqc/grids/grid.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Grid :: test_set_grid() {
        bool result = false;

        Grid grid;
        grid.set_grid(npts, ndim, pts, wts);
        size_t tnpts = grid.get_mnpts();
        size_t tndim = grid.get_mndim();
        double* tpts = grid.get_mpts();
        double* twts = grid.get_mwts();

        if ((npts == tnpts) && (ndim == tndim) && (pts[0] == tpts[0])
                && (pts[9] == tpts[9]) && (wts[0] == twts[0])
                && (wts[4] == twts[4])) result = true;

        return result;
    }

    bool Test_Grid :: test_check_data_validity() {
        bool result = false;

        Grid grid(npts, ndim, pts, wts);
        grid.check_data_validity();

        result = true;
        return result;
    }

    bool Test_Grid :: test_get_mnpts() {
        bool result = false;

        Grid grid(npts, ndim, pts, wts);
        size_t tnpts = grid.get_mnpts();

        if (npts == tnpts) result = true;

        return result;
    }

    bool Test_Grid :: test_get_mndim() {
        bool result = false;

        Grid grid(npts, ndim, pts, wts);
        size_t tndim = grid.get_mndim();

        if (ndim == tndim) result = true;

        return result;
    }

    bool Test_Grid :: test_get_mpts() {
        bool result = false;

        Grid grid(npts, ndim, pts, wts);
        double* tpts = grid.get_mpts();

        if ((pts[0] == tpts[0]) && (pts[9] == tpts[9])) result = true;

        return result;
    }

    bool Test_Grid :: test_get_mwts() {
        bool result = false;

        Grid grid(npts, ndim, pts, wts);
        double* twts = grid.get_mwts();

        if ((wts[0] == twts[0]) && (wts[4] == twts[4])) result = true;

        return result;
    }

    bool Test_Grid :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_set_grid = test_set_grid();
        out << "    Testing grid::set_grid()            ... " << flush
            << (b_set_grid ? "passed" : "failed") << endl;

        bool b_check_data_validity = test_check_data_validity();
        out << "    Testing grid::check_data_validity() ... " << flush
            << (b_check_data_validity ? "passed" : "failed") << endl;

        bool b_get_mnpts = test_get_mnpts();
        out << "    Testing grid::get_mnpts()           ... " << flush
            << (b_get_mnpts ? "passed" : "failed") << endl;

        bool b_get_mndim = test_get_mndim();
        out << "    Testing grid::get_mndim()           ... " << flush
            << (b_get_mndim ? "passed" : "failed") << endl;

        bool b_get_mpts = test_get_mpts();
        out << "    Testing grid::get_mpts()            ... " << flush
            << (b_get_mpts ? "passed" : "failed") << endl;

        bool b_get_mwts = test_get_mwts();
        out << "    Testing grid::get_mwts()            ... " << flush
            << (b_get_mwts ? "passed" : "failed") << endl;

        result = b_set_grid && b_check_data_validity && b_get_mnpts
            && b_get_mndim && b_get_mpts && b_get_mwts;
        return result;

    } // run_test

} // namespace

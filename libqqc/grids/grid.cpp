///
/// Member functions for the grid class
/// @file grid.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 11-01-2022
///

#include "grid.h"

using namespace std;

namespace libqqc {

    bool Grid :: check_data_validity() {

        if (mndim == 0) throw invalid_argument(
                "Dimensionality must not be 0.");
        if (mnpts == 0) throw invalid_argument(
                "You need at least one point for the grid");
        if (!mpts || !mwts) throw invalid_argument(
                "Your pts or wts pointer is NULL.");
        return true;

    }

    void Grid :: set_grid(size_t npts, size_t ndim, double* pts, double* wts) {
        if (npts == 0) throw invalid_argument(
                "The number of points cannot be 0.");
        if (ndim == 0) throw invalid_argument(
                "The dimensionality of grid cannot be 0.");
        if (!pts) throw invalid_argument(
                "Your pts pointer is NULL.");
        if (!wts) throw invalid_argument(
                "Your wts pointer is NULL.");

        mnpts = npts;
        mndim = ndim;

        //check if class variable pointers are null, if so, generate 
        // new on heap
        // if pointers not null it means that they were already
        // initialized. We delete them and generate new ones on heap. 

        if (mpts || mwts) {
            delete[] mpts;
            delete[] mwts;
        }

        mpts = new double[mnpts * mndim];    
        mwts = new double[mnpts];

        //loop over all points and dimensions and copy the data
        for (size_t i = 0; i < mnpts; i++) {
            for (size_t j = 0; j < mndim; j++) {
                mpts[i * mndim + j] = pts[i * mndim + j];
            }
            mwts[i] = wts[i];
        }//for 

        check_data_validity();
    }

    Grid :: Grid(size_t npts, size_t ndim, double* pts, double* wts) {

        set_grid(npts, ndim, pts, wts);

    }

} //namespace libqqc

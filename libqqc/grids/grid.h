#ifndef LIBQQC_GRID_H
#define LIBQQC_GRID_H

#include <stdexcept>

using namespace std;

namespace libqqc {

    ///
    /// @brief Holder class for information regarding a quadrature grid
    ///
    /// @details This class holds all information regarding a quadrature 
    /// grid. 
    ///

    class Grid {
        private:

            // Meta information
            size_t mnpts; ///< Number of points in the grid
            size_t mndim; ///< Dimensionality of a point

            //Points and weights
            double* mpts = NULL; ///< Pointer towards the points
            double* mwts = NULL; ///< Pointer towards the weights

        public: 

            ///
            /// @brief Function checks data validity of grid
            ///
            /// @details This function checks if the data in the grid is valid
            /// and returns TRUE if it is
            ///
            /// @return TRUE if data is valid
            ///
            bool check_data_validity();

            ///
            /// @brief Simple copy constructor 
            ///
            Grid (const Grid &grid1) {
                //sets the grid up, the setter function hardcopies the 
                //mpts and mwts values, so we dont run into double deletion
                //problems when the second grid gets destroyed
                set_grid (grid1.mnpts, grid1.mndim, grid1.mpts, grid1.mwts);
            }

            ///
            /// @brief Sets number, value and weights of points
            ///
            /// @details This function takes a number of grid points, and two 
            /// pointer to the points and weights arrays and copies them to 
            /// the new arrays
            ///
            /// @param[in] npts number of points
            /// @param[in] ndim dimensionalit of grid
            /// @param[in] pts pointer to grid points
            /// @param[in] wts pointer to grid weights
            ///
            void set_grid(size_t npts, size_t ndim, double* pts, double* wts);

            ///
            /// @brief Default constructor
            ///
            /// @details Default constructor
            ///
            Grid() {}; 

            ///
            /// @brief Constructor setting the pts and weights of the grid
            ///
            /// @details This constructor sets the points and weights and meta
            /// information of the grid 
            ///
            /// @param[in] npts Number of points
            /// @param[in] ndim Dimensionality of the grid
            /// @param[in,out] pts Reference pointer to the pts
            /// @param[in,out] wts Reference pointer to the wts
            ///
            Grid(size_t npts, size_t ndim, double* pts, double* wts);

            ///
            /// @brief Destructor, deletes heap arrays
            ///
            /// @details This destructor deletes the heap arrays
            ///
            ///
            ~Grid() {
                delete[] mpts;
                delete[] mwts;
            };

            ///
            /// @brief Getter for size of grid
            ///
            /// @details Returns size of grid
            ///
            /// @return size_t size of grid
            ///
            size_t get_mnpts() {
                if (mndim == 0) throw invalid_argument(
                        "Dimensionality must not be 0.");
                return mnpts;
            };

            ///
            /// @brief Getter for dimensionality of grid
            ///
            /// @details Returns dimensionality of grid
            ///
            /// @return size_t dimensionality of grid
            ///
            size_t get_mndim() {
                if (mnpts == 0) throw invalid_argument(
                        "You need at least one point for the grid");
                return mndim;
            };

            ///
            /// @brief Getter for pointer towards grid points
            ///
            /// @details Returns pointer towards grid points
            ///
            /// @return double* pointer towards grid points
            ///
            double* get_mpts() {
                if (!mpts ) throw invalid_argument(
                        "Your pts or wts pointer is NULL.");
                return mpts;
            };

            ///
            /// @brief Getter for pointer towards grid weights
            ///
            /// @details Returns pointer towards grid weights
            ///
            /// @return double* pointer towards grid weights
            ///
            double* get_mwts() {
                if (!mwts) throw invalid_argument(
                        "Your pts or wts pointer is NULL.");
                return mwts;
            };

    }; // class Grid

} // namespace libqqc

#endif //LIBQQC_GRID_H

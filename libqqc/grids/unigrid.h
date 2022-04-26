#ifndef LIBQQC_UNIGRID_H
#define LIBQQC_UNIGRID_H

namespace libqqc {

    ///
    /// @brief A Uniform grid class
    ///
    /// @details A Unifrom grid class creating points and weights for the numerical integration
    ///
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    class Grid_1D {
    private:
        
        vec_type coords_x;///< Vector with x-values for molecule centers
        n_dim x_dim;      ///< dimension of coords_x vector
        int n_points;

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
        Grid_1D(const Grid_1D& grid1) {
            //sets the grid up, the setter function hardcopies the 
            //mpts and mwts values, so we dont run into double deletion
            //problems when the second grid gets destroyed
            set_grid(grid1.coords_x, grid1.x_dim, grid1.n_points);
            set_weights(grid1.coords_x, grid1.x_dim, grid1.n_points);
        }

       ///
       /// @brief Initialisation of grid points for 1D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
        auto set_grid(vec_type& coords, n_dim& x_dim, int n_points);
       
       ///
       /// @brief Initialisation of the weights for 1D grid
       ///
       /// @details Calculating the weights for the grid points
       ///
       /// @return weights
       ///
        auto set_weights(vec_type& coords, n_dim& x_dim, int n_points);

        ///
        /// @brief Default constructor
        ///
        /// @details Default constructor
        ///
        Grid_1D() {};

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
        Grid_1D(vec_type coords_x, n_dim x_dim, int n_points);

        ///
        /// @brief Destructor, deletes heap arrays
        ///
        /// @details This destructor deletes the heap arrays
        ///
        ///
        ~Grid_1D() {
            //delete[] mpts;
            //delete[] mwts;
        };


        auto gauss_cheb(vec_type& coords, n_dim&, int n_points);

        //void print_func();

    }; // class Grid_1D

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    class Grid_2D {
    private:
        ///< [briefdescription]
        vec_type coords_x;///< Vector with x-values for molecule centers
        vec_type coords_y;///< Vector with y-values for molecule centers
        n_dim x_dim; ///< dimension or coords_x vector
        n_dim y_dim; ///< dimension or coords_y vector
        int n_points; 

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
        Grid_2D(const Grid_2D& grid1) {
            //sets the grid up, the setter function hardcopies the 
            //mpts and mwts values, so we dont run into double deletion
            //problems when the second grid gets destroyed
            set_grid(grid1.coords_x, grid1.coords_y, grid1.x_dim, grid1.y_dim, grid1.n_points);
            set_weights(grid1.coords_x, grid1.coords_y, grid1.x_dim, grid1.y_dim, grid1.n_points);

        }

       ///
       /// @brief Initialisation of grid points for 2D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
        auto set_grid(vec_type& coords_x, vec_type& coords_y, n_dim& x_dim, n_dim& y_dim, int n_points);

       ///
       /// @brief Initialisation of the weights for 2D grid
       ///
       /// @details Calculating the weights for the grid points
       ///
       /// @return weights
       ///
        auto set_weights(vec_type& coords_x, vec_type& coords_y, n_dim& x_dim, n_dim& y_dim, int n_points);

        ///
        /// @brief Default constructor
        ///
        /// @details Default constructor
        ///
        Grid_2D() {};

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
        Grid_2D(vec_type coords_x, vec_type coords_y, n_dim x_dim, n_dim y_dim, int n_points);

        ///
        /// @brief Destructor, deletes heap arrays
        ///
        /// @details This destructor deletes the heap arrays
        ///
        ///
        ~Grid_2D() {
            //delete[] mpts;
            //delete[] mwts;
        };

    }; // class Grid_2D

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    class Grid_3D {
    private:
        vec_type coords_x;///< Vector with x-values for molecule centers
        vec_type coords_y;///< Vector with y-values for molecule centers
        vec_type coords_z;///< Vector with z-values for molecule centers
        n_dim x_dim; ///< dimension or coords_x vector
        n_dim y_dim; ///< dimension or coords_y vector
        n_dim z_dim; ///< dimension or coords_z vector
        int n_points;

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
        Grid_3D(const Grid_3D& grid1) {
            //sets the grid up, the setter function hardcopies the 
            //mpts and mwts values, so we dont run into double deletion
            //problems when the second grid gets destroyed
            set_grid(grid1.coords_x, grid1.coords_y, grid1.coords_z, grid1.x_dim, grid1.y_dim, grid1.z_dim, grid1.n_points);
            set_weights(grid1.coords_x, grid1.coords_y, grid1.coords_z, grid1.x_dim, grid1.y_dim, grid1.z_dim, grid1.n_points);

        }

       ///
       /// @brief Initialisation of grid points for 3D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
        auto set_grid(vec_type& coords_x, vec_type& coords_y, vec_type& coords_z, n_dim& x_dim, n_dim& y_dim, n_dim& z_dim, int n_points);

       ///
       /// @brief Initialisation of the weights for 3D grid
       ///
       /// @details Calculating the weights for the grid points
       ///
       /// @return weights
       ///
        auto set_weights(vec_type& coords_x, vec_type& coords_y, vec_type& coords_z, n_dim& x_dim, n_dim& y_dim, n_dim& z_dim, int n_points);
        
        ///
        /// @brief Default constructor
        ///
        /// @details Default constructor
        ///
        Grid_3D() {};

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
        Grid_3D(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim x_dim, n_dim y_dim, n_dim z_dim, int n_points);

        ///
        /// @brief Destructor, deletes heap arrays
        ///
        /// @details This destructor deletes the heap arrays
        ///
        ///
        ~Grid_3D() {
            //delete[] mpts;
            //delete[] mwts;
        };

    }; // class Grid_3D


} // namespace libqqc

#endif //LIBQQC_UNIGRID_H

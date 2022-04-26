//#ifndef LIBQQC_UNIGRID_H
//#define LIBQQC_UNIGRID_H

//namespace libqqc {

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

    public:
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
    public:
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
    public:
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


    }; // class Grid_3D


//} // namespace libqqc

//#endif //LIBQQC_UNIGRID_H

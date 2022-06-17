#ifndef LIBQQC_ISABELGRID_H
#define LIBQQC_ISABELGRID_H

#include "voronoi.h"

#include <vector>
#include <stdexcept>
#include <map>

using namespace std;

namespace libqqc {

    ///
    /// @brief A Uniform grid class
    ///
    /// @details A Unifrom grid class creating points and weights for the numerical integration
    ///
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    class Grid1D {
    private:
        
        vec_type coords_x;///< Vector with x-values for molecule centers
        n_dim ndim;      ///< dimension of a grid point
        int n_points; ///< number of grid points
//        //int vec_size = static_cast<int>(n_points);
//        vector<double>* x_grid;
//        vector<double>* x_weight;
          
        //pointers to points and weights
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
        /// @brief Simple copy constructor unigrid
        ///      for( int p =0; p < n_points; p++){

        Grid1D(const Grid1D& grid1) {
            //sets the grid up, the setter function hardcopies the 
            //mpts and mwts values, so we dont run into double deletion
            //problems when the second grid gets destroyed
            set_grid(grid1.coords_x, grid1.ndim, grid1.n_points);
            set_weights(grid1.coords_x, grid1.ndim, grid1.n_points);
        };

       ///
       /// @brief Initialisation of grid points for 1D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
       vector<double>  set_grid(vec_type coords_x, n_dim ndim, int n_points);
       
       ///
       /// @brief Initialisation of the weights for 1D grid
       ///
       /// @details Calculating the weights for the grid points
       ///
       /// @return weights
       ///
       vector<double> set_weights(vec_type coords_x, n_dim ndim, int n_points);
        
       ///
       /// @brief Initialisation of Gaussian-Chebyshev grid points for 1D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
       vector<vector<double>> gauss_cheb(vec_type coords_x, int n_points);
        
        ///
        /// @brief Default constructor
        ///
        /// @details Default constructor
        ///
        Grid1D() {};

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
        Grid1D(vec_type coords_x, n_dim ndim, int n_points);

        ///
        /// @brief Destructor, deletes heap arrays
        ///
        /// @details This destructor deletes the heap arrays
        ///
        ///
        ~Grid1D() {
            delete[] mpts;
            delete[] mwts;
        };
        
        ///
            /// @brief Getter for size of grid
            ///
            /// @details Returns size of grid
            ///
            /// @return int size of grid
            ///
            size_t get_mnpts() {
                if (ndim == 0) throw invalid_argument(
                        "Dimensionality must not be 0.");
                return n_points;
            };

            ///
            /// @brief Getter for dimensionality of grid
            ///
            /// @details Returns dimensionality of grid
            ///
            /// @return size_t dimensionality of grid
            ///
            size_t get_mndim() {
                if (n_points == 0) throw invalid_argument(
                        "You need at least one point for the grid");
                return ndim;
            };

            ///
            /// @brief Getter for pointer towards grid points
            ///
            /// @details Returns pointer towards grid points
            ///
            /// @return double* pointer towards grid points
            ///
            vector<double> get_mpts(vec_type coords_x, int n_points) {
                
                vector<vector<double>> result = gauss_cheb(coords_x, n_points);
                return result[0];
            };

            ///
            /// @brief Getter for pointer towards grid weights
            ///
            /// @details Returns pointer towards grid weights
            ///
            /// @return double* pointer towards grid weights
            ///
            vector<double> get_mwts(vec_type coords_x, int n_points) {
               
                vector<vector<double>> result = gauss_cheb(coords_x, n_points);
                return result[1];
            };
        
    }; // class Grid_1D

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    class Grid2D {
    private:
        ///< [briefdescription]
        vec_type coords_x;///< Vector with x-values for molecule centers
        vec_type coords_y;///< Vector with y-values for molecule centers
        n_dim ndim; ///< dimension of a grid point
        int n_points; ///< number of grid points
        //pointers to points and weights
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
        Grid2D(const Grid2D& grid1) {
            //sets the grid up, the setter function hardcopies the 
            //mpts and mwts values, so we dont run into double deletion
            //problems when the second grid gets destroyed
            set_grid(grid1.coords_x, grid1.coords_y, grid1.dim, grid1.n_points);
            set_weights(grid1.coords_x, grid1.coords_y, grid1.dim, grid1.n_points);

        };
        
       ///
       /// @brief Initialisation of grid points for 2D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
        vector<vector<double>> set_grid(vec_type coords_x, vec_type coords_y, n_dim ndim, int n_points);

       ///
       /// @brief Initialisation of the weights for 2D grid
       ///
       /// @details Calculating the weights for the grid points
       ///
       /// @return weights
       ///
        vector<vector<double>>set_weights(vec_type coords_x, vec_type coords_y, n_dim ndim, int n_points);

        ///
        /// @brief Default constructor
        ///
        /// @details Default constructor
        ///
        Grid2D() {};

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
        Grid2D(vec_type coords_x, vec_type coords_y, n_dim ndim, int n_points);

        ///
        /// @brief Destructor, deletes heap arrays
        ///
        /// @details This destructor deletes the heap arrays
        ///
        ///
        ~Grid2D() {
            delete[] mpts;
            delete[] mwts;
        };

    }; // class Grid_2D

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    class Grid3D {
    private:
        vec_type coords_x;///< Vector with x-values for molecule centers
        vec_type coords_y;///< Vector with y-values for molecule centers
        vec_type coords_z;///< Vector with z-values for molecule centers
        n_dim ndim; ///< dimension of a grid points
        int n_points; ///< number of grid points
	
	size_t mndim;
	size_t mnpts;	
        vector<vector<vector<double>>> gauss_cheb_pts; 
        //map<vector<double> ,vector<double>> vertex_weights;
        multimap<vector<double>, vector<double>> pts_in_cell;
        
        //pointers to points and weights
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
       /// @brief Simple copy constructor unigrid
       ///
       Grid3D(const Grid3D& grid1) {
            //sets the grid up, the setter function hardcopies the 
            //mpts and mwts values, so we dont run in to double deletion
            //problems when the second grid gets destroyed
            //set_grid(grid1.coords_x, grid1.coords_y, grid1.coords_z, grid1.ndim, grid1.n_points);
            //set_weights(grid1.coords_x, grid1.coords_y, grid1.coords_z, grid1.ndim, grid1.n_points);
        };

      ///
      /// @brief Default constructor
      ///
      /// @details Default constructor
      ///
      Grid3D() {};
      
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
      Grid3D(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim ndim, int n_points);
      
      ///
      /// @brief Destructor, deletes heap arrays
      ///
      /// @details This destructor deletes the heap arrays
      ///
      ///
      ~Grid3D() {
          delete[] mpts;
          delete[] mwts;
      };
        
       ///
       /// @brief Initialisation of grid points for 3D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
       vector<vector<double>> set_grid(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim ndim, int n_points);
       ///
       /// @brief Initialisation of the weights for 3D grid
       ///
       /// @details Calculating the weights for the grid points
       ///
       /// @return weights
       ///
       vector<vector<double>> set_weights(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim ndim, int n_points);
       
       ///
       /// @brief Initialisation of Gaussian-Chebyshev grid points for 3D grid
       ///
       /// @details constructing grid points and finding the most benefical ones
       ///
       /// @return grid points
       ///
       vector<vector<vector<double>>> gauss_cheb_3D(vec_type coords_x, vec_type coords_y, vec_type coords_z, int n_points);
       ///
       /// @brief Finding which voronoi cell a grid point lies in
       ///
       /// @details constructing a map with a key corresponding to voronoi cell which respective point lies in
       ///
       /// @return map of vertex for corresponding voronoi cell as key and the value of the grid point 
       ///
       multimap<vector<double>, vector<double>> find_cell(vector<vector<vector<double>>> gauss_cheb_pts, int n_points, Voronoi<vec_type> &Voro); // multimap<vector<double>, vector<vector<vector<double>>>> voronoi_cells);
       ///
       /// @brief Initialisation of Voronoi cells for grid points for 3D grid
       ///
       /// @details constructing voronoi cells and give each grid point a weight accordingly
       ///
       //auto voronoi_grid_3D(vec_type coords_x, vec_type coords_y, vec_type coords_z, int n_points);
       ///
       /// @brief Initialisation of Becke 3D grid
       ///
       /// @details constructing voronoi cells, weights and calculating which cell the grid points are in
       ///
       void set_voronoi_grid(vector<vector<vector<double>>> gauss_cheb_pts, multimap<vector<double>, vector<double>> pts_in_cell, int n_points, n_dim ndim, Voronoi<vec_type> &Voro);
       
       ///
       /// @brief Initialisation of Becke 3D grid
       ///
       /// @details constructing voronoi cells, weights and calculating which cell the grid points are in
       ///
       void set_grid_wrapper(vec_type coords_x, vec_type coords_y, vec_type coords_z, int n_points, n_dim ndim);
      
      ///
      /// @brief Getter for size of grid
      ///
      /// @details Returns size of grid
      ///
      /// @return int size of grid
      ///
      size_t  get_mnpts(){
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
      size_t get_mndim(){
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
      double* get_mpts(){
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
      double* get_mwts(){
          if (!mwts) throw invalid_argument(
                  "Your pts or wts pointer is NULL.");
          return mwts;
      };
      

    }; // class Grid_3D

} // namespace libqqc

#endif

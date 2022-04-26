///
/// A uniform grid constructor
/// @file unigrid.cpp
/// @author Isabel Vinterbladh
/// @version 0.1 19-04-2022
//

#define _USE_MATH_DEFINES

#include "unigrid.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
//#include <math.h> 
#include <numbers>

using namespace std;

namespace libqqc {

    // 1D Grid
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    bool Grid_1D<vec_type, ndim> ::check_data_validity() {

        if (x_dim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        if (coords_x == 0) throw invalid_argument(
            "You need at least one point for the grid");
        //if (!mpts || !mwts) throw invalid_argument(
          //  "Your pts or wts pointer is NULL.");
        return true;
    } // Grid_1D::check_data_validity

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    auto Grid_1D<vec_type, n_dim> ::set_grid(vec_type& coords, n_dim& x_dim, int n_points) {
        auto max = coords[0];
        auto min = coords[0];

        for (int i = 1; i < x_dim; i++) {
            if (coords[i] < min) {
                min = coords[i];
            }
            if (coords[i] > max) {
                max = coords[i];
            }
        }
        auto iter = max - min / n_points;
        vec_type unigrid(n_points, 0);

        for (int i = 0; i < n_points; i++) {
            unigrid[i] = min + iter * i;
        }
        return unigrid;
    } //Grid_1D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    auto Grid_1D<vec_type, n_dim> ::set_weights(vec_type& coords, n_dim& x_dim, int n_points) {
        auto weight = 1.0 / n_points;
        vector<double> w_vec(n_points, weight);
        return w_vec;
    } //Grid_1D::set_weights

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    auto Grid_1D<vec_type, n_dim> ::gauss_cheb(vec_type& coords, n_dim& x_dim, int n_points) {
        vector<double> x_grid(n_points, 0.0);
        double pi = 3.1415926535897932384626434;
        auto weight = pi / n_points;
        vector<double> x_weight(n_points, weight);
        for (int i = 1; i < n_points; i++){
                double result;
                result = 11.0/2.0 + (9.0/2.0)*cos((2.0 * i - 1.0) / (2.0 * n_points) * pi);
                x_grid[i-1] = result;
        }
        vector<vector<double>> vec;
        vec.push_back(x_grid);
        vec.push_back(x_weight);
        return vec;
    } //Grid_1D::gauss_cheb

   // template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    //void Grid_1D<vec_type, n_dim> ::print_func() {
     //   Grid_1D<vec_type, n_dim> class_obj;
      //  vector<double> grid_x = class_obj.set_grid(coords_x, n_points);

    //}
    
    // 2D Grid
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    bool Grid_2D<vec_type, ndim> ::check_data_validity() {

        if (x_dim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        if (coords_x.size() == 0) throw invalid_argument(
            "You need at least one point for the grid");
        if (y_dim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        if (coords_y.size() == 0) throw invalid_argument(
            "You need at least one point for the grid");
        //if (!mpts || !mwts) throw invalid_argument(
          //  "Your pts or wts pointer is NULL.");
        return true;
    } //Grid_2D::check_data_validity

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    auto Grid_2D<vec_type, n_dim> ::set_grid(vec_type& coords_x, vec_type& coords_y, n_dim& x_dim, n_dim& y_dim, int n_points) {
        Grid_1D<vec_type, n_dim> class_obj;
        auto grid_x = class_obj.set_grid(coords_x, x_dim, n_points);
        auto grid_y = class_obj.set_grid(coords_y, y_dim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(grid_x);
        return_vec.push_back(grid_y);
        return return_vec;
    } //Grid_2D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    auto Grid_2D<vec_type, n_dim> ::set_weights(vec_type& coords_x, vec_type& coords_y, n_dim& x_dim, n_dim& y_dim, int n_points) {
        Grid_1D<vec_type, n_dim> class_obj;
        vector<double> w_x = class_obj.set_weights(coords_x, x_dim, n_points);
        vector<double> w_y = class_obj.set_weights(coords_y, y_dim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(w_x);
        return_vec.push_back(w_y);
        return return_vec;
    } //Grid_2D::set_weights

    // 3D Grid
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    bool Grid_3D<vec_type, ndim> ::check_data_validity() {

        if (x_dim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        if (coords_x.size() == 0) throw invalid_argument(
            "You need at least one point for the grid");
        if (y_dim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        if (coords_y.size() == 0) throw invalid_argument(
            "You need at least one point for the grid");
        if (z_dim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        if (coords_z.size() == 0) throw invalid_argument(
            "You need at least one point for the grid");
        //if (!mpts || !mwts) throw invalid_argument(
        //  "Your pts or wts pointer is NULL.");
        return true;
    } //Grid_3D::check_data_validity

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    auto Grid_3D<vec_type, n_dim> ::set_grid(vec_type& coords_x, vec_type& coords_y, vec_type& coords_z, n_dim& x_dim, n_dim& y_dim, n_dim& z_dim, int n_points) {
        Grid_1D<vec_type, n_dim> class_obj;
        auto grid_x = class_obj.set_grid(coords_x, x_dim, n_points);
        auto grid_y = class_obj.set_grid(coords_y, y_dim, n_points);
        auto grid_z = class_obj.set_grid(coords_z, z_dim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(grid_x);
        return_vec.push_back(grid_y);
        return_vec.push_back(grid_z);
        return return_vec;
    } //Grid_3D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    auto Grid_3D<vec_type, n_dim> ::set_weights(vec_type& coords_x, vec_type& coords_y, vec_type& coords_z, n_dim& x_dim, n_dim& y_dim, n_dim& z_dim, int n_points) {
        Grid_1D<vec_type, n_dim> class_obj;
        vector<double> w_x = class_obj.set_weights(coords_x, x_dim, n_points);
        vector<double> w_y = class_obj.set_weights(coords_y, y_dim, n_points);
        vector<double> w_z = class_obj.set_weights(coords_z, z_dim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(w_x);
        return_vec.push_back(w_y);
        return_vec.push_back(w_z);
        return return_vec;
    } //Grid_3D::set_weights

   

} //namespace libqqc

#ifndef LIBQQC_VORNOI_H
#define LIBQQC_VORONOI_H

#include <vector>
#include <map>

using namespace std;

namespace libqqc {

    ///
    /// @brief A Voronoi cell generator class
    ///
    /// @details A class creating the Delaunay triangulations and then the voronoi cells
    ///
template<typename vec_type>
class Voronoi {
private:
    multimap<vector<double>, vector<vector<vector<double>>>> voronoi_cells;
  
public:
    
    ///
    /// @brief Function sorting the points 
    ///
    /// @details Function sorting the points of increaseing x values
    ///
    /// @return sorted x and y points
    ///
    vector<vector<double>> sorting(vec_type x_pts, vec_type y_pts);
    ///
    /// @brief Function constructing the super triangle
    ///
    /// @details With the points give a super triangle is constructed which is bigger and includes all the points
    ///
    /// @return Return vertixes of super triangle
    ///
    vector<vector<double>> def_supertriangle(vector<vector<double>> sorted);
    ///
    /// @brief Function performs watsons algorithm
    ///
    /// @details This function executes watsons algorithm and created the Delaunay triangles
    ///
    /// @return Return Delaunay triangle grid
    ///
    vector<vector<vector<double>>> watsons_alg(vector< vector<double> > sorted, vector< vector<double> > super_triangle);
    ///
    /// @brief Function constructs dual graph
    ///
    /// @details This function constructs the dual graph which then gives the voronoi cells
    ///
    /// @return Return voronoi cells 
    ///  
    vector<vector<vector<double>>> getting_dual_graph(vector<vector<vector<double>>> triangles);
    
    ///
    /// @brief Function sorting the 3D points 
    ///
    /// @details Function sorting the 3D points of increaseing x values
    ///
    /// @return sorted x, y and z points
    ///
    vector<vector<double>>  sorting_3D(vec_type x_pts, vec_type y_pts, vec_type z_pts);
    ///
    /// @brief Function constructing the super 3D triangle
    ///
    /// @details With the points give a 3D super triangle is constructed which is bigger and includes all the points
    ///
    /// @return Return vertixes of 3D super triangle
    ///
    vector<vector<double>>  def_supertriangle_3D(vector<vector<double>> sorted);
    ///
    /// @brief Function performs watsons algorithm for 3D case
    ///
    /// @details This function executes watsons algorithm for 3D case and created the Delaunay triangles
    ///
    /// @return Return Delaunay triangle grid 3D
    ///
    vector<vector<vector<double>>> watsons_alg_3D(vector< vector<double> > sorted, vector< vector<double> > super_triangle);
    ///
    /// @brief Function constructs dual graph 3D
    ///
    /// @details This function constructs the dual graph 3D which then gives the voronoi cells
    ///
    /// @return Return voronoi cells 3D
    ///  
    multimap<vector<double>, vector<vector<vector<double>>>>  getting_dual_graph_3D(vector<vector<vector<double>>> triangles);
    ///
    /// @brief Function calculates weight for point in voronoi cell
    ///
    /// @details This function calculates the weight for one coordinate in space
    ///
    /// @return Return voronoi cells weight for point
    ///  
    double weight_generator(vector<double> pts, vector<double> vertex_pts, vector<vector<double>> sorted_pts);
    ///
    /// @brief Function calculates weights for voronoi cells
    ///
    /// @details This function calculates the weights for each dimension of voronoi cell
    ///
    /// @return Return voronoi cells weights
    ///  
    multimap<vector<double>, double> voronoi_weights(vector<vector<double>> sorted_pts, vector<vector<vector<double>>> gauss_cheb_pts, multimap<vector<double>, vector<double>> pts_in_cell);
    ///
    /// @brief Function calling all functions for constructing the voronoi cells
    ///
    /// @details This function calls all necessary functions and constructs the 3D voronoi cells
    ///
    /// @return Return voronoi cells 3D with weights
    ///  
    void construct_voronoi_3D(vec_type x_pts, vec_type y_pts, vec_type z_pts); 
   
   ///
   /// @brief Getter for size of grid
   ///
   /// @details Returns size of grid
   ///
   /// @return size_t size of grid
   ///
   multimap<vector<double>, vector<vector<vector<double>>>>  get_vcells() {
   return voronoi_cells;
    };
   ///
   /// @brief Getter for size of grid
   ///
   /// @details Returns size of grid
   ///
   /// @return size_t size of grid
   ///
   multimap<vector<double>, double>  get_vweights(vector<vector<vector<double>>> gauss_cheb_pts, multimap<vector<double>, vector<double>> pts_in_cell, vector<vector<double>> sorted_pts) {
   return voronoi_weights(sorted_pts, gauss_cheb_pts, pts_in_cell);
    };

}; // Class Voronoi

} // namespace libqqc

#endif 

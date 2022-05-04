#ifndef LIBQQC_UNIGRID_H
#define LIBQQC_UNIGRID_H

#include <vector>

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

public:
    ///
    /// @brief Function sorting the points 
    ///
    /// @details Function sorting the points of increaseing x values
    ///
    /// @return sorted x and y points
    ///
    auto sorting(vec_type& x_pts, vec_type& y_pts);
    ///
    /// @brief Function constructing the super triangle
    ///
    /// @details With the points give a super triangle is constructed which is bigger and includes all the points
    ///
    /// @return Return vertixes of super triangle
    ///
    auto def_supertriangle(vector<vector<double>>& sorted);
    ///
    /// @brief Function performs watsons algorithm
    ///
    /// @details This function executes watsons algorithm and created the Delaunay triangles
    ///
    /// @return Return Delaunay triangle grid
    ///
    auto watsons_alg(vector< vector<double> >& sorted, vector< vector<double> >& super_triangle);
      
    

}; // Class Voronoi

} // namespace libqqc

#endif //LIBQQC_UNIGRID_H

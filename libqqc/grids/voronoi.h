//#ifndef LIBQQC_UNIGRID_H
//#define LIBQQC_UNIGRID_H

//namespace libqqc {
#include <vector>
#include <list>
using namespace std;
    ///
    /// @brief A Voronoi cell generator class
    ///
    /// @details A class creating the Delaunay triangulations and then the voronoi cells
    ///



template<typename vec_type>
class Voronoi {
private:

public:
    auto sorting(vec_type& x_pts, vec_type& y_pts);

    auto def_supertriangle(vector<vector<double>>& sorted);

    auto watsons_alg(vector< vector<double> >& sorted, vector< vector<double> >& super_triangle);

};

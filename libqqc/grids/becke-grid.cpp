///
/// A 1D to 3D grid constructor
/// @file becke-grid.cpp
/// @author Isabel Vinterbladh
/// @version 1 12-09-2022
//

#define _USE_MATH_DEFINES

#include "vectorfuncs.h"
#include "isabel-grids.h"

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath> 
#include <stddef.h>
#include <numeric>
#include <limits>

using namespace std;

namespace libqqc {
    //
    // 1D Grid functions 
    //
    //Uniform grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<double>  Grid1D<vec_type, n_dim> ::set_unigrid(vec_type coords_x, n_dim ndim, int n_points) {
        double max = coords_x[0];
        double min = coords_x[0];

        for (int i = 1; i < coords_x.size(); i++) {
            if (coords_x[i] < min) {
                min = coords_x[i];
            }
            if (coords_x[i] > max) {
                max = coords_x[i];
            }
        }
        double iter = (max - min) / n_points;
        vector<double> unigrid(n_points, 0);
        min = min - 0.5;
	max = max + 0.5; 
        for (int i = 0; i < n_points; i++) {
            unigrid[i] = min + iter * i; // Generating vector with uniform grid points
        }
        return unigrid;
    } //Grid_1D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, nn_dim: dimension of grid point
    vector<double> Grid1D<vec_type, n_dim> ::set_uniweights(vec_type coords_x, n_dim ndim, int n_points) {
        double weight = 1.0 / n_points;
        vector<double> w_vec(n_points, weight); // Generating vector with uniform weights
        return w_vec;
    } //Grid_1D::set_weights
    //Uniform grid end
    
    //
    // 3D Grid functions 
    //
    // Uniform grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<vector<double>> Grid3D<vec_type, n_dim> ::set_unigrid(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim ndim, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
	// Setting up uniform grid points for all 3 dim
        vector<double> grid_x = class_obj.set_unigrid(coords_x, ndim, n_points);
        vector<double> grid_y = class_obj.set_unigrid(coords_y, ndim, n_points);
        vector<double> grid_z = class_obj.set_unigrid(coords_z, ndim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(grid_x);
        return_vec.push_back(grid_y);
        return_vec.push_back(grid_z);
        return return_vec;
    } //Grid_3D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    vector<vector<double>> Grid3D<vec_type, n_dim> ::set_uniweights(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim ndim, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
	// Setting up uniform weights for all 3 dim
        vector<double> w_x = class_obj.set_uniweights(coords_x, ndim, n_points);
        vector<double> w_y = class_obj.set_uniweights(coords_y, ndim, n_points);
        vector<double> w_z = class_obj.set_uniweights(coords_z, ndim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(w_x);
        return_vec.push_back(w_y);
        return_vec.push_back(w_z);
        return return_vec;
    } //Grid_3D::set_weights
    //Uniform grid end

    template<typename vec_type, typename n_dim>
    void Grid3D<vec_type, n_dim> :: set_unigrid_wrapper(vec_type x_pts, vec_type y_pts, vec_type z_pts, int n_points, n_dim ndim){
    // Wrapper function generating uniform grid
    vector<vector<double>> grid_pts = set_unigrid(x_pts,y_pts,z_pts, ndim, n_points);
    vector<vector<double>> grid_wts = set_uniweights(x_pts,y_pts,z_pts, ndim, n_points);
    mnpts = n_points;
    mndim = ndim;
    //if initialised - delete them, if  NULL generate new ones
    if (mpts || mwts) {
          delete[] mpts;
          delete[] mwts;
    }
    mpts = new double[mnpts * mndim];
    mwts = new double[mnpts];
    cout << grid_pts.size() << ";" << grid_pts[0].size() << "\n";
    //loop over all points and dimensions and copy the data
    for (size_t i =0; i < mnpts; i++) {

      for (size_t d = 0; d < mndim; d++) {
        mpts[i * mndim + d] = grid_pts[d][i];
      }
      mwts[i] = (grid_wts[0][i] + grid_wts[1][i] + grid_wts[2][i])/3;
  }
    }//Grid3D::set_unigrid_wrapper

    //
    // Ellipsiodal grid 3D
    //
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<vector<double>> Grid3D<vec_type, n_dim> ::ell_grid(vector<vector<double>> center, vector<vector<vector<double>>> foci, vec_type x_pts, vec_type y_pts, vec_type z_pts, int n_points){
 	 VectorFuncs vecf; //initialise vectorfunc class
	 // Generate ellipsoidal grid
	 vector<double> mm_x = vecf.min_max(x_pts);
	 vector<double> mm_y = vecf.min_max(y_pts);
    	 vector<double> mm_z = vecf.min_max(z_pts);
	 vector<vector<double>> final_pts;

	 for(int i=0; i< center.size() ; i++){
	 for(int e=0; e< 5; e++){
	 double a = (e+1)*vecf.Length(foci[i][0],center[i])/5;
	 double b = 0.2 +e*0.15;
	 double c = 0.1 +e*0.15;
	   for(int p=0; p< n_points/(5*center.size()); p++){
	   double pi = 3.1415926535897932384626434;
	   double theta = p*(pi/(n_points/(5*center.size())));
	   double phi = p*((2*pi)/(n_points/(5*center.size())));
 	   double x = a*sin(theta)*cos(phi); 
	   double y = b*sin(theta)*sin(phi);
	   double z = c*cos(theta);
	   final_pts.push_back({x,y,z});
	   }
	 //test if points are on ellipsiod
	 //for(int pp =0 ; pp <prob_pts.size(); pp++){
	   //if( (pow(prob_pts[pp][0]/a,2) + pow(prob_pts[pp][1]/b,2) + pow(prob_pts[pp][2]/c,2)) -1< 0.00000000001){ 
		//   final_pts.push_back(prob_pts[pp]);
		//   cout<< prob_pts[pp][0] << "," << prob_pts[pp][1] << "," << prob_pts[pp][2] << ",\n";
	 } }
	 //}}
	 return final_pts;
 	}// Grid3D::ell_grid

    //
    // 1D Grid functions 
    //
    //Becke grid
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    bool Grid1D<vec_type, n_dim> ::check_data_validity() {
        // if (n_points == 0) throw invalid_argument(
        //    "Number of points  must not be 0.");
        return true;
    } // Grid_1D::check_data_validity
    
    //Gauss-Chebyshev grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<vector<double>> Grid1D<vec_type, n_dim> ::gauss_cheb(vec_type x_pts, int n_points) {
        vector<double> grid;
        vector<double> weight;
        double max = x_pts[0]; // finding interval of points, maximum
        double min = x_pts[0]; // finding interval of points, minimum
        //for loop to find max and min
        for (int i = 1; i < x_pts.size(); i++) {
            if (x_pts[i] < min) {
                min = x_pts[i];
            }
            if (x_pts[i] > max) {
                max = x_pts[i];
            }
	}
        // check if all values are zero, if yes set interval to [-1,1]	
	if(abs(max) < 0.00000000001 & abs(min) < 0.00000000001) {
	min = -1;
	max = 1;
	}
	min = min - 0.05;
        max = max + 0.05;
        double pi = 3.1415926535897932384626434;
        double w = pow((max-min)/2,2)* pi/(n_points + 1);
	// constructing the grid points and weights ith Gauss-Chebyshev
        for (int i = 0; i < n_points; i++){
          double y_i = (min+max)/2 + ((max-min)/2)*cos((i*pi)/(n_points+1));
          grid.insert(grid.end(), y_i);
          double w_i = 0;
          if((y_i - min)*(max-y_i) < 0) {w_i = w*pow(sin((i*pi)/(n_points +1)),2);} // if expression under square root is negative the weight becomes nan
          if((y_i-min)*(max-y_i) >= 0) {w_i  = w*pow(sin((i*pi)/(n_points +1)),2)*sqrt((y_i-min)*(max-y_i));}
	  weight.insert(weight.end(), w_i);
           }
        vector<vector<double>> result;
        result.push_back(grid);
        result.push_back(weight);
        return result;
    } //Grid_1D::gauss_cheb
    //Gauss-Chebyshev grid end
    
    //
    // 3D Grid functions 
    //
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    bool Grid3D<vec_type, n_dim> ::check_data_validity() {
        //if (ndim == 0) throw invalid_argument(
        //    "Dimensionality must not be 0.");
    	return true;
    } //Grid_3D::check_data_validity

    // Becke grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    vector<vector<vector<double>>> Grid3D<vec_type, n_dim> ::gauss_cheb_3D(vec_type x_pts, vec_type y_pts, vec_type z_pts, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
        // Construct grid points and weights for all 3 dim
        vector<double> xpts = class_obj.get_mpts(x_pts, n_points);
        vector<double> xwts = class_obj.get_mwts(x_pts, n_points);
      
        vector<double> ypts = class_obj.get_mpts(y_pts, n_points);
        vector<double> ywts = class_obj.get_mwts(y_pts, n_points);
    
        vector<double> zpts = class_obj.get_mpts(z_pts, n_points);
        vector<double> zwts = class_obj.get_mwts(z_pts, n_points);

        vector<vector<double>> grid;
        vector<vector<double>> weights;
        grid.push_back(xpts);
        grid.push_back(ypts);
        grid.push_back(zpts);
        weights.push_back(xwts);
        weights.push_back(ywts);
        weights.push_back(zwts);
        vector<vector<vector<double>>> return_vec {grid, weights};
        return return_vec;
    } //Grid_3D::gauss_cheb_3D
  
  //
  // Voronoi class functions
  // 
  // 3D functions
  //
	template<typename vec_type> // vec_type: vector type for points
	 vector<vector<double>> Voronoi<vec_type> ::sorting_3D(vec_type x_pts, vec_type y_pts, vec_type z_pts) {
		if (x_pts.size() != y_pts.size() || x_pts.size() != z_pts.size() || z_pts.size() != y_pts.size()) throw invalid_argument(
			"Must be same dimensionality of x, y and z points.");

		// initializing variables
		double min_x = x_pts[0];
		double min_y = y_pts[0];
		double min_z = z_pts[0];
		int min_iter = 0;

		vector<double> sorted(3, 0); // vector with the sorted point
		vector<vector<double>> all_sort; // vector with all points sorted
		//Finding minimum of the x_pts points
		while (x_pts.size() > 0) {
			for (int i = 1; i < x_pts.size(); i++) {
				if (x_pts[i] < min_x) {
					min_x = x_pts[i];
					min_y = y_pts[i];
					min_z = z_pts[i];
					min_iter = i;
				}
			}
			// saving the minimum found and then add to vector with all sorted points
			sorted[0] = min_x;
			sorted[1] = min_y;
			sorted[2] = min_z;
			all_sort.push_back(sorted);
			// erase found minimum point from original points list
			x_pts.erase(x_pts.begin() + min_iter);
			y_pts.erase(y_pts.begin() + min_iter);
			z_pts.erase(z_pts.begin() + min_iter);

			// setting new initial values for variables
			if (x_pts.size() != 0) {
				min_x = x_pts[0];
				min_y = y_pts[0];
				min_z = z_pts[0];
				min_iter = 0;
			}
		}
		return all_sort; //returning vector with sorted points
	} // Voronoi::sorting_3D

	template<typename vec_type> // vec_type: vector type for points
	vector<vector<double>> Voronoi<vec_type> ::def_supertriangle_3D(vector<vector<double>>  sorted) {
		// create super-triangle bigger than all initial points
		vector< vector<double> > super_triangle{ {0, 0, 10000}, {-10000, 0, -1000}, {10000, -10000, -1000},{10000, 10000, -1000} };
		return super_triangle;
	} // Voronoi::def_supertriangle_3D
  
	template<typename vec_type> // vec_type: vector type for points
	vector< vector<vector<double>>> Voronoi<vec_type> ::watsons_alg_3D(vector< vector<double> >  sorted, vector< vector<double> >  super_triangle) {
		// initializing vectors for triangles and edges
		vector< vector<vector<double>> > triangle_list{ super_triangle };
		vector< vector<vector<double>> > final_triangles;
		//vector<vector<double>> edges;
		VectorFuncs vecf; //initialise vectorfunc class
		// loop over sorted points
		for (int s = 0; s < sorted.size(); s++) {
			// initialise first point and call "new"
			double x_new = sorted[s][0];
			double y_new = sorted[s][1];
			double z_new = sorted[s][2];
			vector<vector<double>> edges; // creating a vector with the edges for each new point
			// iterate through non-complete triangle list
			for (int itr = 0; itr < triangle_list.size(); itr++) {
        vector<vector<double>> t = triangle_list[itr]; // Triangle coordinates
        sort(t.begin(), t.end());
        // compute triangle circumcenter
        vector<vector<double>>  matrix = vecf.Det_Matrix(t);
        vector<vector<double>>  a {matrix[1], matrix[2], matrix[3], matrix[4]};
        double A = vecf.Determinant(a);
        vector<vector<double>> c {matrix[0], matrix[1], matrix[2], matrix[3]};
        double C = vecf.Determinant(c);
        vector<vector<double>> d_x {matrix[0], matrix[2], matrix[3], matrix[4]};
        vector<vector<double>> d_y {matrix[0], matrix[1], matrix[3], matrix[4]};
        vector<vector<double>> d_z {matrix[0], matrix[1], matrix[2], matrix[4]};
        double Dx = vecf.Determinant(d_x);
        double Dy = -1* vecf.Determinant(d_y);
        double Dz = vecf.Determinant(d_z);
        vector<double> circumcenter{ Dx/(2*A), Dy/(2*A), Dz/(2*A) }; // adding the circumcenter of tetrahedron as a vertex
        if( s== 0){ //super triangle should not be accepted, therefore add it as edges
          edges.push_back({ t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2] });
          edges.push_back({ t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2] });
          edges.push_back({ t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2] });
          edges.push_back({ t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2] });
          triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list and adding the four  edges to edges vector
          goto added_super_edges;
          }
        double R = pow((t[0][0] - circumcenter[0]), 2) + pow((t[0][1] - circumcenter[1]), 2) + pow((t[0][2] - circumcenter[2]), 2); // squared radius of circumcircle
        double D_x = pow((circumcenter[0]- x_new), 2); // square of x-distance from x_new to triangle circumcenter
        if (D_x > R ) { // checking if squared x-distance is bigger than squared radius, when true adding triangle to final triangles
        // -> point lies outside of triangle
          final_triangles.push_back(triangle_list[itr]);
          triangle_list.erase(triangle_list.begin() + itr);// erasing triangle from incomplete triangle list
          itr--; //to not miss a triangle
          }
				if (D_x < R) { // when x-distance is smaller than radius
					double  D = D_x + pow((circumcenter[1] - y_new), 2) + pow((circumcenter[2] - z_new), 2); // calculating distane from new point to circumcenter of triangle
					if (D < R) { // checking if distance is smaller than squared radius -> point lies within triangle -> cannot be a final triangle
						//cout << "adding edges\n";
						edges.push_back({ t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2] });
						edges.push_back({ t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2] });
						edges.push_back({ t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2] });
						edges.push_back({ t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2] });
						triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list and adding the four  edges to edges vector
						itr--;
					  }
				  }
			  }
        added_super_edges:
        sort(edges.begin(), edges.end()); // sort edges
        edges.erase(unique(edges.begin(), edges.end()), edges.end()); // erase edges that occur multiple times
			//from all edges create new triangles with the new point and add them as incomplete triangles
			for (int i = 0; i < edges.size(); i++) {
				vector< vector<double> > new_triangle{ {x_new,y_new, z_new} };
				new_triangle.push_back({ edges[i][0], edges[i][1], edges[i][2] });
				new_triangle.push_back({ edges[i][3], edges[i][4], edges[i][5] });
				new_triangle.push_back({ edges[i][6], edges[i][7], edges[i][8] });
				for (int a=0 ; a <4; a++){
				}
				sort(new_triangle.begin(), new_triangle.end());
				triangle_list.push_back(new_triangle);
			}
			
			if (triangle_list.size() > edges.size() ){ // check if triangles are left over from construction with last point
			for( int tri = 0 ; tri < triangle_list.size(); tri ++) { 
				for ( int next = tri + 1; next < triangle_list.size() ; next ++) {
					int counter = 0;
					for( int pts = 0 ; pts <4 ; pts++){// when yes, check if any of the triangles have three points in common -> a whole face in common
            if (triangle_list[tri][pts] == triangle_list[next][pts]){
            counter++;
            }
				  }
          if (counter == 3){ // when counter is 3, then the triangles do have a face in common
            vector<vector<double>> not_equal(2);
            vector<vector<double>> same(3);
            set_symmetric_difference(triangle_list[tri].begin(), triangle_list[tri].end(), triangle_list[next].begin(), triangle_list[next].end(), not_equal.begin()); // find which point do differ
            set_intersection(triangle_list[tri].begin(), triangle_list[tri].end(), triangle_list[next].begin(), triangle_list[next].end(), same.begin()); // find which three points are the same in the two triangles
            vector<double> vec1 {{same[1][0]- same[0][0]}, {same[1][1] - same[0][1]}, {same[1][2]  - same[0][2]} }; //construct a vector in the plane of the face of triangles
            vector<double> vec2 {{same[2][0]- same[0][0]}, {same[2][1] - same[0][1]}, {same[2][2]  - same[0][2]} }; // construct a vector in the plane of the face of triangles
            vector<double> normal = vecf.crossProduct(vec1, vec2); // calculating the normal of the plane
            double d_tri = normal[0]*(not_equal[0][0] - same[0][0]) + normal[1]*(not_equal[0][1] - same[0][1]) + normal[2]*(not_equal[0][2] - same[0][2]); // calculating distance of point from plane
            double d_next = normal[0]*(not_equal[1][0] - same[0][0]) + normal[1]*(not_equal[1][1] - same[0][1]) + normal[2]*(not_equal[1][2] - same[0][2]); // calculating distance of point from plane
            if (signbit(d_tri) == signbit(d_next)) { // if distance same sign then the points are on the same side of the plane, if opposite signs then on difference sides and both triangles are kept
              if ( abs(d_tri) > abs(d_next) ) { // checking which point is furthest away from plane
                triangle_list.erase(triangle_list.begin() + tri); // erase the bigger tetrahedron
              }
              if ( abs(d_tri) < abs(d_next) ) { // checking which point is furthest away from plane
                triangle_list.erase(triangle_list.begin() + next); // erasing the bigger tetrahedron
              }
             }
            }	
           }
          }
         }
        }
		// after all points have been added, add all triangles to the final list
		final_triangles.insert(final_triangles.end(), triangle_list.begin(), triangle_list.end());
		int itr = 0; // initialise counter
		while (itr < final_triangles.size() || final_triangles.size() == 0) { // execute while counter is smaller than length of final triangle list or as long as final triangle list is not zero
		  continue_here:
			// nested for loop to remove all triangles that include one or more of the points from the super triangle
			for (int i = 0; i < 4; i++) {
				for (int f = 0; f < 4; f++) {
					if (final_triangles[itr][f] == super_triangle[i]) { // when point from super triangle found, erase triangle
						final_triangles.erase(final_triangles.begin() + itr);
						if (final_triangles.size() == 0) { //if all final triangles include super triangle points exit while loop
							goto end_while_loop;
						}
						if (itr != 0) { //after removing a triangle decrease counter to not miss checking a triangle
							itr--;
						}
						goto continue_here; // after removing a triangle do not increase counter, therefore goto used
					}
				}
			}
			itr++; // increase couter to check next triangle
		}
	  end_while_loop:
		for (int i = 0; i < final_triangles.size(); i++) { //making sure no identical tetrahedrons are saved 
			sort(final_triangles[i].begin(), final_triangles[i].end());
		} 
		sort(final_triangles.begin(), final_triangles.end()); // sort triangles
		final_triangles.erase(unique(final_triangles.begin(), final_triangles.end()), final_triangles.end());//removing copies of triangles
		if( final_triangles.size() == 1){
		vector<vector<double>> odd(1);
		set_symmetric_difference(final_triangles[0].begin(),final_triangles[0].end(), sorted.begin(), sorted.end(), odd.begin());
		for( int p = 0; p <4; p++){
		vector<double> vec1 {{final_triangles[0][(p+1)%4][0] - final_triangles[0][(p+2)%4][0]}, {final_triangles[0][(p+1)%4][1] - final_triangles[0][(p+2)%4][1]}, {final_triangles[0][(p+1)%4][2] - final_triangles[0][(p+2)%4][2]}};
		vector<double> vec2 {{final_triangles[0][(p+3)%4][0] - final_triangles[0][(p+2)%4][0]}, {final_triangles[0][(p+3)%4][1] - final_triangles[0][(p+2)%4][1]}, {final_triangles[0][(p+3)%4][2] - final_triangles[0][(p+2)%4][2]}};
		vector<double> normal = vecf.crossProduct(vec1, vec2);
		double tetr = normal[0]*(final_triangles[0][p][0] - final_triangles[0][(p+1)%4][0]) + normal[1]*(final_triangles[0][p][1] - final_triangles[0][(p+1)%4][1]) +normal[2]*(final_triangles[0][p][2] - final_triangles[0][(p+1)%4][2]);
		double newodd = normal[0]*(odd[0][0] - final_triangles[0][(p+1)%4][0]) + normal[1]*(odd[0][1] - final_triangles[0][(p+1)%4][1]) + normal[2]*(odd[0][2] - final_triangles[0][(p+1)%4][2]);
		if(signbit(tetr) != signbit(newodd)){
		vector<vector<double>> new_triangle {odd[0]};
                new_triangle.push_back(final_triangles[0][(p+1)%4]);
		new_triangle.push_back(final_triangles[0][(p+2)%4]);
		new_triangle.push_back(final_triangles[0][(p+3)%4]);
		sort(new_triangle.begin(), new_triangle.end());
		final_triangles.push_back(new_triangle);
		break;
		}
		}
		}
		return final_triangles;	// returning vector of Delaunay triangles
	} // Voronoi::watsons_alg_3D 


	template<typename vec_type> // vec_type: vector type for points
	multimap<vector<double>,  vector<vector<vector<double>>>>  Voronoi<vec_type> ::getting_dual_graph_3D(vector< vector< vector<double> >>  triangles) {
		vector<vector<double>> vertex;
		VectorFuncs vecf; //initialising vectorfunc class
		for (int i = 0; i < triangles.size(); i++) {
			// calculating cicumcenters for all triangles
			vector<vector<double>>  matrix = vecf.Det_Matrix(triangles[i]);
			vector<vector<double>>  a {matrix[1], matrix[2], matrix[3], matrix[4]}; 	
			double A = vecf.Determinant(a);
			vector<vector<double>> D_x {matrix[0], matrix[2], matrix[3], matrix[4]};
		 	vector<vector<double>> D_y {matrix[0], matrix[1], matrix[3], matrix[4]};
			vector<vector<double>> D_z {matrix[0], matrix[1], matrix[2], matrix[4]};
			double Dx = vecf.Determinant(D_x);
			double Dy = -1* vecf.Determinant(D_y);
			double Dz = vecf.Determinant(D_z);
			vector<double> circumcenter{ Dx/(2*A), Dy/(2*A), Dz/(2*A) }; // adding the circumcenter of tetrahedron as a vertex
			vertex.push_back(circumcenter);
		}
		// checking that the circumcenters for all triangles were calculated
		if (vertex.size() != triangles.size()) throw invalid_argument(
			"Must be same dimensionality of vertex and triangles.");
			
	vector <vector <vector<double>> > inner_edges; // initialising vector with inner edges for voronoi cells
        vector <vector <vector<double>> > triangle_edges; // initialising vector with outer edges for voronoi cells
   	multimap<vector<vector<double>>, vector<vector<double>> > map;
		// adding edges
		for (int ver = 0; ver < vertex.size(); ver++){ // loop over vertex
			vector<vector<vector<double>>> faces;
			for (int next_v= 0; next_v< triangles.size(); next_v++) { // loop over triangles for not already checked vertex
				if( ver != next_v) {
				  vector<vector<double>> same_points;
				  set_intersection(triangles[ver].begin(), triangles[ver].end(), triangles[next_v].begin(), triangles[next_v].end(),back_inserter(same_points));
				  if (same_points.size() == 3){ //checking if triangles shares a face
        		sort(same_points.begin(), same_points.end());
       			faces.push_back(same_points); // save the face that the triangles share 
            vector< vector < double> >  edge_found{ {vertex[ver][0],vertex[ver][1],vertex[ver][2]}, { vertex[next_v][0],vertex[next_v][1],vertex[next_v][2]} };  // adding the edge connecting the two vertex of the triangles
            sort(edge_found.begin(), edge_found.end());  //sort the points for the edge
            vector<vector<double>> triangle_edge1 {same_points[0], same_points[1]};
            vector<vector<double>> triangle_edge2 {same_points[1], same_points[2]};
            vector<vector<double>> triangle_edge3 {same_points[0], same_points[2]};
            triangle_edges.insert(triangle_edges.end(), { triangle_edge1, triangle_edge2, triangle_edge3});
            map.insert({triangle_edge1, edge_found});
            map.insert({triangle_edge2, edge_found});
            map.insert({triangle_edge3, edge_found});
					  }
				}
			}
			//check if the vertex has any outer edges, when yes -> create 'outer vertexes' and construct the outer edges 
			if(faces.size() <4){
			// find how many outer edges the vertex should have- max is 3
			vector<vector<vector<double>>> outer_edges;
			vector<vector<vector<double>>> triangle_faces {{triangles[ver][0], triangles[ver][1], triangles[ver][2]}, {triangles[ver][1], triangles[ver][2], triangles[ver][3] }, {triangles[ver][2], triangles[ver][3], triangles[ver][0]},{ triangles[ver][3], triangles[ver][0], triangles[ver][1]}};
			set_symmetric_difference(triangle_faces.begin(), triangle_faces.end(), faces.begin(), faces.end(), back_inserter(outer_edges));
			for (int outer_ver= 0 ; outer_ver < outer_edges.size(); outer_ver++) {// if less than four edges found the tetrahedron is an outer cell, has to add corresponding number of edges
        vector<vector<double>> oe_ov = outer_edges[outer_ver]; 
        sort(oe_ov.begin(), oe_ov.end());
        vector<double> vec1 {{oe_ov[1][0]- oe_ov[0][0]}, { oe_ov[1][1] -  oe_ov[0][1]}, {oe_ov[1][2]  -  oe_ov[0][2]} }; //construct a vector in the plane of the face of triangles
        vector<double> vec2 {{ oe_ov[2][0]-  oe_ov[0][0]}, { oe_ov[2][1] -  oe_ov[0][1]}, { oe_ov[2][2]  -  oe_ov[0][2]} }; // construct a vector in the plane of the face of triangles
        vector<double> normal = vecf.crossProduct(vec1, vec2); // calculating the normal of the plane
        double k = (oe_ov[0][0]-vertex[ver][0])/normal[0];
        // finding the point on the opposite side of the face and add an edge between
	vector<double> new_vertex {2*(normal[0]*k + vertex[ver][0])- vertex[ver][0], 2*(normal[1]*k + vertex[ver][1])-vertex[ver][1], 2*(normal[2]*k + vertex[ver][2])-vertex[ver][2] }; // finding a point on the other side of the plane at the same distance
	vector<vector<double>>  edge_found{{ vertex[ver][0],vertex[ver][1],vertex[ver][2]},{ new_vertex[0], new_vertex[1], new_vertex[2]}}; // creating new edge between the vertex and new_vertex
	sort(edge_found.begin(), edge_found.end());
	//adding an outer edge
        vector<vector<double>> triangle_v1 { oe_ov[0],  oe_ov[1]};
        vector<vector<double>> triangle_v2 { oe_ov[1],  oe_ov[2]};
        vector<vector<double>> triangle_v3 { oe_ov[0],  oe_ov[2]};
        triangle_edges.insert(triangle_edges.end(), { triangle_v1, triangle_v2, triangle_v3});
        map.insert({triangle_v1, edge_found});
        map.insert({triangle_v2, edge_found});
        map.insert({triangle_v3, edge_found});
				}			
			}
		}
		
    // erasing copies of edges
    sort(triangle_edges.begin(), triangle_edges.end()); // sort edges
    triangle_edges.erase(unique(triangle_edges.begin(), triangle_edges.end()), triangle_edges.end());// erase copies
    multimap<vector<double>, vector<vector<double>>> map2;
    vector<vector<double>> voronoi_vertex;
    for(int edge = 0 ; edge < triangle_edges.size(); edge++){
      pair<multimap<vector<vector<double>>, vector<vector<double>>>::iterator,multimap<vector<vector<double>>, vector<vector<double>>>::iterator> face = map.equal_range(triangle_edges[edge]);
      vector<vector<double>> v_face;
      for (multimap<vector<vector<double>>, vector<vector<double>>>::iterator it = face.first ; it != face.second ; it++){
        vector<vector<double>> pts = it->second;
        v_face.insert(v_face.end(), pts.begin(), pts.end());
        }
      sort(v_face.begin(), v_face.end()); // sort points of voronoi face
      v_face.erase(unique(v_face.begin(), v_face.end()), v_face.end());// sort points of voronoi face
      //if voronoi face is an outer face add points
      if(v_face.size() < 4){
        if(v_face.size() == 3){
          vector<double> new_pt {-1*v_face[0][0], -1*v_face[0][1], -1*v_face[0][2]};
          v_face.push_back(new_pt);
          sort(v_face.begin(), v_face.end());
        }
        if(v_face.size() == 2){
          vector<double> new_pt0 {-1*v_face[0][0], -1*v_face[0][1], -1*v_face[0][2]};
          vector<double> new_pt1 {-1*v_face[1][0], -1*v_face[1][1], -1*v_face[1][2]};
          v_face.push_back(new_pt0);
          v_face.push_back(new_pt1);
          sort(v_face.begin(), v_face.end());
        }
      }
      voronoi_vertex.insert(voronoi_vertex.end(), triangle_edges[edge].begin(), triangle_edges[edge].end());
      map2.insert({triangle_edges[edge][0], v_face});
      map2.insert({triangle_edges[edge][1], v_face});
      }
      sort(voronoi_vertex.begin(), voronoi_vertex.end()); // sort vertex for voronoi cells
      voronoi_vertex.erase(unique(voronoi_vertex.begin(), voronoi_vertex.end()), voronoi_vertex.end());// sort vertex for voronoi cells
      for( int vertex = 0 ; vertex < voronoi_vertex.size(); vertex++ ){
        pair<multimap<vector<double>, vector<vector<double>>>::iterator, multimap<vector<double>, vector<vector<double>>>::iterator> same_v = map2.equal_range(voronoi_vertex[vertex]);
        vector<vector<vector<double>>> v_cell;
        for(multimap<vector<double>, vector<vector<double>>>::iterator it = same_v.first; it != same_v.second; it++){
          vector<vector<double>> face = it->second;
          v_cell.push_back(face);
        }
        sort(v_cell.begin(), v_cell.end()); // sort points in cell
	      v_cell.erase(unique(v_cell.begin(), v_cell.end()), v_cell.end());// erase copies
	      voronoi_cells.insert({voronoi_vertex[vertex], v_cell});
      }
	return voronoi_cells;
	}// Voronoi::getting_dual_graph_3D
    
  template<typename vec_type> // vec_type: vector type for points
  double  Voronoi<vec_type> ::weight_generator( vector<double> pts, vector<double> vertex_pts, vector<vector<double>> sorted_pts) {// pts value of point; vertex_pts value of vertex point 
    // generating Becke weights
    vector<vector<double>>:: iterator  n = find(sorted_pts.begin(), sorted_pts.end(), vertex_pts);
    int index_n = distance(sorted_pts.begin(), n);
    VectorFuncs vecf;
    double Denom = 0;
    // constructing denominator for weight function
    for(int m = 0 ; m < sorted_pts.size(); m++){  
      vector<double> ver_m = sorted_pts[m]; 
      int index_m = m;
      double Pm = 1.0;
       for(int j = 0 ; j < sorted_pts.size(); j++){ 
        vector<double> ver_j = sorted_pts[j];
        int index_j = j;
        if (index_j!=index_m){
	double mu2 = (vecf.Length(ver_m, pts)  - vecf.Length(ver_j,pts)) /vecf.Length(ver_m, ver_j);
        double p_m = (3/2)*((3/2)*((3/2)*mu2 -(1/2)*pow(mu2,3) ) -(1/2)*pow((3/2)*mu2 -(1/2)*pow(mu2,3),3)) -(1/2)*pow((3/2)*((3/2)*mu2 -(1/2)*pow(mu2,3) ) -(1/2)*pow((3/2)*mu2 -(1/2)*pow(mu2,3),3),3);
        Pm *=(1 - p_m)/2;
      }
    }
    Denom += Pm;
    }// Loop for denominator done
    // construct nominator
    double r_i = vecf.Length(vertex_pts, pts);
    double Pn =1.0;	
    for(int j= 0 ; j < sorted_pts.size(); j++){  
      vector<double> ver_j = sorted_pts[j];
      int index_j = j;
      if (index_j!=index_n){
      double mu = (r_i - vecf.Length(ver_j, pts)) /vecf.Length(vertex_pts, ver_j);
      double p_mu0 = (3/2)*((3/2)*((3/2)*mu -(1/2)*pow(mu,3) ) -(1/2)*pow((3/2)*mu -(1/2)*pow(mu,3),3)) -(1/2)*pow((3/2)*((3/2)*mu -(1/2)*pow(mu,3) ) -(1/2)*pow((3/2)*mu -(1/2)*pow(mu,3),3),3);
      Pn *=(1.0 - p_mu0)/2;
      }
    }// Loop for nominator done
    double w_n = Pn/Denom; // calc. weight
    return w_n;
  } //Voronoi::weight_generation
    

  template<typename vec_type> // vec_type: vector type for points
  void Voronoi<vec_type> ::construct_voronoi_3D(vec_type  x_pts, vec_type  y_pts, vec_type  z_pts){
    // Voronoi cell wrapper func
    vector<vector<double>> sorted_pts = sorting_3D(x_pts, y_pts, z_pts);
    vector<vector<double>> super_triangle = def_supertriangle_3D(sorted_pts);
    vector< vector<vector<double>>> delaunay = watsons_alg_3D(sorted_pts, super_triangle);
    voronoi_cells = getting_dual_graph_3D(delaunay);
  } //Voronoi::construct_voronoi_3D
  
   template<typename vec_type>
  multimap<vector<double>, double> Voronoi<vec_type> :: voronoi_weights(vector<vector<double>> sorted_pts, vector<vector<vector<double>>> gauss_cheb_pts, multimap<vector<double>, vector<double>> pts_in_cell){
    VectorFuncs vecf;
    vector<vector<double>> all_pts = gauss_cheb_pts[0];    
    vector<double> mm_x = vecf.min_max(all_pts[0]);
    vector<double> mm_y = vecf.min_max(all_pts[1]);
    vector<double> mm_z = vecf.min_max(all_pts[2]);
    double bs_radii =0.30; //approximation of the bragg-slater radii
    vector<double> weights;
    vector<vector<double>> points;
    vector<vector<double>> vertices;
    multimap<vector<double>, double> voronoi_wts;
    multimap<vector<double>, double> vertex_weight;
    multimap<vector<double>, vector<double>>::iterator iter;
    // starting iteration where a weight is computed for each point
    for (iter = pts_in_cell.begin(); iter != pts_in_cell.end(); ++iter) {
      vector<double> vertex = iter->first;
      vector<double> pts = iter->second;
      vertices.push_back(vertex);
      int n = distance(pts_in_cell.begin(), iter);
      double r_x = 0;
      if((mm_x[1] - pts[0]) != 0) r_x=(pts[0] - mm_x[0])/(mm_x[1]-pts[0]);
      double r_y = 0;
      if((mm_y[1] - pts[1]) != 0) r_y=(pts[1] - mm_y[0])/(mm_y[1]-pts[1]);
      double r_z = 0;
      if((mm_z[1] - pts[2]) != 0) r_z=(pts[2] - mm_z[0])/(mm_z[1]-pts[2]);
      vector<double> r_pts {r_x, r_y, r_z};
      transform(r_pts.begin(), r_pts.end(), r_pts.begin(), [bs_radii](double c){return c*bs_radii;});
      double weight = weight_generator(r_pts, vertex, sorted_pts);//*((gauss_cheb_pts[1][0][n]+gauss_cheb_pts[1][1][n]+gauss_cheb_pts[1][2][n])/3);// weight_generator
      vertex_weight.insert({vertex,weight});
      points.push_back(r_pts);
    }
   sort(vertices.begin(), vertices.end()); // sort vertices
   vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());// erase copies
   // normalising the weights
   for (int v =0; v< vertices.size(); v++){
     pair<multimap<vector<double>, double>::iterator, multimap<vector<double>, double>::iterator> range;
     range = vertex_weight.equal_range(vertices[v]);
     double sum1 = 0;
     for( multimap<vector<double>, double>::iterator it = range.first; it !=range.second ; ++it){
     sum1 = sum1 + it->second;
     }
     for( multimap<vector<double>, double>::iterator it = range.first; it !=range.second ; ++it){
	 it->second = it->second/sum1;
     }
   }
   for (multimap<vector<double>, double>::iterator it = vertex_weight.begin(); it != vertex_weight.end(); it++){
   weights.insert(weights.end(), it->second);
   }
   double sum2 = accumulate(weights.begin(), weights.end(), 0.0);
   transform(weights.begin(), weights.end(), weights.begin(), [sum2](double c){return c/sum2;});
   double sum3 = accumulate(weights.begin(), weights.end(), 0.0);
   // saving the weights and returning them 
   for( int i = 0; i< weights.size(); i++){
	voronoi_wts.insert({points[i], weights[i]});
   }
  return voronoi_wts;
  } //Voronoi::voronoi_weights

  //
  //end voronoi class
  //

  template<typename vec_type, typename n_dim>
  multimap<vector<double>, vector<double>> Grid3D<vec_type, n_dim> ::find_cell(vector<vector<vector<double>>> gauss_cheb_pts, int n_points, Voronoi<vec_type> &Voro,  vector<vector<double>> sorted_pts){
    VectorFuncs vecf;
    multimap<vector<double>, vector<double>> grid_p;
    multimap<vector<double>, vector<vector<vector<double>>>> voronoi_cells = Voro.get_vcells();
    //find cell grid point is located in
    //looping over all the generated grid points
    for( int p =0; p < n_points; p++){
      // creating a vector with the grid point:
      vector<double> gc_pts;
      gc_pts.push_back(gauss_cheb_pts[0][0][p]);
      gc_pts.push_back(gauss_cheb_pts[0][1][p]);
      gc_pts.push_back(gauss_cheb_pts[0][2][p]);
      // initialise smallest to inf use for comapring 
      double smallest = numeric_limits<double>::infinity();
      vector<double> cell_v; //use for saving vertex of smallest distance
    // looping through the voronoi cells to find in which one the point is located
    for(int s =0; s <sorted_pts.size(); s++){  
	vector<double> vertex = sorted_pts[s];
	double dis = vecf.Length(gc_pts, vertex);	
	if ( abs(dis) < smallest){
	smallest = abs(dis);
	//if(abs(dis) < (1.06)) cell_v = vertex; //used when introducing a cutoff
	cell_v = vertex;
	}
      }
      if(!cell_v.empty()) grid_p.insert({cell_v, gc_pts}); //checked that point was within boundaries -> point is in cell with vertex
      }
  return grid_p;
  }//Grid_3D::find_cell
  
  template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
  void Grid3D<vec_type, n_dim> :: set_voronoi_grid(vector<vector<vector<double>>> gauss_cheb_pts, multimap<vector<double>, vector<double>> pts_in_cell,  int n_points, n_dim ndim, Voronoi<vec_type> &Voro, vector<vector<double>> sorted_pts){
    //accessing the voronoi weights   
    multimap<vector<double>, double> voronoi_wts = Voro.get_vweights(gauss_cheb_pts, pts_in_cell, sorted_pts);
    mnpts = n_points;
    mndim = ndim;
    //if initialised - delete them, if  NULL generate new ones
    if (mpts || mwts) {
          delete[] mpts;
          delete[] mwts;
    }
    mpts = new double[mnpts * mndim];
    mwts = new double[mnpts];
    multimap<vector<double>, double>::iterator iter;
    // saving the grid points and weights as doubles
    for (iter = voronoi_wts.begin(); iter != voronoi_wts.end(); ++iter) {
      int n = distance(voronoi_wts.begin(), iter);
      vector<double> point = iter->first;
      double voro_weight = iter->second;
      for (size_t d = 0; d < mndim; d++) {
        mpts[n * mndim + d] = point[d];
      }
      mwts[n] = voro_weight;
  }
  }// Grid_3D::set_voronoi_grid
  
  template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
  void Grid3D<vec_type, n_dim> ::set_grid_wrapper(vec_type x_pts, vec_type y_pts, vec_type z_pts, int n_points, n_dim ndim){
    //wrapper foe constructing the Becke grid
    //construct chebyshev grid and voronoi cells
    vector<vector<vector<double>>> gauss_cheb_pts = gauss_cheb_3D(x_pts, y_pts, z_pts, n_points);
    Voronoi<vec_type> Voronoi_class;
    //Voronoi_class.construct_voronoi_3D(x_pts, y_pts, z_pts);
    vector<vector<double>> sorted_pts = Voronoi_class.sorting_3D(x_pts, y_pts, z_pts);
    // find which cell a  point is located in 
    multimap<vector<double>, vector<double>> pts_in_cell = find_cell(gauss_cheb_pts, n_points, Voronoi_class, sorted_pts);
    //setting up grid with becke weights and points
    set_voronoi_grid(gauss_cheb_pts, pts_in_cell, n_points, ndim, Voronoi_class, sorted_pts);
  } //Grid_3D::set_grid_wrapper
 
  //Becke grid end

  template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
  void Grid3D<vec_type, n_dim> ::set_ellgrid_wrapper(vector<vector<double>> center, vector<vector<vector<double>>> foci, vec_type x_pts, vec_type y_pts, vec_type z_pts, int n_points, n_dim ndim){
    // wrapper for constructing ellipsoidal grid
    vector<vector<double>> ell_pts =  ell_grid(center, foci, x_pts,y_pts,z_pts,n_points);
    Voronoi<vec_type> Voronoi_class;
    Voronoi_class.construct_voronoi_3D(x_pts, y_pts, z_pts);
    vector<vector<double>> sorted_pts = Voronoi_class.sorting_3D(x_pts, y_pts, z_pts);
    // find which cell point is located in
    vector<vector<vector<double>>> pts;
    pts.push_back(ell_pts); 
    multimap<vector<double>, vector<double>> pts_in_cell = find_cell(pts, n_points, Voronoi_class, sorted_pts);
    //setting up grid with becke weights and points
    set_voronoi_grid(pts, pts_in_cell, n_points, ndim, Voronoi_class, sorted_pts);
  } //Grid_3D::set_ellgrid_wrapper


 // initialize template classes:
  template class Grid3D<vector<double>, size_t>;
  template class Voronoi<vector<double>>; 

} //namespace libqqc

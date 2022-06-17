///
/// A 1D to 3D grid constructor
/// @file becke-grid.cpp
/// @author Isabel Vinterbladh
/// @version 0.1 19-04-2022
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

using namespace std;

namespace libqqc {

    //
    // 1D Grid functions 
    //
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    bool Grid1D<vec_type, n_dim> ::check_data_validity() {

        if (ndim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
       // if (coords_x == 0) throw invalid_argument(
           // "You need at least one point for the grid");
        //if (!mpts || !mwts) throw invalid_argument(
          //  "Your pts or wts pointer is NULL.");
        return true;
    } // Grid_1D::check_data_validity
    
    //Uniform grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<double>  Grid1D<vec_type, n_dim> ::set_grid(vec_type coords_x, n_dim ndim, int n_points) {
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

        for (int i = 0; i < n_points; i++) {
            unigrid[i] = min + iter * i;
        }
        return unigrid;
    } //Grid_1D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, nn_dim: dimension of grid point
    vector<double> Grid1D<vec_type, n_dim> ::set_weights(vec_type coords_x, n_dim ndim, int n_points) {
        double weight = 1.0 / n_points;
        vector<double> w_vec(n_points, weight);
        return w_vec;
    } //Grid_1D::set_weights
    //Uniform grid end
    
    //Gauss-Chebyshev grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<vector<double>> Grid1D<vec_type, n_dim> ::gauss_cheb(vec_type coords_x, int n_points) {
        vector<double> grid(n_points, 0.0);
        vector<double> weight(n_points, 0.0);
        double max = coords_x[0]; // finding interval of points, maximum
        double min = coords_x[0]; // finding interval of points, minimum
        //for loop to find max and min
        for (int i = 1; i < coords_x.size(); i++) {
            if (coords_x[i] < min) {
                min = coords_x[i];
            }
            if (coords_x[i] > max) {
                max = coords_x[i];
            }
        }
	//cout << "min " << min<< "\n";
	//cout << "max " << max << "\n";
        double pi = 3.1415926535897932384626434;
        double w = (pi*(max-min)) / n_points;
        // constructing the grid points with chebyshev points
        for (int i = 1; i < n_points; i++){
                double x_i = (min+max)/2 + ((max-min)/2)*cos(((2.0 * i - 1.0)*pi)/(2.0 * n_points));
		grid[i-1] = x_i;
		double w_i = 0;
		if((1- pow(x_i,2)) < 0) {w_i = w;} // if expression under square root is negative the weight becomes nan
		if((1- pow(x_i,2)) >= 0) {w_i  = w*sqrt(1 - pow(x_i,2));}
		weight[i-1] = w_i;
        }
        vector<vector<double>> result;
        result.push_back(grid);
        result.push_back(weight);
        return result;
    } //Grid_1D::gauss_cheb
    //Gauss-Chebyshev grid end
    
    //
    // 2D Grid functions 
    //
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    bool Grid2D<vec_type, n_dim> ::check_data_validity() {

        if (ndim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        //if (coords_x.size() == 0) throw invalid_argument(
        //    "You need at least one point for the grid");
        //if (coords_y.size() == 0) throw invalid_argument(
        //    "You need at least one point for the grid");
        //if (!mpts || !mwts) throw invalid_argument(
          //  "Your pts or wts pointer is NULL.");
        return true;
    } //Grid_2D::check_data_validity
    
    //Uniform grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<vector<double>> Grid2D<vec_type, n_dim> ::set_grid(vec_type coords_x, vec_type coords_y, n_dim ndim, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
        vector<double> grid_x = class_obj.set_grid(coords_x, ndim, n_points);
        vector<double> grid_y = class_obj.set_grid(coords_y, ndim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(grid_x);
        return_vec.push_back(grid_y);
        return return_vec;
    } //Grid_2D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<vector<double>> Grid2D<vec_type, n_dim> ::set_weights(vec_type coords_x, vec_type coords_y, n_dim ndim, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
        vector<double> w_x = class_obj.set_weights(coords_x, ndim, n_points);
        vector<double> w_y = class_obj.set_weights(coords_y, ndim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(w_x);
        return_vec.push_back(w_y);
        return return_vec;
    } //Grid_2D::set_weights
    //Uniform grid end
    
    //
    // 3D Grid functions 
    //
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    bool Grid3D<vec_type, n_dim> ::check_data_validity() {
        if (ndim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
        //if (coords_x.size() == 0) throw invalid_argument(
         //   "You need at least one point for the grid");
        //if (coords_y.size() == 0) throw invalid_argument(
        //    "You need at least one point for the grid");
        //if (coords_z.size() == 0) throw invalid_argument(
         //   "You need at least one point for the grid");
    	return true;
    } //Grid_3D::check_data_validity

    // Uniform grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of grid point
    vector<vector<double>> Grid3D<vec_type, n_dim> ::set_grid(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim ndim, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
        vector<double> grid_x = class_obj.set_grid(coords_x, ndim, n_points);
        vector<double> grid_y = class_obj.set_grid(coords_y, ndim, n_points);
        vector<double> grid_z = class_obj.set_grid(coords_z, ndim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(grid_x);
        return_vec.push_back(grid_y);
        return_vec.push_back(grid_z);
        return return_vec;
    } //Grid_3D::set_grid

    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    vector<vector<double>> Grid3D<vec_type, n_dim> ::set_weights(vec_type coords_x, vec_type coords_y, vec_type coords_z, n_dim ndim, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
        vector<double> w_x = class_obj.set_weights(coords_x, ndim, n_points);
        vector<double> w_y = class_obj.set_weights(coords_y, ndim, n_points);
        vector<double> w_z = class_obj.set_weights(coords_z, ndim, n_points);
        vector<vector<double>> return_vec;
        return_vec.push_back(w_x);
        return_vec.push_back(w_y);
        return_vec.push_back(w_z);
        return return_vec;
    } //Grid_3D::set_weights
    //Uniform grid end
    
    // Becke grid start
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    vector<vector<vector<double>>> Grid3D<vec_type, n_dim> ::gauss_cheb_3D(vec_type coords_x, vec_type coords_y, vec_type coords_z, int n_points) {
        Grid1D<vec_type, n_dim> class_obj;
        
        vector<double> xpts = class_obj.get_mpts(coords_x, n_points);
        vector<double> xwts = class_obj.get_mwts(coords_x, n_points);
      
        vector<double> ypts = class_obj.get_mpts(coords_y, n_points);
        vector<double> ywts = class_obj.get_mwts(coords_y, n_points);
    
        vector<double> zpts = class_obj.get_mpts(coords_z, n_points);
        vector<double> zwts = class_obj.get_mwts(coords_z, n_points);

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
   
  ///
  /// Voronoi class functions
  ///
  /// 2D functions
  /// 
	template<typename vec_type> // vec_type: vector type for points
	vector<vector<double>> Voronoi<vec_type> ::sorting(vec_type x_pts, vec_type y_pts) {
		if (x_pts.size() != y_pts.size()) throw invalid_argument(
			"Must be same dimensionality of x and y points.");

		// initializing variables
		double min_x = x_pts[0];
		double min_y = y_pts[0];
		int min_iter = 0;

		vector<double> sorted(2, 0); // vector with the sorted point
		vector<vector<double>> all_sort; // vector with all points sorted
		//Finding minimum of the x_pts points
		while (x_pts.size() > 0) {
			for (int i = 1; i < x_pts.size(); i++) {
				if (x_pts[i] < min_x) {
					min_x = x_pts[i];
					min_y = y_pts[i];
					min_iter = i;
				}
			}
			// saving the minimum found and then add to vector with all sorted points
			sorted[0] = min_x;
			sorted[1] = min_y;
			all_sort.push_back(sorted);
			// erase found minimum point from original points list
			x_pts.erase(x_pts.begin() + min_iter);
			y_pts.erase(y_pts.begin() + min_iter);

			// setting new initial values for variables
			if (x_pts.size() != 0) {
				min_x = x_pts[0];
				min_y = y_pts[0];
				min_iter = 0;
			}

		}
		return all_sort; //returning vector with sorted points
	} // Voronoi::sorting


	template<typename vec_type> // vec_type: vector type for points
	vector<vector<double>> Voronoi<vec_type> ::def_supertriangle(vector<vector<double>> sorted) {
		double min_y = sorted[0][1];
		double max_y = sorted[0][1];
		// Finding the maximum and minimum of y to be able to construct the super triangle
		for (int i = 1; i < sorted.size(); i++) {
			if (sorted[i][1] < min_y) {
				min_y = sorted[i][1];
			}
			if (sorted[i][1] > max_y) {
				max_y = sorted[i][1];
			}
		}
		// adding the points (-2*min x, max y - min y),(2*max x, -2 * min y), (2*max x, 2* max y)
		vector< vector<double> > super_triangle{ {-30 * sorted[0][0],(max_y - min_y)}, {10 * sorted.back()[0], -30* min_y}, {10 * sorted.back()[0], 10 * max_y} };
		// add test to see that points are in super triangle
		return super_triangle;
	} // Voronoi::def_supertriangle

	template<typename vec_type> // vec_type: vector type for points
	vector< vector<vector<double>>> Voronoi<vec_type> ::watsons_alg(vector< vector<double> > sorted, vector< vector<double> > super_triangle) {
		// initializing vectors for triangles and edges
		vector< vector<vector<double>> > triangle_list{ super_triangle };
		vector< vector<vector<double>> > final_triangles;
		vector<vector<double>> edges;
		// loop over sorted points
		for (int s = 0; s < sorted.size(); s++) {
			// initialise first point and call "new"
			double x_new = sorted[s][0];
			double y_new = sorted[s][1];
			// iterate through non-complete triangle list
			for (int itr = 0; itr < triangle_list.size(); itr++) {
				// compute triangle circumcenter
				vector<vector<double>> t = triangle_list[itr]; // Triangle coordinates
				double slope_1 = (t[1][1] - t[0][1]) / (t[1][0] - t[0][0]); // calculating slope of triangle edge
				double slope_2 = (t[2][1] - t[1][1]) / (t[2][0] - t[1][0]); // calculating slope of triangle edge
		
				if (t[0][0] == t[1][0] || t[1][1] == t[0][1]) {
				slope_1 = 1; // calculating slope of triangle edge
				} 
				if (t[1][0] == t[2][0] || t[2][1] == t[1][1]) {
				slope_2 = 1; // calculating slope of triangle edge
				}
				vector<double> mid_p1{ (t[0][0] + t[1][0]) / 2, (t[0][1] + t[1][1]) / 2 }; // constructing vector of midpoints on edge
				vector<double> mid_p2{ (t[1][0] + t[2][0]) / 2, (t[1][1] + t[2][1]) / 2 }; // constructing vector of midpoints on edge
				double c_x = ((slope_1/ slope_2) * mid_p2[0] - slope_1* (mid_p1[1] - mid_p2[1])) / (1 - (slope_1 / slope_2)); // circumcenter x point
				double c_y = (1/slope_1) * (c_x - mid_p1[0]) + mid_p1[1]; // circumcenter y point
				double R = pow((t[0][0] - c_x), 2) + pow((t[0][1] - c_y), 2); // squared radius of circumcircle
				double D_x = pow((c_x - x_new), 2); // square of x-distance from x_new to triangle circumcenter
				if (D_x >= R) { // checking if squared x-distance is bigger than squared radius, when true adding triangle to final triangles
				// -> point lies outside of triangle
					final_triangles.push_back(triangle_list[itr]);
					triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list
					//cout << "triangle added \n";
				} 
				// when x-distance is smaller than radius
				double D = sqrt(D_x + pow((c_y - y_new), 2)); // calculating distane from new point to circumcenter of triangle
				if (D < R) { // checking if distance is smaller than squared radius -> point lies within triangle -> cannot be a final triangle
					triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list and adding the three edges to edges vector
					//cout << "edges added \n";
					edges.push_back({ t[0][0], t[0][1], t[1][0], t[1][1] });
					edges.push_back({ t[1][0], t[1][1], t[2][0], t[2][1] });
					edges.push_back({ t[2][0], t[2][1], t[0][0], t[0][1] });
				}
			}
			
			cout << edges.size();
			sort(edges.begin(), edges.end()); // sort edges
			edges.erase(unique(edges.begin(), edges.end()), edges.end()); // erase edges that occur multiple times
			cout << ":" << edges.size() << "\n";
				//from all edges create new triangles with the new point and add them as incomplete triangles
			for (int i = 0; i < edges.size(); i++) {
				vector< vector<double> > new_triangle{ {x_new,y_new} };
				new_triangle.push_back({ edges[i][0], edges[i][1] });
				new_triangle.push_back({ edges[i][2], edges[i][3] });
				triangle_list.push_back(new_triangle);
			}
			cout << triangle_list.size() <<" :" << final_triangles.size() << "\n";
		}
		int itr = 0; // initialise counter
		while (itr < final_triangles.size() || final_triangles.size() == 0) { // execute while counter is smaller than length of final triangle list or as long as final triangle list is not zero
		continue_here:
			// nested for loop to remove all triangles that include one or more of the points from the super triangle
			for (int i = 0; i < 3; i++) {
				for (int f = 0; f < 3; f++) {
					if (final_triangles[itr][f] == super_triangle[i]) { // when point from super triangle found, erase triangle
						
						final_triangles.erase(final_triangles.begin() + itr);
						cout << final_triangles.size() << "\n";
						if (final_triangles.size() == 0) { //if all final triangles include super triangle points exit while loop
							goto end_while_loop;
						}
						if (itr != 0) { //after removing a triangle decrease counter to not miss checking a triangle
							itr--;
						}
						goto continue_here; // aftet removing a triangle do not increase counter, therefore goto used
					}
				}
			}
			itr++; // increase couter to check next triangle
		}
	end_while_loop:
		return final_triangles;	// returning vector of Delaunay triangles
	} // Voronoi::watsons_alg 

	template<typename vec_type> // vec_type: vector type for points
	vector<vector<vector<double>>> Voronoi<vec_type> ::getting_dual_graph(vector< vector< vector<double> >> triangles) {
		vector<vector<double>> vertex;

		for (int i = 0; i < triangles.size(); i++) {
			vector<vector<double>> t = triangles[i]; // Triangle coordinates
			double slope_1 = (t[1][1] - t[0][1]) / (t[1][0] - t[0][0]); // calculating slope of triangle edge
			double slope_2 = (t[2][1] - t[1][1]) / (t[2][0] - t[1][0]); // calculating slope of triangle edge
			vector<double> mid_p1{ (t[0][0] + t[1][0]) / 2, (t[0][1] + t[1][1]) / 2 }; // constructing vector of midpoints on edge
			vector<double> mid_p2{ (t[1][0] + t[2][0]) / 2, (t[1][1] + t[2][1]) / 2 }; // constructing vector of midpoints on edge
			double c_y = ((-1 / slope_2) * (mid_p1[0] - mid_p2[0]) + mid_p2[1] - (slope_1 / slope_2) * mid_p1[1]) / (1 - (slope_1 / slope_2)); // circumcenter y point
			double c_x = -slope_1 * (c_y - mid_p1[1]) + mid_p1[0]; // circumcenter x point
			vector<double> circumcenter{ c_x, c_y };
			cout << circumcenter[0] << circumcenter[1] << "\n";
			vertex.push_back(circumcenter);
		}
		// checking that the circumcenters for all triangles were calculated
		if (vertex.size() != triangles.size()) throw invalid_argument(
			"Must be same dimensionality of vertex and triangles.");

		vector< vector< vector<double> >> voronoi_cells; // initialising vector with edges for voronoi cells

		for (int j = 0; j < vertex.size(); j++) { // loop over vertex
			for (int k = j + 1; k < triangles.size(); k++) { // loop over triangles for not already checked vertex
				for (int edge = 0; edge < 3; edge++) { // loop over the three edges of triangle
					if (triangles[j][0] == triangles[k][edge] || triangles[j][1] == triangles[k][edge] || triangles[j][2] == triangles[k][edge]) { // if an edge is the same in two triangles, connect the vertex
						vector<vector < double>> edge_found{ vertex[j], vertex[k] };  // adding the edge connecting the two vertex of the triangles
						voronoi_cells.push_back(edge_found); // save edge as part of voronoi cells
						break;
					}
				}
			}
		}
		//adding outer vertices
		for (int i = 0; i < vertex.size(); i++) {
			int counter = 0;
			for (int j = 0; j < voronoi_cells.size(); j++) {
				counter = counter + count(voronoi_cells[j].begin(), voronoi_cells[j].end(), vertex[i]);
				if (counter == 3) {
					break;
				}
			}
			if (counter < 3) {
				voronoi_cells.push_back({ vertex[i], {10 * vertex[i][0],10 * vertex[i][1]} });
			}
		}

		return voronoi_cells;
	}// Voronoi::getting_dual_graph

  ///
  /// Voronoi class functions
  /// 
  /// 3D functions
  ///
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
		vector< vector<double> > super_triangle{ {0, 0, 10000}, {-10000, 0, -1000}, {10000, -10000, -1000},{10000, 10000, -1000} };
		// add test to see that all points are in super triangle
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
                //cout << Dx << "," << Dy <<"," << Dz << "\n";
                	vector<double> circumcenter{ Dx/(2*A), Dy/(2*A), Dz/(2*A) }; // adding the circumcenter of tetrahedron as a vertex
                //cout << circumcenter[0] << "," << circumcenter[1] << "," << circumcenter[2] << "\n";
        		//cout << "a:" << A << "\n";
			if( s== 0){ //super triangle should not be accepted, therefore add it as edges
			edges.push_back({ t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2] });
      			edges.push_back({ t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2] });
			edges.push_back({ t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2] });
			edges.push_back({ t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2] });
			triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list and adding the four  edges to edges vector
			goto added_super_edges;
			}
				double R = pow((t[0][0] - circumcenter[0]), 2) + pow((t[0][1] - circumcenter[1]), 2) + pow((t[0][2] - circumcenter[2]), 2); // squared radius of circumcircle
				double D_x = pow((circumcenter[1]- y_new), 2); // square of x-distance from x_new to triangle circumcenter
				//cout <<  R << ":" << D_x <<  "\n";
				if (D_x > R ) { // checking if squared x-distance is bigger than squared radius, when true adding triangle to final triangles
				// -> point lies outside of triangle
					  //cout << "added triangle \n";
						final_triangles.push_back(triangle_list[itr]);
						triangle_list.erase(triangle_list.begin() + itr);// erasing triangle from incomplete triangle list
						itr--; //to not miss a triangle
				}
				
				if (D_x < R) { // when x-distance is smaller than radius
					double  D = D_x + pow((circumcenter[1] - y_new), 2) + pow((circumcenter[2] - z_new), 2); // calculating distane from new point to circumcenter of triangle
					//cout << "if D smaller than R:"<< D << ":" << R << "\n";
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
			//cout << "edges:" << edges.size() << "\n";
			//from all edges create new triangles with the new point and add them as incomplete triangles
			for (int i = 0; i < edges.size(); i++) {
				vector< vector<double> > new_triangle{ {x_new,y_new, z_new} };
				new_triangle.push_back({ edges[i][0], edges[i][1], edges[i][2] });
				new_triangle.push_back({ edges[i][3], edges[i][4], edges[i][5] });
				new_triangle.push_back({ edges[i][6], edges[i][7], edges[i][8] });
				sort(new_triangle.begin(), new_triangle.end());
				triangle_list.push_back(new_triangle);
			}
			//cout << "triangles:" << triangle_list.size() <<":" << final_triangles.size() << "\n";
			
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
						//cout << "erasing " << final_triangles[itr][f][0] << ";" << final_triangles[itr][f][1] << ";" << super_triangle[i][0] << ";" << super_triangle[i][1] << ";" << "\n";
						final_triangles.erase(final_triangles.begin() + itr);
						if (final_triangles.size() == 0) { //if all final triangles include super triangle points exit while loop
							goto end_while_loop;
						}
						if (itr != 0) { //after removing a triangle decrease counter to not miss checking a triangle
							itr--;
						}
						goto continue_here; // afte removing a triangle do not increase counter, therefore goto used
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

		return final_triangles;	// returning vector of Delaunay triangles
	} // Voronoi::watsons_alg_3D 


	template<typename vec_type> // vec_type: vector type for points
	multimap<vector<double>,  vector<vector<double>>>  Voronoi<vec_type> ::getting_dual_graph_3D(vector< vector< vector<double> >>  triangles) {
		vector<vector<double>> vertex;
		VectorFuncs vecf; //initialising vectorfunc class
		for (int i = 0; i < triangles.size(); i++) {
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
			//cout << circumcenter[0] << "," << circumcenter[1] << "," << circumcenter[2] << "\n";
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
				//inner_edges.push_back(edge_found); // save edge as part of voronoi cells
				}
				}
			}
			//check if the vertex has any outer edges, when yes -> create 'outer vertexes' and construct the outer edges 
			if(faces.size() <4){
			// find how many outer edges the vertex should have- max is 3
			vector<vector<vector<double>>> outer_edges;
			vector<vector<vector<double>>> triangle_faces {{triangles[ver][0], triangles[ver][1], triangles[ver][2]}, {triangles[ver][1], triangles[ver][2], triangles[ver][3] }, {triangles[ver][2], triangles[ver][3], triangles[ver][0]},{ triangles[ver][3], triangles[ver][0], triangles[ver][1]}};
			set_symmetric_difference(triangle_faces.begin(), triangle_faces.end(), faces.begin(), faces.end(), back_inserter(outer_edges));
			//cout << "check if 4 is found:"<< faces.size() <<"+ "<< outer_edges.size()<< "\n";	
			for (int outer_ver= 0 ; outer_ver < outer_edges.size(); outer_ver++) {// if less than four edges found the tetrahedron is an outer cell, has to add corresponding number of edges
				      vector<vector<double>> oe_ov = outer_edges[outer_ver]; 
				      sort(oe_ov.begin(), oe_ov.end());
				      vector<double> vec1 {{oe_ov[1][0]- oe_ov[0][0]}, { oe_ov[1][1] -  oe_ov[0][1]}, {oe_ov[1][2]  -  oe_ov[0][2]} }; //construct a vector in the plane of the face of triangles
        			      vector<double> vec2 {{ oe_ov[2][0]-  oe_ov[0][0]}, { oe_ov[2][1] -  oe_ov[0][1]}, { oe_ov[2][2]  -  oe_ov[0][2]} }; // construct a vector in the plane of the face of triangles
        			      vector<double> normal = vecf.crossProduct(vec1, vec2); // calculating the normal of the plane
				      double k = ( oe_ov[0][0]-vertex[ver][0])/normal[0];
				// finding the point on the opposite side of the face and add an edge between
				vector<double> new_vertex {2*(normal[0]*k + vertex[ver][0])- vertex[ver][0], 2*(normal[1]*k + vertex[ver][1])-vertex[ver][1], 2*(normal[2]*k + vertex[ver][2])-vertex[ver][2] }; // finding a point on the other side of the plane at the same distance
				vector<vector<double>>  edge_found{{ vertex[ver][0],vertex[ver][1],vertex[ver][2]},{ new_vertex[0], new_vertex[1], new_vertex[2]}}; // creating new edge between the vertex and new_vertex
				//cout << "new edge" << vertex[ver][0]<<","<< vertex[ver][1] <<"," << vertex[ver][2] <<","<< new_vertex[0] <<","<< new_vertex[1]<<","<< new_vertex[2]<< "\n";
				sort(edge_found.begin(), edge_found.end());
				//adding an outer edge
        			vector<vector<double>> triangle_v1 { oe_ov[0],  oe_ov[1]};
        			vector<vector<double>> triangle_v2 { oe_ov[1],  oe_ov[2]};
        			vector<vector<double>> triangle_v3 { oe_ov[0],  oe_ov[2]};
        			triangle_edges.insert(triangle_edges.end(), { triangle_v1, triangle_v2, triangle_v3});
        			map.insert({triangle_v1, edge_found});
        			map.insert({triangle_v2, edge_found});
        			map.insert({triangle_v3, edge_found});
				//o_edges.push_back(edge_found);
				}			
			}
		}
		
	// erasing copies of edges
	sort(triangle_edges.begin(), triangle_edges.end()); // sort edges
	triangle_edges.erase(unique(triangle_edges.begin(), triangle_edges.end()), triangle_edges.end());// erase copies
	//for( int i =0; i < triangle_edges.size(); i++){
	//	for(int j=1; j < triangle_edges.size(); j++){
	//	if( (triangle_edges[i][0] == triangle_edges[j][0] & triangle_edges[i][1] == triangle_edges[j][1]) ||  (triangle_edges[i][1] == triangle_edges[j][0] & triangle_edges[i][0] == triangle_edges[j][1])){
	//	triangle_edges.erase(triangle_edges.begin() + j);
	//	j--;		
	//	}
	//}
	//}	
	// finding faces
	//vector <vector <vector<double>> > voronoi_faces;	
    	//cout <<"map size" << map.size() << "\n";
	//for( int t = 0; t < triangle_edges.size(); t++){
	//	for( int tt = 0; tt < triangle_edges[t].size(); tt++){
	// 	cout << triangle_edges[t][tt][0] << ":" << triangle_edges[t][tt][1] << ";" << triangle_edges[t][tt][2]<< "\n";
	//}}
	//cout <<"triangle edges size"<< triangle_edges.size();
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
	//cout << "printing points in face \n";
	//for( int i =0; i <v_face.size(); i++){
	// cout << v_face[i][0] << ";" << v_face[i][1] << ";" << v_face[i][2] << "\n";
	//}
	//cout << "v_face size before:" << v_face.size()<< "\n";
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
	//cout << "v_face size:" << v_face.size()<< "\n";
        voronoi_vertex.insert(voronoi_vertex.end(), triangle_edges[edge].begin(), triangle_edges[edge].end());
        map2.insert({triangle_edges[edge][0], v_face});
        map2.insert({triangle_edges[edge][1], v_face});
      }

      sort(voronoi_vertex.begin(), voronoi_vertex.end()); // sort vertex for voronoi cells
      voronoi_vertex.erase(unique(voronoi_vertex.begin(), voronoi_vertex.end()), voronoi_vertex.end());// sort vertex for voronoi cells
      multimap<vector<double>,  vector<vector<double>>> voronoi_cells;
      for( int vertex = 0 ; vertex < voronoi_vertex.size(); vertex++ ){
        pair<multimap<vector<double>, vector<vector<double>>>::iterator, multimap<vector<double>, vector<vector<double>>>::iterator> same_v = map2.equal_range(voronoi_vertex[vertex]);
        vector<vector<double>> v_cell;
        for(multimap<vector<double>, vector<vector<double>>>::iterator it = same_v.first; it != same_v.second; it++){
          vector<vector<double>> cell = it->second;
          v_cell.insert(v_cell.end(), cell.begin(), cell.end());
          }
          sort(v_cell.begin(), v_cell.end()); // sort points in cell
	  v_cell.erase(unique(v_cell.begin(), v_cell.end()), v_cell.end());// erase copies
	  voronoi_cells.insert({voronoi_vertex[vertex], v_cell});
        }

	return voronoi_cells;
	 }// Voronoi::getting_dual_graph_3D
    
  template<typename vec_type> // vec_type: vector type for points
  vector<double> Voronoi<vec_type> ::weight_generation(vec_type  pts) {
        int n_points = pts.size(); 
        vector<double> weights(n_points, 0.0);
        double max = pts[0]; // finding interval of points, maximum
        double min = pts[0]; // finding intervla of points, minimum
        //for loop to find max and min
        for (int i = 1; i < n_points; i++) {
            if (pts[i] < min) {
                min = pts[i];
            }
            if (pts[i] > max) {
                max = pts[i];
            }
        }
        double pi = 3.1415926535897932384626434;
        double w = (pi*(max-min)) / n_points;
     
        // constructing the grid points with chebyshev points
        for (int i = 0; i < n_points; i++){
                weights[i] = w*sqrt(1 - pow(pts[i],2));
        }
        return weights;
    } //Voronoi::weight_generation
    
  template<typename vec_type>
  map<vector<double>, vector<double>> Voronoi<vec_type> :: voronoi_weights(vec_type  x_pts, vec_type  y_pts, vec_type  z_pts){
      //Voronoi<vec_type> Voro_class;
      vector<double> weights_x = weight_generation(x_pts);
      vector<double> weights_y = weight_generation(y_pts);
      vector<double> weights_z = weight_generation(z_pts);
      int n_points = x_pts.size(); 
      map<vector<double>, vector<double>> vertex_weights;
      for (int i=0; i <n_points; i++){
        vector<double> key {x_pts[i], y_pts[i], z_pts[i]};
        vector<double> value {weights_x[i], weights_y[i], weights_z[i]};
        vertex_weights.insert({key, value});
        }
      return vertex_weights;
      }


  template<typename vec_type> // vec_type: vector type for points
  void Voronoi<vec_type> ::construct_voronoi_3D(vec_type  x_pts, vec_type  y_pts, vec_type  z_pts){
    //Voronoi<vec_type> Voro_class;
    vector<vector<double>> sorted_pts = sorting_3D(x_pts, y_pts, z_pts);
    vector<vector<double>> super_triangle = def_supertriangle_3D(sorted_pts);
    vector< vector<vector<double>>> delaunay = watsons_alg_3D(sorted_pts, super_triangle);
    voronoi_cells = getting_dual_graph_3D(delaunay);
    vertex_weights = voronoi_weights(x_pts, y_pts, z_pts); // voronoi cell weights
  }
//end voronoi class
   
    template<typename vec_type, typename n_dim>
    multimap<vector<double>, vector<double>> Grid3D<vec_type, n_dim> ::find_cell(vector<vector<vector<double>>> gauss_cheb_pts, int n_points, Voronoi<vec_type> &Voro){
      //Voronoi<vec_type> Voro;
      VectorFuncs vecf;
      multimap<vector<double>, vector<double>> grid_p;
      multimap<vector<double>, vector<vector<double>>> voronoi_cells = Voro.get_vcells();
     // map<vector<double>, vector<double>> vertex_weights;
     // cout << "initialisations\n";
      for( int p =0; p < n_points; p++){
	//new_point:
        vector<double> gc_pts;
        gc_pts.push_back(gauss_cheb_pts[0][0][p]);
        gc_pts.push_back(gauss_cheb_pts[0][1][p]);
        gc_pts.push_back(gauss_cheb_pts[0][2][p]);
	//cout <<"loaded new point\n";
        for( multimap<vector<double>, vector<vector<double>>>::iterator itr = voronoi_cells.begin(); itr !=voronoi_cells.end(); itr++){
	  test_new_cell:
	  vector<vector<double>> cell = itr->second;
	  //cout << "assinged cell to check\n";
          for( int c = 0; c < 3; c++){ //looping over first x coordinated, then y coordinates and at last z coordinates
	  	double min = cell[0][c];
		double max = cell[0][c];
		//cout << "assigned min and max \n";
	  	for (int i = 1; i < cell.size(); i++){
	        if(cell[i][c] < min) min = cell[i][c];
	      	if(cell[i][c] > max) max = cell[i][c];
		}
		//cout << "found min and max \n"<< min << ";" << max << "\n";
		if(gc_pts[c] < min  || gc_pts[c] > max){
		//cout << "point which is smaller/bigger \n" << gc_pts[c];
		//if(itr == voronoi_cells.end()){
		//p++;
		//goto new_point;
		itr++;
                goto test_new_cell;
		}
		}
	      //vector<double> vec1 {{cell[i][0][0]- cell[i][1][0]}, {cell[i][0][1] - cell[i][1][1]}, {cell[i][0][2]  - cell[i][1][2]} }; //construct a vector in the plane of the face of triangles
              //vector<double> vec2 {{cell[i][0][0]- cell[i][2][0]}, {cell[i][0][1] - cell[i][2][1]}, {cell[i][0][2]  - cell[i][2][2]} }; //construct a vector in the plane of the face of triangles
              //vector<double> normal = vecf.crossProduct(vec1, vec2); // calculating the normal of the plane
              //double check_sign = (normal[0]*(gc_pts[0] - cell[i][0][0]) + normal[1]*(gc_pts[1] - cell[i][0][1]) + normal[2]*(gc_pts[2] - cell[i][0][2]))/(sqrt(pow(normal[0],2) + pow(normal[1], 2) + pow(normal[2], 2))); // calculating distance of point from plane, to check if point is in cell
              //cout<<"sign:" <<  signbit(check_sign) << "\n";
	  //    if (signbit(check_sign)==1){
	//	itr++;
	//	cout << "entered break \n";
	//	goto test_new_cell; //if sign is negative, break! 
             // }
	      //cout << "found cell\n";
              vector<double> vertex = itr->first;
              grid_p.insert({vertex, gc_pts}); //checked that point distance was positive to all faces -> point is in cell with vertex
              //cout << "inserted vertex and point\n"; 
	  }
        }
        return grid_p;
      }//Grid_3D::find_cell
    
    //template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    //void Grid_3D<vec_type, n_dim> ::voronoi_grid_3D(vec_type coords_x, vec_type coords_y, vec_type coords_z, int n_points) {
     // Grid_3D<vec_type, n_dim> class_3D;
     // gauss_cheb_pts = gauss_cheb_3D(coords_x, coords_y, coords_z, n_points);
      
      //Voronoi<vec_type> Voronoi_class;
      //Voronoi_class.construct_voronoi_3D(coords_x, coords_y, coords_z);
      
      // find which cell point is located in 
     // pts_in_cell = find_cell(class_3D.gauss_cheb_pts, n_points);
      //class_3D.gauss_cheb_pts = *gauss_cheb_pts;
    //}//Grid_3D::voronoi_grid_3D
    
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    void Grid3D<vec_type, n_dim> :: set_voronoi_grid(vector<vector<vector<double>>> gauss_cheb_pts, multimap<vector<double>, vector<double>> pts_in_cell,  int n_points, n_dim ndim, Voronoi<vec_type> &Voro){
      //Voronoi<vec_type> Voro;
      //save number of points and dimension to be accesible
      //multimap<vector<double>, vector<vector<vector<double>>>> voronoi_cells;
      map<vector<double>, vector<double>> vertex_weights = Voro.get_vweights();

      mnpts = n_points;
      mndim = ndim;
      // NULL generate new ones
      if (mpts || mwts) {
            delete[] mpts;
            delete[] mwts;
        }
      mpts = new double[n_points * ndim];    
      mwts = new double[n_points * ndim];
      vector<vector<double>> grid_pts = gauss_cheb_pts[0];
      vector<vector<double>> grid_wts = gauss_cheb_pts[1];
      //loop over all points and dimensions and copy the data
      int counter = 0;
      multimap<vector<double>, vector<double>>::iterator iter;
      for (iter = pts_in_cell.begin(); iter !=  pts_in_cell.end(); iter++) {
      //for ( int i = 0; i < grid_pts.size(); i++){      
	      vector<double> vertex = iter-> first;
              vector<double> point = iter -> second;
              map<vector<double>, vector<double>>::iterator voronoi_w = vertex_weights.find(vertex); //getting the weight for the voronoi cell which the point is located in
              vector<double> voro_weight = voronoi_w->second;
	      mpts[counter * ndim + 0] = point[0];
              mpts[counter * ndim + 1] = point[1];
              mpts[counter * ndim + 2] = point[2];
              mwts[counter * ndim + 0] = grid_wts[0][counter]*voro_weight[0];
              mwts[counter * ndim + 1] = grid_wts[1][counter]*voro_weight[1];
              mwts[counter * ndim + 2] = grid_wts[2][counter]*voro_weight[2];
	      counter++;
          }
    //cout << "pointer to pts" << mpts[0];
    //cout << "pointer to pts" << *mwts;
    }// Grid_3D::set_voronoi_grid
    
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    void Grid3D<vec_type, n_dim> ::set_grid_wrapper(vec_type coords_x, vec_type coords_y, vec_type coords_z, int n_points, n_dim ndim){
       
     // Grid_3D<vec_type, n_dim> class_3D;
      vector<vector<vector<double>>> gauss_cheb_pts = gauss_cheb_3D(coords_x, coords_y, coords_z, n_points);
     // multimap<vector<double>, vector<vector<vector<double>>>> voronoi_cells;
     // map<vector<double>, vector<double>> vertex_weights;
      cout << "chebyshev points set\n"; 
      Voronoi<vec_type> Voronoi_class;
      Voronoi_class.construct_voronoi_3D(coords_x, coords_y, coords_z);
      cout <<"voronoi class done\n";
      // find which cell point is located in 
      multimap<vector<double>, vector<double>> pts_in_cell = find_cell(gauss_cheb_pts, n_points, Voronoi_class);
      cout<< "found cells for points\n";
      set_voronoi_grid(gauss_cheb_pts, pts_in_cell, n_points, ndim, Voronoi_class);
      cout <<"set grid for pts\n";
    } //Grid_3D::set_grid_wrapper
   
    //Becke grid end

 // initialize template classes:
  template class Grid3D<vector<double>, size_t>;
  template class Voronoi<vector<double>>; 


//    int main(){
//      vector<double> x { -1.04056,(-1.04056+0.07267)/2, -0.07267,(-0.07267+1.31941)/2 ,-1.31941};
//      vector<double> y { 2.23373,(2.23373-2.19278)/2 ,2.19278,(2.19278-1.41547)/2 ,1.41547};
//      vector<double> z { -0.00942, (-0.00942-0.01248)/2,0.01248, (0.01248-0.43837)/2 ,0.43837};
//     cout<< "test\n";
//      int n_points= 10;
//      size_t dim =3;
//      Grid_3D<vector<double>, size_t> Grid;
//      auto R = Grid.gauss_cheb_3D(x, y, z, n_points);
//      
//      auto C = Grid.find_cell(R, n_points);
//      Grid.voronoi_grid_3D(x, y,z, n_points);
//      Grid.set_voronoi_grid(R, C, n_points, dim);
//     Grid.set_grid_wrapper(x, y, z, n_points, dim);
      
//      cout<< "it worked!!\n";
      
      
//      return 0;
//      }


} //namespace libqqc

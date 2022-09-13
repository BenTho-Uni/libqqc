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

using namespace std;

namespace libqqc {
    //
    // 1D Grid functions 
    //
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

    //
    // 2D Grid functions 
    //
    template<typename vec_type, typename n_dim> // vec_type: vector type, n_dim: dimension of molecule list
    bool Grid2D<vec_type, n_dim> ::check_data_validity() {
        if (ndim == 0) throw invalid_argument(
            "Dimensionality must not be 0.");
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
  
    template<typename vec_type, typename n_dim> 
    void Grid3D<vec_type, n_dim> :: set_unigrid_wrappter(vec_type x_pts, vec_type y_pts, vec_type z_pts, n_dim ndim, int n_points){
    vector<vector<double>> grid_pts = set_grid(x_pts,y_pts,z_pts, ndim, n_points);
    vector<vector<double>> grid_wts = set_weights(x_pts,y_pts,z_pts, ndim, n_points);
    
    }//Grid3D::set_unigrid_wrapper

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
    vector<vector<double>> super_triangle { {-100, -100}, {-100, 100}, {100, 0}};
		// add test to see that points are in super triangle
		return super_triangle;
	} // Voronoi::def_supertriangle

	template<typename vec_type> // vec_type: vector type for points
	vector< vector<vector<double>>> Voronoi<vec_type> ::watsons_alg(vector< vector<double> > sorted, vector< vector<double> > super_triangle) {
		// initializing vectors for triangles and edges
		vector< vector<vector<double>> > triangle_list{ super_triangle };
		vector< vector<vector<double>> > final_triangles;
		vector<vector<double>> edges;
		VectorFuncs vecf; //initialise vectorfunc class
		// loop over sorted points
		for (int s = 0; s < sorted.size(); s++) {
			// initialise first point and call "new"
			double x_new = sorted[s][0];
			double y_new = sorted[s][1];
			// iterate through non-complete triangle list
			for (int itr = 0; itr < triangle_list.size(); itr++) {
				// compute triangle circumcenter
				vector<vector<double>> t = triangle_list[itr]; // Triangle coordinates
				sort(t.begin(), t.end());
        // compute triangle circumcenter
        vector<vector<double>>  matrix = vecf.Det_Matrix2D(t);
        vector<vector<double>>  a {matrix[1], matrix[2], matrix[3]};
        double A = vecf.Determinant2D(a);
        vector<vector<double>> c {matrix[0], matrix[1], matrix[2]};
        double C = vecf.Determinant2D(c);
        vector<vector<double>> d_x {matrix[0], matrix[2],  matrix[3]};
        vector<vector<double>> d_y {matrix[0], matrix[1],  matrix[3]};
        double Dx = vecf.Determinant2D(d_x);
        double Dy = -1* vecf.Determinant2D(d_y);
        vector<double> circumcenter{ Dx/(2*A), Dy/(2*A)}; // adding the circumcenter of triangle as a vertex
				double R = pow((t[0][0] - circumcenter[0]), 2) + pow((t[0][1] - circumcenter[1]), 2); // squared radius of circumcircle
				double D_x = pow((circumcenter[0] - x_new), 2); // square of x-distance from x_new to triangle circumcenter
				if (D_x > R) { // checking if squared x-distance is bigger than squared radius, when true adding triangle to final triangles
				// -> point lies outside of triangle
					final_triangles.push_back(triangle_list[itr]);
					if(final_triangles.size() > 1 ){
              for( int f =0; f < final_triangles.size() ; f++){
              vector<vector<double>> samefinal;
              for( int f2 =1; f2 < final_triangles.size(); f2++){
                for( int pts = 0 ; pts <3 ; pts++){// when yes, check if any of the triangles have two points in common -> an edge in common
                if (final_triangles[f][pts] == final_triangles[f2][pts]){
                    samefinal.insert(samefinal.end(), final_triangles[f][pts]);
                    }
                 }
                if (samefinal.size() == 2){ // when counter is 2, then the triangles do have an edge in common
                //triangle_list.erase(triangle_list.begin() + tri) ; //removing bigger triangle
                vector<vector<double>> not_equal;
                for( int t1 =0; t1< 3; t1++){
                  if(final_triangles[f][t1] != samefinal[0] & final_triangles[f][t1] != samefinal[1]) not_equal.insert(not_equal.end(), final_triangles[f][t1]);
                  }
                for( int t1 =0; t1< 3; t1++){
                  if(final_triangles[f2][t1] != samefinal[0] & final_triangles[f2][t1] != samefinal[1]) not_equal.insert(not_equal.end(), final_triangles[f2][t1]);
                  }
                double denominator = sqrt( pow(samefinal[1][0] - samefinal[0][0], 2) + pow(samefinal[1][1] - samefinal[0][1], 2));
                double d_tri = ((samefinal[1][0] - samefinal[0][0])*(samefinal[0][1] - not_equal[0][1]) + (samefinal[0][0] - not_equal[0][0])*(samefinal[1][1] - samefinal[0][1])) / denominator ; // calculating distance of point to edge
                double d_next = ((samefinal[1][0] - samefinal[0][0])*(samefinal[0][1] - not_equal[1][1]) + (samefinal[0][0] - not_equal[1][0])*(samefinal[1][1] - samefinal[0][1])) / denominator ; // calculating distance of point to edge
               // cout << d_tri << ";" << d_next << "\n";
                if (signbit(d_tri) == signbit(d_next)) {
                if ( abs(d_tri) > abs(d_next) ) { // checking which point is furthest away from edge
                  final_triangles.erase(final_triangles.begin() +f); // erase the bigger triangle
                  }
                if ( abs(d_tri) < abs(d_next) ) { // checking which point is furthest away from edge
                  final_triangles.erase(final_triangles.begin() + f2); // erasing the bigger triangle
                  }
                }
            }}}}

					triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list
					itr--;
				}
        if (D_x < R) {
				// when x-distance is smaller than radius
				double D = sqrt(D_x + pow((circumcenter[1] - y_new), 2)); // calculating distane from new point to circumcenter of triangle
				if (D < R) { // checking if distance is smaller than squared radius -> point lies within triangle -> cannot be a final triangle
					edges.push_back({ t[0][0], t[0][1], t[1][0], t[1][1] });
					edges.push_back({ t[1][0], t[1][1], t[2][0], t[2][1] });
					edges.push_back({ t[0][0], t[0][1], t[2][0], t[2][1] });
          				triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list and adding the three edges to edges vector
          				itr--;
				}
       }
			}
			sort(edges.begin(), edges.end()); // sort edges
			edges.erase(unique(edges.begin(), edges.end()), edges.end()); // erase edges that occur multiple times
			//from all edges create new triangles with the new point and add them as incomplete triangles
			for (int i = 0; i < edges.size(); i++) {
				vector< vector<double> > new_triangle{ {x_new,y_new} };
				new_triangle.push_back({ edges[i][0], edges[i][1] });
				new_triangle.push_back({ edges[i][2], edges[i][3] });
        sort(new_triangle.begin(), new_triangle.end());
				triangle_list.push_back(new_triangle);
			  }
      sort(triangle_list.begin(), triangle_list.end()); // sort edges
			triangle_list.erase(unique(triangle_list.begin(), triangle_list.end()), triangle_list.end()); // erase edges that occur multiple times
			if (triangle_list.size()+final_triangles.size()  > edges.size() ){ // check if triangles are left over from construction with last point       
          for( int tri = 0 ; tri < triangle_list.size(); tri++) {
              for ( int next = tri + 1; next < triangle_list.size() ; next++) {
                vector<vector<double>> same;
                for( int pts = 0 ; pts <3 ; pts++){// when yes, check if any of the triangles have two points in common -> an edge in common
                if (triangle_list[tri][pts] == triangle_list[next][pts]){
                  same.insert(same.end(), triangle_list[tri][pts]);
                  }
                }
                if (same.size() == 2){ // when counter is 2, then the triangles do have an edge in common
                vector<vector<double>> not_equal;
                for( int t1 =0; t1< 3; t1++){
                  if(triangle_list[tri][t1] != same[0] & triangle_list[tri][t1] != same[1]) not_equal.insert(not_equal.end(), triangle_list[tri][t1]);
                  }
                for( int t1 =0; t1< 3; t1++){
                   if(triangle_list[next][t1] != same[0] & triangle_list[next][t1] != same[1]) not_equal.insert(not_equal.end(), triangle_list[next][t1]); 
                   }
                double denominator = sqrt( pow(same[1][0] - same[0][0], 2) + pow(same[1][1] - same[0][1], 2));
                double d_tri = ((same[1][0] - same[0][0])*(same[0][1] - not_equal[0][1]) + (same[0][0] - not_equal[0][0])*(same[1][1] - same[0][1])) / denominator ; // calculating distance of point to edge
                double d_next = ((same[1][0] - same[0][0])*(same[0][1] - not_equal[1][1]) + (same[0][0] - not_equal[1][0])*(same[1][1] - same[0][1])) / denominator ; // calculating distance of point to edge
                // cout << d_tri << ";" << d_next << "\n";
                if (signbit(d_tri) == signbit(d_next)){ // if distance same sign then the points are on the same side of the plane, if opposite signs then on difference sides and both triangles are kept
                if ( abs(d_tri) > abs(d_next) ) { // checking which point is furthest away from edge
                triangle_list.erase(triangle_list.begin() + tri); // erase the bigger triangle
                }
                if ( abs(d_tri) < abs(d_next) ) { // checking which point is furthest away from edge
                triangle_list.erase(triangle_list.begin() + next); // erasing the bigger triangle
                }
              }
            }
          }
          if(final_triangles.size() > 0 ){
          for( int f =0; f < final_triangles.size() ; f++){
            vector<vector<double>> samefinal;
            for( int pts = 0 ; pts <3 ; pts++){// when yes, check if any of the triangles have two points in common -> an edge in common
              if (triangle_list[tri][pts] == final_triangles[f][pts]){
                samefinal.insert(samefinal.end(), triangle_list[tri][pts]);
                }
              }
            if (samefinal.size() == 2){ // when counter is 2, then the triangles do have an edge in common
            vector<vector<double>> not_equal;
            for( int t1 =0; t1< 3; t1++){
            if(triangle_list[tri][t1] != samefinal[0] & triangle_list[tri][t1] != samefinal[1]) not_equal.insert(not_equal.end(), triangle_list[tri][t1]);
              }
            for( int t1 =0; t1< 3; t1++){
              if(final_triangles[f][t1] != samefinal[0] & final_triangles[f][t1] != samefinal[1]) not_equal.insert(not_equal.end(), final_triangles[f][t1]); 
              }
            double denominator = sqrt( pow(samefinal[1][0] - samefinal[0][0], 2) + pow(samefinal[1][1] - samefinal[0][1], 2));
            double d_tri = ((samefinal[1][0] - samefinal[0][0])*(samefinal[0][1] - not_equal[0][1]) + (samefinal[0][0] - not_equal[0][0])*(samefinal[1][1] - samefinal[0][1])) / denominator ; // calculating distance of point to edge
            double d_next = ((samefinal[1][0] - samefinal[0][0])*(samefinal[0][1] - not_equal[1][1]) + (samefinal[0][0] - not_equal[1][0])*(samefinal[1][1] - samefinal[0][1])) / denominator ; // calculating distance of point to edge
            if (signbit(d_tri) == signbit(d_next)) { // if distance same sign then the points are on the same side of the plane, if opposite signs then on difference sides and both triangles are kept
            if ( abs(d_tri) > abs(d_next) ) { // checking which point is furthest away from edge
            triangle_list.erase(triangle_list.begin() + tri); // erase the bigger triangle
            }
            if ( abs(d_tri) < abs(d_next) ) { // checking which point is furthest away from edge
            triangle_list.erase(final_triangles.begin() + f); // erasing the bigger triangle
            }
          }
        }
      }
    }
    }
    }
		}
		final_triangles.insert(final_triangles.end(), triangle_list.begin(), triangle_list.end());
		int itr = 0; // initialise counter
		while (itr < final_triangles.size() || final_triangles.size() == 0) { // execute while counter is smaller than length of final triangle list or as long as final triangle list is not zero
		continue_here:
			// nested for loop to remove all triangles that include one or more of the points from the super triangle
			for (int i = 0; i < 3; i++) {
				for (int f = 0; f < 3; f++) {
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
  
  
// int main(){
    //  vector<double> x { -1.04056,(-1.04056+0.07267)/2, -0.07267,(-0.07267+1.31941)/2 ,-1.31941};
    //  vector<double> y { 2.23373,(2.23373-2.19278)/2 ,2.19278,(2.19278-1.41547)/2 ,1.41547};
    //  vector<double> z { -0.00942, (-0.00942-0.01248)/2,0.01248, (0.01248-0.43837)/2 ,0.43837};
    //  cout<< "test\n";
    //  int n_points= 10;
    //  size_t dim =3;
    //  Grid3D<vector<double>, size_t> Grid;
    //  auto R = Grid.gauss_cheb_3D(x, y, z, n_points);
    //  Voronoi<vector<double>> Voro;
    //  Voro.construct_voronoi_3D(x, y, z);
    //  cout << "worked until here";
    //  auto C = Grid.find_cell(R, n_points, Voro);
      //cout << "worked until here\n";
    //  double* mpts = NULL; ///< Pointer towards the points
    //  double* mwts = NULL; ///< Pointer towards the weights
    //  size_t mnpts;
    //  size_t mndim;
    //  map<vector<double>, vector<double>> weights = Voro.get_vweights();
    //  mnpts = n_points;
    //  mndim = dim;
      //if initialised - delete them, if  NULL generate new ones
    //  if (mpts || mwts) {
     //       delete[] mpts;
     //       delete[] mwts;
   // }
    //   mpts = new double[mnpts * mndim];
     //  mwts = new double[mnpts * mndim];
     //  vector<vector<double>> grid_pts = R[0];
      // vector<vector<double>> grid_wts = R[1];
       //mpts = new double[grid_pts[0].s)*mndim];
       //mwts = new double[grid_wts[0].size()*mndim];
    //loop over all points and dimensions and copy the data
    //   size_t count = 0;
    //   multimap<vector<double>, vector<double>>::iterator iter;
       //mpts[0] = grid_pts[0][0];
       //mwts[0] = grid_wts[0][0];
      // for (iter = C.begin(); iter != C.end(); iter++) {
        //for ( int i = 0; i < grid_pts.size(); i++){
      //  vector<double> vertex = iter->first;
      //  vector<double> point = iter->second;
      //  map<vector<double>, vector<double>>::iterator voronoi_w = weights.find(vertex); //getting the weight for the voronoi cell which the point is located in
      //  vector<double> voro_weight = voronoi_w->second;
      //  for (size_t d = 0; d < mndim; d++) {
       //     mpts[count * mndim + d] = point[d];
       //     mwts[count * mndim + d] = grid_wts[d][count] * voro_weight[d];
     //   }

        //mpts[count * mndim + 1] = point[1];
        //mpts[count * mndim + 2] = point[2];

        //mwts[count * mndim + 1] = grid_wts[1][count] * voro_weight[1];
        //mwts[count * mndim + 2] = grid_wts[2][count] * voro_weight[2];
   //     count++;
  //  }
   //    cout << "pointer to pts" << mpts[0];
  //     cout << "pointer to pts" << *mwts;

//      Grid.set_voronoi_grid(R, C, n_points, dim, Voro);
     // Grid.set_grid_wrapper(x, y, z, n_points, dim);
//      cout<< "it worked!!\n";
      
      
//      return 0;
//      }

} // namespace libqqc

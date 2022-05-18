///
/// A uniform grid constructor
/// @file unigrid.cpp
/// @author Isabel Vinterbladh
/// @version 0.1 19-04-2022
//

#define _USE_MATH_DEFINES

#include "voronoi.h"
#include "vector_funcs.h"

#include <vector>
#include <iterator>
#include <list>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
using namespace std;


namespace libqqc {

	template<typename vec_type>
	auto Voronoi<vec_type> ::sorting(vec_type& x_pts, vec_type& y_pts) {
		if (x_pts.size() != y_pts.size()) throw invalid_argument(
			"Must be same dimensionality of x and y points.");

		// initializing variables
		auto min_x = x_pts[0];
		auto min_y = y_pts[0];
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


	template<typename vec_type>
	auto Voronoi<vec_type> ::def_supertriangle(vector<vector<double>>& sorted) {
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

	template<typename vec_type>
	auto Voronoi<vec_type> ::watsons_alg(vector< vector<double> >& sorted, vector< vector<double> >& super_triangle) {
		// initializing vectors for triangles and edges
		vector< vector<vector<double>> > triangle_list{ super_triangle };
		vector< vector<vector<double>> > final_triangles;
		vector<vector<double>> edges;
		// loop over sorted points
		for (int s = 0; s < sorted.size(); s++) {
			// initialise first point and call "new"
			auto x_new = sorted[s][0];
			auto y_new = sorted[s][1];
			// iterate through non-complete triangle list
			for (auto itr = 0; itr < triangle_list.size(); itr++) {
				// compute triangle circumcenter
				auto t = triangle_list[itr]; // Triangle coordinates
				auto slope_1 = (t[1][1] - t[0][1]) / (t[1][0] - t[0][0]); // calculating slope of triangle edge
				auto slope_2 = (t[2][1] - t[1][1]) / (t[2][0] - t[1][0]); // calculating slope of triangle edge
		
				if (t[0][0] == t[1][0] || t[1][1] == t[0][1]) {
				slope_1 = 1; // calculating slope of triangle edge
				} 
				if (t[1][0] == t[2][0] || t[2][1] == t[1][1]) {
				slope_2 = 1; // calculating slope of triangle edge
				}
				vector<double> mid_p1{ (t[0][0] + t[1][0]) / 2, (t[0][1] + t[1][1]) / 2 }; // constructing vector of midpoints on edge
				vector<double> mid_p2{ (t[1][0] + t[2][0]) / 2, (t[1][1] + t[2][1]) / 2 }; // constructing vector of midpoints on edge
				auto c_x = ((slope_1/ slope_2) * mid_p2[0] - slope_1* (mid_p1[1] - mid_p2[1])) / (1 - (slope_1 / slope_2)); // circumcenter x point
				auto c_y = (1/slope_1) * (c_x - mid_p1[0]) + mid_p1[1]; // circumcenter y point
				auto R = pow((t[0][0] - c_x), 2) + pow((t[0][1] - c_y), 2); // squared radius of circumcircle
				auto D_x = pow((c_x - x_new), 2); // square of x-distance from x_new to triangle circumcenter
				if (D_x >= R) { // checking if squared x-distance is bigger than squared radius, when true adding triangle to final triangles
			// -> point lies outside of triangle
					final_triangles.push_back(triangle_list[itr]);
					triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list
					//cout << "triangle added \n";
				} 
				// when x-distance is smaller than radius
				auto D = sqrt(D_x + pow((c_y - y_new), 2)); // calculating distane from new point to circumcenter of triangle
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
						goto continue_here; // afte removing a triangle do not increase counter, therefore goto used
					}
				}
			}
			itr++; // increase couter to check next triangle
		}
	end_while_loop:
		return final_triangles;	// returning vector of Delaunay triangles
	} // Voronoi::watsons_alg 

	template<typename vec_type>
	auto Voronoi<vec_type> ::getting_dual_graph(vector< vector< vector<double> >>& triangles) {
		vector<vector<double>> vertex;

		for (int i = 0; i < triangles.size(); i++) {
			auto t = triangles[i]; // Triangle coordinates
			auto slope_1 = (t[1][1] - t[0][1]) / (t[1][0] - t[0][0]); // calculating slope of triangle edge
			auto slope_2 = (t[2][1] - t[1][1]) / (t[2][0] - t[1][0]); // calculating slope of triangle edge
			vector<double> mid_p1{ (t[0][0] + t[1][0]) / 2, (t[0][1] + t[1][1]) / 2 }; // constructing vector of midpoints on edge
			vector<double> mid_p2{ (t[1][0] + t[2][0]) / 2, (t[1][1] + t[2][1]) / 2 }; // constructing vector of midpoints on edge
			auto c_y = ((-1 / slope_2) * (mid_p1[0] - mid_p2[0]) + mid_p2[1] - (slope_1 / slope_2) * mid_p1[1]) / (1 - (slope_1 / slope_2)); // circumcenter y point
			auto c_x = -slope_1 * (c_y - mid_p1[1]) + mid_p1[0]; // circumcenter x point
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



	template<typename vec_type>
	auto Voronoi<vec_type> ::sorting_3D(vec_type& x_pts, vec_type& y_pts, vec_type& z_pts) {
		if (x_pts.size() != y_pts.size() || x_pts.size() != z_pts.size() || z_pts.size() != y_pts.size()) throw invalid_argument(
			"Must be same dimensionality of x, y and z points.");

		// initializing variables
		auto min_x = x_pts[0];
		auto min_y = y_pts[0];
		auto min_z = z_pts[0];
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

	template<typename vec_type>
	auto Voronoi<vec_type> ::def_supertriangle_3D(vector<vector<double>>& sorted) {
		vector< vector<double> > super_triangle{ {0, 0, 10000}, {-10000, 0, -1000}, {10000, -10000, -1000},{10000, 10000, -1000} };
		// add test to see that all points are in super triangle
		return super_triangle;
	} // Voronoi::def_supertriangle_3D
  
	template<typename vec_type>
	auto Voronoi<vec_type> ::watsons_alg_3D(vector< vector<double> >& sorted, vector< vector<double> >& super_triangle) {
		// initializing vectors for triangles and edges
		vector< vector<vector<double>> > triangle_list{ super_triangle };
		vector< vector<vector<double>> > final_triangles;
		//vector<vector<double>> edges;

		// loop over sorted points
		for (int s = 0; s < sorted.size(); s++) {
			// initialise first point and call "new"
			auto x_new = sorted[s][0];
			auto y_new = sorted[s][1];
			auto z_new = sorted[s][2];
			cout << "new point \n";
			vector<vector<double>> edges; // creating a vector with the edges for each new point
			// iterate through non-complete triangle list
			for (int itr = 0; itr < triangle_list.size(); itr++) {
		        auto t = triangle_list[itr]; // Triangle coordinates
             		sort(t.begin(), t.end());
			// compute triangle circumcenter
                     	vector<vector<double>>  matrix = Det_Matrix(t);
                      	vector<vector<double>>  a {matrix[1], matrix[2], matrix[3], matrix[4]};
                      	double A = Determinant(a);
                      	vector<vector<double>> c {matrix[0], matrix[1], matrix[2], matrix[3]};
                      	double C = Determinant(c);
                      	vector<vector<double>> d_x {matrix[0], matrix[2], matrix[3], matrix[4]};
                      	vector<vector<double>> d_y {matrix[0], matrix[1], matrix[3], matrix[4]};
                      	vector<vector<double>> d_z {matrix[0], matrix[1], matrix[2], matrix[4]};
                      	double Dx = Determinant(d_x);
                      	double Dy = -1* Determinant(d_y);
                      	double Dz = Determinant(d_z);
                      	//cout << Dx << "," << Dy <<"," << Dz << "\n";
                      	vector<double> circumcenter{ Dx/(2*A), Dy/(2*A), Dz/(2*A) }; // adding the circumcenter of tetrahedron as a vertex
                      	//cout << circumcenter[0] << "," << circumcenter[1] << "," << circumcenter[2] << "\n";
        		//cout << "a:" << A << "\n";
			if( s== 0){
			cout << "super edges \n" ;
			edges.push_back({ t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2] });
                        edges.push_back({ t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2] });
			edges.push_back({ t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2] });
			edges.push_back({ t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2] });
			triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list and adding the four  edges to edges vector
			goto added_super_edges;
			}
				//auto R = (pow(circumcenter[0], 2) + pow(circumcenter[1], 2) + pow(circumcenter[2], 2) -4*A*C) / pow((2*abs(A)),2) ; // squared radius of circumcircle
				double R = pow((t[0][0] - circumcenter[0]), 2) + pow((t[0][1] - circumcenter[1]), 2) + pow((t[0][2] - circumcenter[2]), 2); // squared radius of circumcircle
				double D_x = pow((circumcenter[1]- y_new), 2); // square of x-distance from x_new to triangle circumcenter
				//cout <<  R << ":" << D_x <<  "\n";
				if (D_x > R ) { // checking if squared x-distance is bigger than squared radius, when true adding triangle to final triangles
				// -> point lies outside of triangle
					cout << "added triangle \n";
						final_triangles.push_back(triangle_list[itr]);
						triangle_list.erase(triangle_list.begin() + itr);// erasing triangle from incomplete triangle list
						itr--; //to not miss a triangle
				}
				
				if (D_x < R) { // when x-distance is smaller than radius
					double  D = D_x + pow((circumcenter[1] - y_new), 2) + pow((circumcenter[2] - z_new), 2); // calculating distane from new point to circumcenter of triangle
					//cout << "if D smaller than R:"<< D << ":" << R << "\n";
					if (D < R) { // checking if distance is smaller than squared radius -> point lies within triangle -> cannot be a final triangle
						cout << "adding edges\n";
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
			cout << "edges:" << edges.size() << "\n";
			//from all edges create new triangles with the new point and add them as incomplete triangles
			for (int i = 0; i < edges.size(); i++) {
				vector< vector<double> > new_triangle{ {x_new,y_new, z_new} };
				new_triangle.push_back({ edges[i][0], edges[i][1], edges[i][2] });
				new_triangle.push_back({ edges[i][3], edges[i][4], edges[i][5] });
				new_triangle.push_back({ edges[i][6], edges[i][7], edges[i][8] });
				sort(new_triangle.begin(), new_triangle.end());
				triangle_list.push_back(new_triangle);
			}
			cout << "triangles:" << triangle_list.size() <<":" << final_triangles.size() << "\n";
			
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
				auto normal = crossProduct(vec1, vec2); // calculating the normal of the plane
				auto d_tri = normal[0]*(not_equal[0][0] - same[0][0]) + normal[1]*(not_equal[0][1] - same[0][1]) + normal[2]*(not_equal[0][2] - same[0][2]); // calculating distance of point from plane
				auto d_next = normal[0]*(not_equal[1][0] - same[0][0]) + normal[1]*(not_equal[1][1] - same[0][1]) + normal[2]*(not_equal[1][2] - same[0][2]); // calculating distance of point from plane
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
	
//	for( int tri = 0 ; tri < final_triangles.size(); tri ++) {
//                                for ( int next = tri + 1; next < triangle_list.size() ; next ++) {
//                                        int counter = 0;
//                                        for( int pts = 0 ; pts <4 ; pts++){// when yes, check if any of the triangles have three points in common -> a whole face in common
//                                        if (final_triangles[tri][pts] == final_triangles[next][pts]){
//                                        counter++;
//                                        }
//                                 }
//                                if (counter == 3){ // when counter is 3, then the triangles do have a face in common
//                                vector<vector<double>> not_equal(2);
//                                vector<vector<double>> same(3);
//                                set_symmetric_difference(final_triangles[tri].begin(), final_triangles[tri].end(), final_triangles[next].begin(),final_triangles[next].end(), not_equal.begin()); // find which point do differ
//                                set_intersection(final_triangles[tri].begin(), final_triangles[tri].end(), final_triangles[next].begin(), final_triangles[next].end(), same.begin()); // find which three points are the same in the two triangles
//                                vector<double> vec1 {{same[1][0]- same[0][0]}, {same[1][1] - same[0][1]}, {same[1][2]  - same[0][2]} }; //construct a vector in the plane of the face of triangles
//                                vector<double> vec2 {{same[2][0]- same[0][0]}, {same[2][1] - same[0][1]}, {same[2][2]  - same[0][2]} }; // construct a vector in the plane of the face of triangles
//                                auto normal = crossProduct(vec1, vec2); // calculating the normal of the plane
//                                auto d_tri = normal[0]*(not_equal[0][0] - same[0][0]) + normal[1]*(not_equal[0][1] - same[0][1]) + normal[2]*(not_equal[0][2] - same[0][2]); // calculating distance of point from plane
//                                auto d_next = normal[0]*(not_equal[1][0] - same[0][0]) + normal[1]*(not_equal[1][1] - same[0][1]) + normal[2]*(not_equal[1][2] - same[0][2]); // calculating distance of point from plane
//                                if (signbit(d_tri) == signbit(d_next)) { // if distance same sign then the points are on the same side of the plane, if opposite signs then on difference sides and both triangles are kept
//                                if ( abs(d_tri) > abs(d_next) ) { // checking which point is furthest away from plane
//                                final_triangles.erase(final_triangles.begin() + tri); // erase the bigger tetrahedron
//                                }
//                                if ( abs(d_tri) < abs(d_next) ) { // checking which point is furthest away from plane
//                                final_triangles.erase(final_triangles.begin() + next); // erasing the bigger tetrahedron
//                                }
//                                }
//                                }
//                                }
//                }

		for (int i = 0; i < final_triangles.size(); i++) { //making sure no identical tetrahedrons are saved 
			sort(final_triangles[i].begin(), final_triangles[i].end());
		} 
		sort(final_triangles.begin(), final_triangles.end()); // sort triangles
		final_triangles.erase(unique(final_triangles.begin(), final_triangles.end()), final_triangles.end());//removing copies of triangles

		return final_triangles;	// returning vector of Delaunay triangles
	} // Voronoi::watsons_alg_3D 


	template<typename vec_type>
	auto Voronoi<vec_type> ::getting_dual_graph_3D(vector< vector< vector<double> >>& triangles) {
		vector<vector<double>> vertex;

		for (int i = 0; i < triangles.size(); i++) {
			vector<vector<double>>  matrix = Det_Matrix(triangles[i]);
			vector<vector<double>>  a {matrix[1], matrix[2], matrix[3], matrix[4]}; 	
			double A = Determinant(a);
			vector<vector<double>> D_x {matrix[0], matrix[2], matrix[3], matrix[4]};
		  	vector<vector<double>> D_y {matrix[0], matrix[1], matrix[3], matrix[4]};
			vector<vector<double>> D_z {matrix[0], matrix[1], matrix[2], matrix[4]};
			double Dx = Determinant(D_x);
			double Dy = -1* Determinant(D_y);
			double Dz = Determinant(D_z);
			
			vector<double> circumcenter{ Dx/(2*A), Dy/(2*A), Dz/(2*A) }; // adding the circumcenter of tetrahedron as a vertex
			cout << circumcenter[0] << "," << circumcenter[1] << "," << circumcenter[2] << "\n";
			vertex.push_back(circumcenter);
			
			//auto t = triangles[i]; // Triangle coordinates
			//vector<double> cross1 = crossProduct(U(t[2], t[0]), U(t[3], t[0]));
			//vector<double> cross2 = crossProduct(U(t[3], t[0]), U(t[1], t[0]));
			//vector<double> cross3 = crossProduct(U(t[1], t[0]), U(t[2], t[0]));
			//double c_x = t[0][0] + (Length(t[0], t[1]) * cross1[0] + Length(t[0], t[2]) * cross2[0] + Length(t[0], t[3]) * cross3[0]) / (2 * U(t[1], t[0])[0] * cross1[0]);
			//if (isinf(c_x) || isnan(c_x)) {
			//	c_x = 0;
			//}
			//double c_y = t[0][1] + (Length(t[0], t[1]) * cross1[1] + Length(t[0], t[2]) * cross2[1] + Length(t[0], t[3]) * cross3[1]) / (2 * U(t[1], t[0])[1] * cross1[1]);
			//if (isinf(c_y) || isnan(c_y)) {
			//	c_y = 0;
			//}
			//double c_z = t[0][2] + (Length(t[0], t[1]) * cross1[2] + Length(t[0], t[2]) * cross2[2] + Length(t[0], t[3]) * cross3[2]) / (2 * U(t[1], t[0])[2] * cross1[2]);
			//if (isinf(c_z) || isnan(c_z)) {
			//	c_z = 0;
			//}
			//cout << "vertex " << c_x << ":" << c_y << ":" << c_z << "\n";
			//vector<double> circumcenter{ c_x, c_y, c_z }; // adding the circumcenter of tetrahedron as a vertex
			//vertex.push_back(circumcenter);
		}
		// checking that the circumcenters for all triangles were calculated
		if (vertex.size() != triangles.size()) throw invalid_argument(
			"Must be same dimensionality of vertex and triangles.");
		vector <vector <vector<double>> > voronoi_edges; // initialising vector with edges for voronoi cells
            
		// adding edges
		for (int ver = 0; ver < vertex.size(); ver++){ // loop over vertex
			vector<vector<vector<double>>> faces;
			
			for (int next_v= 0; next_v< triangles.size(); next_v++) { // loop over triangles for not already checked vertex
				if( ver != next_v) {
				vector<vector<double>> same_points;
				set_intersection(triangles[ver].begin(), triangles[ver].end(), triangles[next_v].begin(), triangles[next_v].end(),back_inserter(same_points));
				if (same_points.size() == 3){
				faces.push_back(same_points);
				vector< vector < double> >  edge_found{ {vertex[ver][0],vertex[ver][1],vertex[ver][2]}, { vertex[next_v][0],vertex[next_v][1],vertex[next_v][2]} };  // adding the edge connecting the two vertex of the triangles
                                sort(edge_found.begin(), edge_found.end());
				voronoi_edges.push_back(edge_found); // save edge as part of voronoi cells
				}}}
			if( faces.size() <4 ){
			vector<vector<vector<double>>> outer_edges;
			vector<vector<vector<double>>> triangle_faces {{triangles[ver][0], triangles[ver][1], triangles[ver][2]}, {triangles[ver][1], triangles[ver][2], triangles[ver][3] }, {triangles[ver][2], triangles[ver][3], triangles[ver][0]},{ triangles[ver][3], triangles[ver][0], triangles[ver][1]}};
			set_symmetric_difference(triangle_faces.begin(), triangle_faces.end(), faces.begin(), faces.end(), back_inserter(outer_edges));
			//cout << outer_edges.size() << "\n";	
			for (int outer_ver= 0 ; outer_ver < outer_edges.size(); outer_ver++) {// if less than four edges found the tetrahedron is an outer cell, has to add corresponding number of edges
				vector<double> vec1 {{outer_edges[outer_ver][1][0]- outer_edges[outer_ver][0][0]}, {outer_edges[outer_ver][1][1] - outer_edges[outer_ver][0][1]}, {outer_edges[outer_ver][1][2]  - outer_edges[outer_ver][0][2]} }; //construct a vector in the plane of the face of triangles
                                vector<double> vec2 {{outer_edges[outer_ver][2][0]- outer_edges[outer_ver][0][0]}, {outer_edges[outer_ver][2][1] - outer_edges[outer_ver][0][1]}, {outer_edges[outer_ver][2][2]  - outer_edges[outer_ver][0][2]} }; // construct a vector in the plane of the face of triangles
                                auto normal = crossProduct(vec1, vec2); // calculating the normal of the plane
				auto k = (outer_edges[outer_ver][0][0]-vertex[ver][0])/normal[0];
				// finding the point on the opposite side of the face and add an edge between
				vector <double> new_vertex {2*(normal[0]*k + vertex[ver][0])- vertex[ver][0], 2*(normal[1]*k + vertex[ver][1])-vertex[ver][1], 2*(normal[2]*k + vertex[ver][2])-vertex[ver][2] }; // finding a point on the other side of the plane at the same distance
				vector< vector <double>>  edge_found{{ vertex[ver][0],vertex[ver][1],vertex[ver][2]},{ new_vertex[0], new_vertex[1], new_vertex[2]}}; // creating new edge between the vertex and new_vertex
				cout << "new edge" << vertex[ver][0]<<","<< vertex[ver][1] <<"," << vertex[ver][2] <<","<< new_vertex[0] <<","<< new_vertex[1]<<","<< new_vertex[2]<< "\n";
				sort(edge_found.begin(), edge_found.end());
				voronoi_edges.push_back(edge_found);
			}			
			}
			
		}
		

		for (int i = 0; i < voronoi_edges.size(); i++) {
			if (voronoi_edges[i][0] == voronoi_edges[i][1]) { // checking if voronoi edge is just between same point, erase it
				voronoi_edges.erase(voronoi_edges.begin() + i);
			}
		}
		// erasing copies of edges
		sort(voronoi_edges.begin(), voronoi_edges.end()); // sort edges
		cout << voronoi_edges.size() << " before erasing copies\n";
		voronoi_edges.erase(unique(voronoi_edges.begin(), voronoi_edges.end()), voronoi_edges.end());// erase copies
		vector <vector <vector<double>> > voronoi_faces;
		
		// finding faces	
		for(int edge = 0 ; edge < voronoi_edges.size(); edge++){
			vector <int> indices {edge};	
			vector <vector< double>> face {voronoi_edges[edge][0], voronoi_edges[edge][1]};
				for(int next = 0;  next < voronoi_edges.size() ; next++) {
				cout << next << "\n";
				if( find(indices.begin(), indices.end(), next) == indices.end()  && face.back() == voronoi_edges[next][0]) {
				cout << "found point 0 \n";
				indices.push_back(next);
				face.push_back(voronoi_edges[next][1]);
				next = 0;		
				}

				if( find(indices.begin(), indices.end(), next) == indices.end() && face.back() == voronoi_edges[next][1]) {
                                cout << "found point 1 \n";
				indices.push_back(next);
				face.push_back(voronoi_edges[next][0]);
				next = 0;
				}			
				if(face.front()== face.back()){
                                        cout << "adding face \n";
                                        voronoi_faces.push_back(face);
                                        break;
                                        }
			}
			if(face.size()>2){
			voronoi_faces.push_back(face);
				
			}
	       	}	
	return voronoi_edges;
	 }// Voronoi::getting_dual_graph_3D


//	int main() {
//		vector<double> x { -1.04056,(-1.04056+0.07267)/2, -0.07267,(-0.07267+1.31941)/2 ,-1.31941};
//		vector<double> y { 2.23373,(2.23373-2.19278)/2 ,2.19278,(2.19278-1.41547)/2 ,1.41547};
//		vector<double> z { -0.00942, (-0.00942-0.01248)/2,0.01248, (0.01248-0.43837)/2 ,0.43837};
//		Voronoi<vector<double>> Voro;
//		vector< vector<double> > result = Voro.sorting_3D(x, y, z);
//		for (int i = 0; i <result.size(); i++) {
//			cout << result[i][0]<<"," << result[i][1] << ","<< result[i][2] << "\n";
//		}
//	
//		vector< vector<double> > s_tri = Voro.def_supertriangle_3D(result);
//		
//		cout << "Super triangle \n";
//		for (int i = 0; i < s_tri.size(); i++) {
//			cout << s_tri[i][0] << "," << s_tri[i][1] << "," << s_tri[i][2] << " ;\n";
//		}
//		auto res = Voro.watsons_alg_3D(result, s_tri);
//		cout << "Delauny triangulation \n";
//		cout << res.size() << "\n";
//		ofstream outd;
//		outd.open("delaunay.txt");
//		for (int i = 0; i < res.size(); i++) {
//			for (int j = 0; j < res[i].size(); j++) {
//				outd << res[i][j][0] << "," << res[i][j][1] << "," << res[i][j][2] << "\n";
//			}
//		}
//		outd.close();
//    vector<vector<double>>  matrix = Det_Matrix(s_tri);
//    vector<vector<double>>  a {matrix[1], matrix[2], matrix[3], matrix[4]};
//    double A = Determinant(a);
//    //cout << "A:" << A << "\n";
//		//vector< vector<vector<double>> > Del{ {{-2,5,1},{2,0,0},{0,2,0},{0,-1,3}},{{-2,5,1},{2,0,0},{0,2,0},{3,1,-5}} };
//		auto voronoi = Voro.getting_dual_graph_3D(res);
//		cout << "Voronoi cells\n";
//		ofstream outdata;
//		outdata.open("voro.txt");
//		cout << voronoi.size() << "\n";
//		for (int i = 0; i < voronoi.size(); i++) {
//			for (int j = 0; j < voronoi[i].size(); j++) {
//				outdata << voronoi[i][j][0] << "," << voronoi[i][j][1] << "," << voronoi[i][j][2] << ",\n";
//			}
//		}
//		outdata.close();
//		return 0;
//	} 


} //namespace libqqc

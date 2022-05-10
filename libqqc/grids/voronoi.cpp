///
/// A uniform grid constructor
/// @file unigrid.cpp
/// @author Isabel Vinterbladh
/// @version 0.1 19-04-2022
//

#define _USE_MATH_DEFINES

#include "voronoi.h"
#include <vector>
#include <map>
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

	double Length(vector<double> a, vector<double> b) {
		double l = pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2);
		double sqrt_l = sqrt(l);
		return sqrt_l;
	}
	vector<double> U(vector<double> a, vector<double> b) {
		vector<double> u{ a[0] - b[0], a[1] - b[1], a[2] - b[2] };
		return u;
	}

	vector<double> crossProduct(vector<double> vect_A, vector<double> vect_B) {
		vector<double> cross_P(3, 0);
		cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
		cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
		cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
		return cross_P;
	}

	template<typename vec_type>
	auto Voronoi<vec_type> ::def_supertriangle_3D(vector<vector<double>>& sorted) {
		//double min_y = sorted[0][1];
		//double max_y = sorted[0][1];
		//double min_z = sorted[0][2];
		//double max_z = sorted[0][2];
		//// Finding the maximum and minimum of y and z to be able to construct the super triangle
		//for (int i = 1; i < sorted.size(); i++) {
		//	if (sorted[i][1] < min_y) {
		//		min_y = sorted[i][1];
		//	}
		//	if (sorted[i][1] > max_y) {
		//		max_y = sorted[i][1];
		//	}
		//	if (sorted[i][2] < min_z) {
		//		min_z = sorted[i][2];
		//	}
		//	if (sorted[i][2] > max_z) {
		//		max_z = sorted[i][2];
		//	}
		//}
		// adding the points to construct the super triangle/polyhedron
		vector< vector<double> > super_triangle{ {0, 0, 100}, {-100, 0, -100}, {100, -100, -100},{100, 100, -100} };
		// add test to see that all points are in super triangle
		return super_triangle;
	} // Voronoi::def_supertriangle_3D

	template<typename vec_type>
	auto Voronoi<vec_type> ::watsons_alg_3D(vector< vector<double> >& sorted, vector< vector<double> >& super_triangle) {
		// initializing vectors for triangles and edges
		vector< vector<vector<double>> > triangle_list{ super_triangle };
		vector< vector<vector<double>> > final_triangles;
		vector<vector<double>> edges;

		// loop over sorted points
		for (int s = 0; s < sorted.size(); s++) {
			// initialise first point and call "new"
			auto x_new = sorted[s][0];
			auto y_new = sorted[s][1];
			auto z_new = sorted[s][2];
			cout << "new point \n";
			// iterate through non-complete triangle list
			for (auto itr = 0; itr < triangle_list.size(); itr++) {
				// compute triangle circumcenter
				auto t = triangle_list[itr]; // Triangle coordinates
				
				vector<double> cross1 = crossProduct(U(t[2], t[0]), U(t[3], t[0]));
				vector<double> cross2 = crossProduct(U(t[3], t[0]), U(t[1], t[0]));
				vector<double> cross3 = crossProduct(U(t[1], t[0]), U(t[2], t[0]));
				double c_x = t[0][0] + (Length(t[0], t[1]) * cross1[0] + Length(t[0], t[2]) * cross2[0] + Length(t[0], t[3]) * cross3[0]) / (2 * U(t[1], t[0])[0] * cross1[0]);
				if (isinf(c_x) || isnan(c_x)) {
					c_x = 0;
				}
				double c_y = t[0][1] + (Length(t[0], t[1]) * cross1[1] + Length(t[0], t[2]) * cross2[1] + Length(t[0], t[3]) * cross3[1]) / (2 * U(t[1], t[0])[1] * cross1[1]);
				
				if (isinf(c_y) || isnan(c_y)) {
					c_y = 0;
				}
				double c_z = t[0][2] + (Length(t[0], t[1]) * cross1[2] + Length(t[0], t[2]) * cross2[2] + Length(t[0], t[3]) * cross3[2]) / (2 * U(t[1], t[0])[2] * cross1[2]);
				if (isinf(c_z) || isnan(c_z)) {
					c_z = 0;
				}
				auto R = pow((t[0][0] - c_x), 2) + pow((t[0][1] - c_y), 2) + pow((t[0][2] - c_z), 2); // squared radius of circumcircle
				auto D_x = pow((c_x - x_new), 2); // square of x-distance from x_new to triangle circumcenter
				cout << D_x << ":" << R << "\n";
				if (D_x >= R) { // checking if squared x-distance is bigger than squared radius, when true adding triangle to final triangles
				// -> point lies outside of triangle
					cout << "added triangle \n";
						final_triangles.push_back(triangle_list[itr]);
						triangle_list.erase(triangle_list.begin() + itr);// erasing triangle from incomplete triangle list
						itr--; //to not miss a triangle
				}
				
				if (D_x < R) { // when x-distance is smaller than radius
					auto D = D_x + pow((c_y - y_new), 2) + pow((c_z - z_new), 2); // calculating distane from new point to circumcenter of triangle
					cout << "if D smaller than R:"<< D << ":" << R << "\n";
					if (D < R || s==0) { // checking if distance is smaller than squared radius -> point lies within triangle -> cannot be a final triangle
						triangle_list.erase(triangle_list.begin() + itr); // erasing triangle from incomplete triangle list and adding the three edges to edges vector
						itr--;
						cout << "adding edges\n";
						edges.push_back({ t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2] });
						edges.push_back({ t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2] });
						edges.push_back({ t[2][0], t[2][1], t[2][2], t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2] });
						edges.push_back({ t[3][0], t[3][1], t[3][2], t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2] });
					}
				}
			}
			sort(edges.begin(), edges.end()); // sort edges
			edges.erase(unique(edges.begin(), edges.end()), edges.end()); // erase edges that occur multiple times
			cout << "edges:" << edges.size() << "\n";
				//from all edges create new triangles with the new point and add them as incomplete triangles
			for (int i = 0; i < edges.size(); i++) {
				vector< vector<double> > new_triangle{ {x_new,y_new, z_new} };
				new_triangle.push_back({ edges[i][0], edges[i][1], edges[i][2] });
				new_triangle.push_back({ edges[i][3], edges[i][4], edges[i][5] });
				new_triangle.push_back({ edges[i][6], edges[i][7], edges[i][8] });
				//sort(new_triangle.begin(), new_triangle.end());
				triangle_list.push_back(new_triangle);
			}
			cout << "triangles:" << triangle_list.size() <<":" << final_triangles.size() << "\n";
		}
		final_triangles.insert(final_triangles.end(), triangle_list.begin(), triangle_list.end());
		int itr = 0; // initialise counter
		while (itr < final_triangles.size() || final_triangles.size() == 0) { // execute while counter is smaller than length of final triangle list or as long as final triangle list is not zero
		continue_here:
			
			// nested for loop to remove all triangles that include one or more of the points from the super triangle
			for (int i = 0; i < 4; i++) {
				for (int f = 0; f < 4; f++) {
					if (final_triangles[itr][f] == super_triangle[i]) { // when point from super triangle found, erase triangle
						cout << "erasing " << final_triangles[itr][f][0] << ";" << final_triangles[itr][f][1] << ";" << super_triangle[i][0] << ";" << super_triangle[i][1] << ";" << "\n";
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

		for (int i = 0; i < final_triangles.size(); i++) {
			sort(final_triangles[i].begin(), final_triangles[i].end());
		}
		sort(final_triangles.begin(), final_triangles.end()); // sort triangles
		final_triangles.erase(unique(final_triangles.begin(), final_triangles.end()), final_triangles.end());
		return final_triangles;	// returning vector of Delaunay triangles
	} // Voronoi::watsons_alg_3D 

	template<typename vec_type>
	auto Voronoi<vec_type> ::getting_dual_graph_3D(vector< vector< vector<double> >>& triangles) {
		vector<vector<double>> vertex;

		for (int i = 0; i < triangles.size(); i++) {
			auto t = triangles[i]; // Triangle coordinates
			vector<double> cross1 = crossProduct(U(t[2], t[0]), U(t[3], t[0]));
			vector<double> cross2 = crossProduct(U(t[3], t[0]), U(t[1], t[0]));
			vector<double> cross3 = crossProduct(U(t[1], t[0]), U(t[2], t[0]));
			double c_x = t[0][0] + (Length(t[0], t[1]) * cross1[0] + Length(t[0], t[2]) * cross2[0] + Length(t[0], t[3]) * cross3[0]) / (2 * U(t[1], t[0])[0] * cross1[0]);
			if (isinf(c_x) || isnan(c_x)) {
				c_x = 0;
			}
			double c_y = t[0][1] + (Length(t[0], t[1]) * cross1[1] + Length(t[0], t[2]) * cross2[1] + Length(t[0], t[3]) * cross3[1]) / (2 * U(t[1], t[0])[1] * cross1[1]);
			if (isinf(c_y) || isnan(c_y)) {
				c_y = 0;
			}
			double c_z = t[0][2] + (Length(t[0], t[1]) * cross1[2] + Length(t[0], t[2]) * cross2[2] + Length(t[0], t[3]) * cross3[2]) / (2 * U(t[1], t[0])[2] * cross1[2]);
			if (isinf(c_z) || isnan(c_z)) {
				c_z = 0;
			}
			//cout << "vertex " << Length(t[0], t[1]) << ":" << Length(t[0], t[2]) << ":" << U(t[1], t[0])[1] << "\n";
			vector<double> circumcenter{ c_x, c_y, c_z };
			vertex.push_back(circumcenter);
		}
		// checking that the circumcenters for all triangles were calculated
		if (vertex.size() != triangles.size()) throw invalid_argument(
			"Must be same dimensionality of vertex and triangles.");
		
		vector< vector< vector<double> > > voronoi_cells; // initialising vector with edges for voronoi cells

		// added inner vertices
		for (int j = 0; j < vertex.size(); j++) { // loop over vertex
			for (int k = j + 1; k < triangles.size(); k++) { // loop over triangles for not already checked vertex
				for (int edge = 0; edge < 3; edge++) { // loop over the four plane-triangles of tetrahedron
					if (triangles[j][0] == triangles[k][edge] || triangles[j][1] == triangles[k][edge] || triangles[j][2] == triangles[k][edge] || triangles[j][3] == triangles[k][edge] ) {// if an edge is the same in two triangles, connect the vertex
						for (int next = edge + 1; next < 4; next++) {
							if (triangles[j][0] == triangles[k][next] || triangles[j][1] == triangles[k][next] || triangles[j][2] == triangles[k][next] || triangles[j][3] == triangles[k][next]) {// if an edge is the same in two triangles, connect the vertex
								// two verteces equal
								vector<vector < double>> edge_found{ vertex[j], vertex[k] };  // adding the edge connecting the two vertex of the triangles
								voronoi_cells.push_back(edge_found); // save edge as part of voronoi cells
								break;
							}
						}
					}
				}
			}
		}
		//adding outer vertices
		for (int i = 0; i < vertex.size(); i++) {
			int counter = 0;
			for (int j = 0; j < voronoi_cells.size(); j++) {
				counter = counter + count(voronoi_cells[j].begin(), voronoi_cells[j].end(), vertex[i]);
				if (counter == 4) {
					break;
				}
			}
			if (counter < 4) {
				voronoi_cells.push_back({ vertex[i], {5 * vertex[i][0],5 * vertex[i][1],5 * vertex[i][2]} });
			}
		}
		for (int i = 0; i < voronoi_cells.size(); i++) {
			if (voronoi_cells[i][0] == voronoi_cells[i][1]) {
				voronoi_cells.erase(voronoi_cells.begin() + i);
			}
		}
		sort(voronoi_cells.begin(), voronoi_cells.end()); // sort edges
		voronoi_cells.erase(unique(voronoi_cells.begin(), voronoi_cells.end()), voronoi_cells.end());
		return voronoi_cells;
	}// Voronoi::getting_dual_graph_3D


	//int main() {
	//	vector<int> x { 1,5,4,2,6};
	//	vector<int> y { 2,3,1,1,4};
	//	vector<int> z { 3,1,4,7,1};
	//	Voronoi<vector<int>> Voro;
	//	vector< vector<double> > result = Voro.sorting_3D(x, y, z);
	//	/*for (int i = 0; i <result.size(); i++) {
	//		cout << result[i][0]<<"," << result[i][1] << ";\n";
	//	}*/
	//
	//	vector< vector<double> > s_tri = Voro.def_supertriangle_3D(result);
	//	
	//	cout << "Super triangle \n";
	//	for (int i = 0; i < s_tri.size(); i++) {
	//		cout << s_tri[i][0] << "," << s_tri[i][1] << "," << s_tri[i][2] << " ;\n";
	//	}
	//	auto res = Voro.watsons_alg_3D(result, s_tri);
	//	cout << "Delauny triangulation \n";
	//	cout << res.size() << "\n";
	//	ofstream outd;
	//	outd.open("delaunay.txt");
	//	for (int i = 0; i < res.size(); i++) {
	//		for (int j = 0; j < res[i].size(); j++) {
	//			outd << res[i][j][0] << "," << res[i][j][1] << "," << res[i][j][2] << "\n";
	//		}
	//	}
	//	outd.close();
	//	vector< vector<vector<double>> > Del{ {{6,4,1},{1,2,3},{2,1,7},{5,3,1}},{{6,4,1},{5,3,1},{4,1,4},{1,2,3}} };
	//	auto voronoi = Voro.getting_dual_graph_3D(res);
	//	cout << "Voronoi cells \n";
	//	ofstream outdata;
	//	outdata.open("test.txt");
	//	cout << voronoi.size() << "\n";
	//	for (int i = 0; i < voronoi.size(); i++) {
	//		for (int j = 0; j < 2; j++) {
	//			outdata << voronoi[i][j][0] << "," << voronoi[i][j][1] << "," << voronoi[i][j][2] << "\n";
	//		}
	//	}
	//	outdata.close();
	//	return 0;
	//}

} //namespace libqqc

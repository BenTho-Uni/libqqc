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
#include <algorithm>
#include <cmath>
using namespace std;


template<typename vec_type>
auto Voronoi<vec_type> ::sorting(vec_type& x_pts, vec_type& y_pts){
	if (x_pts.size() != y_pts.size()) throw invalid_argument(
		"Must be same dimensionality of x and y points.");

	auto min_x = x_pts[0];
	auto min_y = y_pts[0];
	int min_iter = 0;
	
//Finding minimum of x points
	vector<double> sorted(2, 0);
	vector<vector<double>> all_sort;
	while (x_pts.size() > 0) {
		for (int i = 1; i < x_pts.size(); i++) {
			if (x_pts[i] < min_x) {
				min_x = x_pts[i];
				min_y = y_pts[i];
				min_iter = i;
			}
		}
		sorted[0] = min_x;
		sorted[1] = min_y;
		all_sort.push_back(sorted);
		x_pts.erase(x_pts.begin()+min_iter);
		y_pts.erase(y_pts.begin()+min_iter);
		//y_pts.erase(min_iter);
		if (x_pts.size() != 0) {
		min_x = x_pts[0];
		min_y = y_pts[0];
		min_iter = 0;
		}
		
	}
	return all_sort;
	}


template<typename vec_type>
auto Voronoi<vec_type> ::def_supertriangle(vector<vector<double>>& sorted) {
	double min_y = sorted[0][1];
	double max_y = sorted[0][1];
	
	for (int i = 1; i < sorted.size(); i++) {
		if (sorted[i][1] < min_y) {
			min_y = sorted[i][1];
		}
		if(sorted[i][1] >max_y){
			max_y = sorted[i][1];
		}
	}
	// adding the points (-2*min x, max y - min y),(2*max x, -2 * min y), (2*max x, 2* max y)
	vector< vector<double> > super_triangle{ {-2 * sorted[0][0],max_y - min_y}, {2 * sorted.back()[0], -2 * min_y}, {2 * sorted.back()[0], 2 * max_y}};
	// add test to see that points are in super triangle
	return super_triangle;
}

template<typename vec_type>
auto Voronoi<vec_type> ::watsons_alg(vector< vector<double> >& sorted, vector< vector<double> >& super_triangle) {
	vector< vector<vector<double>> > triangle_list { super_triangle };
	vector< vector<vector<double>> > final_triangles;

	vector<vector<double>> edges;
	//bool key = false;
	for (int s = 0; s < sorted.size(); s++) {
		auto x_new = sorted[s][0];
		auto y_new = sorted[s][1];

		for (auto itr = 0; itr < triangle_list.size(); itr++) {
			// compute triangle circumcenter
			auto t = triangle_list[itr]; // Triangle coordinates
			auto slope_1 = (t[1][1] - t[0][1]) / (t[1][0] - t[0][0]);
			auto slope_2 = (t[2][1] - t[1][1]) / (t[2][0] - t[1][0]);
			vector<double> mid_p1{ (t[0][0] + t[1][0]) / 2, (t[0][1] + t[1][1]) / 2 };
			vector<double> mid_p2{ (t[1][0] + t[2][0]) / 2, (t[1][1] + t[2][1]) / 2 };
			auto c_y = ((-1 / slope_2) * (mid_p1[0] - mid_p2[0]) + mid_p2[1] - (slope_1 / slope_2) * mid_p1[1]) / (1 - (slope_1 / slope_2));
			auto c_x = -slope_1 * (c_y - mid_p1[1]) + mid_p1[0];
			// compute circumcircle radius squared
			auto R = pow((t[0][0] - c_x), 2) + pow((t[0][1] - c_y), 2);
			auto D_x = pow((c_x - x_new), 2);

			if (D_x >= R) {
				cout << "adding triangle \n";
				final_triangles.push_back(triangle_list[itr]);
				triangle_list.erase(triangle_list.begin() + itr);
			}

			if (D_x < R) {
				auto D = D_x + pow((c_y - y_new), 2);
				if (D < R) {
					triangle_list.erase(triangle_list.begin() + itr);
					edges.push_back({ t[0][0], t[0][1], t[1][0], t[1][1] });
					edges.push_back({ t[1][0], t[1][1], t[2][0], t[2][1] });
					edges.push_back({ t[2][0], t[2][1], t[0][0], t[0][1] });
				}
			}
		}
					sort(edges.begin(), edges.end());
					edges.erase(unique(edges.begin(), edges.end()), edges.end());

						//add new triangles
						for (int i = 0; i < edges.size(); i++) {
							vector< vector<double> > new_triangle{ {x_new,y_new} };
							new_triangle.push_back({ edges[i][0], edges[i][1] });
							new_triangle.push_back({ edges[i][2], edges[i][3] });
							triangle_list.push_back(new_triangle);
						}

	}
	int itr = 0; 

while (itr < final_triangles.size() || final_triangles.size()==0) {
continue_here:
		for (int i = 0; i < 3; i++){
			for (int f = 0; f < 3; f++) {
				if (final_triangles[itr][f] == super_triangle[i]) {
					final_triangles.erase(final_triangles.begin() + itr);
					cout << "erased triangle \n";
					if (final_triangles.size() == 0) {
						goto end_while_loop;
					}
					if (itr != 0) {
					itr--;
					}
					goto continue_here;
				}
			}
		}
		itr++;
		cout << itr << " " << final_triangles.size() << "\n";
}
end_while_loop:
return final_triangles;	
}


int main() {
	vector<int> x { 1,5,4,2, 6, 5};
	vector<int> y { 2,3,1,2 ,1,7};
	Voronoi<vector<int>> Voro;
	vector< vector<double> > result = Voro.sorting(x, y);
	for (int i = 0; i <result.size(); i++) {
		cout << result[i][0]<<"," << result[i][1] << ";\n";
	}

	vector< vector<double> > s_tri = Voro.def_supertriangle(result);
	
	cout << "Super triangle \n";
	for (int i = 0; i < s_tri.size(); i++) {
		cout << s_tri[i][0] << "," << s_tri[i][1] << ";";
	}
	auto res = Voro.watsons_alg(result, s_tri);
	cout << "Delauny triangulation \n";
	cout << res.size() << "\n";
	for (int i = 0; i < res.size(); i++) {
		for (int j = 0; j < res[i].size(); j++) {
			cout << res[i][j][0] << "," << res[i][j][1] << ";";
		}
	}
	return 0;
}



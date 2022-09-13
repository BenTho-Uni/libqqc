#ifndef LIBQQC_VECTORFUNCS_H
#define LIBQQC_VECTORFUNCS_H

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

namespace libqqc {
    
class VectorFuncs {	
    private:
	
    public:
   
    ///
    /// @brief Function calculating the min and max of a set of values
    ///
    /// @details Calculating the minimum and maximum of a set of values
    ///
    /// @return the minimum and maximum value
    ///

    vector<double> min_max(vector<double> pts){
	double max = pts[0]; // finding interval of points, maximum
        double min = pts[0]; // finding interval of points, minimum
        vector<double> minmax;
	//for loop to find max and min
        for (int i = 1; i < pts.size(); i++) {
            if (pts[i] < min) {
                min = pts[i];
            }
            if (pts[i] > max) {
                max = pts[i];
            }
        }
	if( abs(min+max) < 0.00000001){
	min = -1;
	max = 1;
	}
	minmax.insert(minmax.end(),min);
	minmax.insert(minmax.end(),max);
 	return minmax;
    };
    ///
    /// @brief Function calculating the length between two points  
    ///
    /// @details Calculating the distance between two points as a double
    ///
    /// @return the distance as a double
    ///
    double Length(vector<double> vect_a, vector<double> vect_b) {
      double l = pow(vect_a[0] - vect_b[0], 2) + pow(vect_a[1] - vect_b[1], 2) + pow(vect_a[2] - vect_b[2], 2);
      double sqrt_l = sqrt(l);
      return sqrt_l;
	};
  
    ///
    /// @brief Function calculating the cross product of two vectors   
    ///
    /// @details Calculating the cross product of two vectors   
    ///
    /// @return A vector with the cross product
    ///
    vector<double> crossProduct(vector<double> vect_a, vector<double> vect_b) {
      vector<double> cross_p(3, 0);
      cross_p[0] = vect_a[1] * vect_b[2] - vect_a[2] * vect_b[1];
      cross_p[1] = vect_a[2] * vect_b[0] - vect_a[0] * vect_b[2];
      cross_p[2] = vect_a[0] * vect_b[1] - vect_a[1] * vect_b[0];
      return cross_p;
	};
    ///
    /// @brief Function constructing the matrix used for circumcenter calculations 
    ///
    /// @details Constructing matrix with all 4 points from tetrahedron and combinations to use matrix for circumcenter calculations
    ///
    /// @return A 4x5 matrix for a specific tetrahedron
    ///
    vector<vector<double>> Det_Matrix(vector<vector<double>> triangle){
         vector<vector<double>> M;
         vector <double> m_vec;
         double* m = new double[4];
         for(int j=0; j <4; j++){
                m[j] = pow(triangle[j][0],2) + pow(triangle[j][1],2) + pow(triangle[j][2],2);
                }
         m_vec.insert(m_vec.begin(), m, m + 4 );
         M.push_back(m_vec);
         int tri_counter=0;
         for (int i=1; i<4; i++){
                for(int j=0; j <4; j++){
                m[j] = triangle[j][tri_counter];
                }
                tri_counter++;
                m_vec.insert(m_vec.begin(), m, m + 4 );
                M.push_back(m_vec);              
                }
         for (int j=0; j<4; j++){
                m[j] =1 ;
                }
        m_vec.insert(m_vec.begin(), m, m + 4 );
        M.push_back(m_vec);
        delete[] m;
        return M;
        };
    ///
    /// @brief Function calculating the determinant 
    ///
    /// @details Function calculating the determinant for a 4x4 matrix
    ///
    /// @return the determinant of the 4x4 matrix
    ///
    double Determinant(vector<vector<double>>  mat){
            double det= 0;
            for (int a = 0; a < 4; a++) {
                    double sub_sum =  0;
                    for (int b = 0; b < 4 ; b++) {
                            if (b != a){
                            vector<int> index {0,1,2,3};
                            vector<int> used {a,b};
                            vector<int> difference(2);
                            set_symmetric_difference(index.begin(), index.end(), used.begin(), used.end(), difference.begin());
                            int c = difference[0];
                            int d = difference[1];
                            sub_sum = sub_sum + pow(-1, b)*mat[b][1] * ((mat[c][2] * mat[d][3]) - (mat[d][2] * mat[c][3]));
                            }
                    }
                    det = det + pow(-1,a)*mat[a][0]* sub_sum;
            }
       return det;
   };

   };
} // namespace libqqc

#endif

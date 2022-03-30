#ifndef LIBQQC_QMP2_ENERGY_H
#define LIBQQC_QMP2_ENERGY_H

#include <stddef.h> //needed for size_t
#include <Eigen>
#include <vector>

using namespace std;
using namespace Eigen;

namespace libqqc {

    ///
    /// @brief class which handles the computation of Q-MP2 energies
    ///
    /// @details  This class handles the computational of the ground state 
    /// Q-MP2 energies
    ///
    class Qmp2_energy {
        private:
            size_t &m1Dnpts; ///< Number of points on 1D grid
            size_t &m3Dnpts; ///< Number of points on 3D grid
            size_t &mnocc; ///< Number of occupied orbitals
            size_t &mnvirt; ///< Number of virtual orbitals

            double *mmo = NULL; ///< Pointer to occupied orbital matrix
            double *mmv = NULL; ///< Pointer to virtual orbital matrix
            double *mc_c = NULL; ///< Pointer to tensor of coulomb orbitals
            double *mm1Deps_o = NULL; ///< Pointer to matrix-array of 1D exponent factors of occupied energies
            double *mm1Deps_v = NULL; ///< Pointer to matrix-array of 1D exponent factors of virtual energies
            double *mm1Deps_ov = NULL; ///< Pointer to matrix-array of 1D exponent factors

            double *mvf = NULL; ///< Pointer to array of fock energies
            double *mv1Dpts = NULL; ///< Pointer to array of 1D grid point
            double *mv1Dwts = NULL; ///< Pointer to array of 1D grid point weights
            double *mv3Dwts = NULL; ///< Pointer to array of 3D grid point weights

            size_t &moffset; ///< Offset of 3D grid pts to start calculation from
            size_t &mnpts_to_proc; ///< Numer of 3D grid pts to process in this iteration

        public: 
            ///
            /// @brief Constructor setting needed data
            ///
            /// @details This constructor sets the data which is required for 
            /// the calculation from reference and pointers
            ///
            /// @param[in] &p1Dnpts reference to number of 1D points
            /// @param[in] &p3Dnts reference to number of 3D poitns
            /// @param[in] &nocc reference to number of occupied orbitals
            /// @param[in] &nvirt reference to number of virtual orbitals
            /// @param[in] *mo pointer to matrix-array of occupied orbitals
            /// @param[in] *mv pointer to matrix-array of virtual orbitals
            /// @param[in] *c_c pointer to tensor-array of coulomb integral
            /// @param[in] *m1Deps_o pointer to matrix-array of exponent factors of occupied energies
            /// @param[in] *m1Deos_v pointer to matrix-array of exponent factors of virtual energies
            /// @param[in] *m1Deps_ov pointer to matrix-array of exponent factors 
            /// @param[in] *vf pointer to array of fock energies
            /// @param[in] *v1Dpts pointer to array of 1D grid point weights
            /// @param[in] *v1Dwts pointer to array of 1D grid point weights
            /// @param[in] *v3Dwts pointer to array of 3D grid point weights
            /// @param[in] &offset reference to number of offset elements
            /// @param[in] &npts_to_proc number of points to process in this iteration
            ///
            Qmp2_energy (size_t &p1Dnpts, size_t &p3Dnpts, size_t &nocc, 
                    size_t &nvirt, double *mo, double *mv, double *c_c,
                    double *m1Deps_o, double *m1Deps_v, double *m1Deps_ov, 
                    double *vf, double *v1Dpts, double *v1Dwts, double *v3Dwts, 
                    size_t &offset, 
                    size_t &npts_to_proc) : m1Dnpts(p1Dnpts), 
            m3Dnpts(p3Dnpts), mnocc(nocc), mnvirt(nvirt), mmo(mo),
            mmv(mv), mc_c(c_c), mm1Deps_o(m1Deps_o), mm1Deps_v(m1Deps_v),
            mm1Deps_ov(m1Deps_ov), mvf(vf), mv1Dpts(v1Dpts), 
            mv1Dwts(v1Dwts), mv3Dwts (v3Dwts), moffset(offset), 
            mnpts_to_proc(npts_to_proc){
            //Precalc. Scale all slices of the integral by the weight of 
            //each point
            //gg
            #pragma omp parallel for schedule(static) default(none)\
                            shared(m3Dnpts, mnocc, mnvirt, mc_c, mv3Dwts)\
                            collapse(2)
            for (size_t p = 0; p < m3Dnpts; p++){
                for (size_t i = 0; i < mnocc; i++){
                    for (size_t a = 0; a < mnvirt; a++){
                        mc_c [p * mnvirt * mnocc + i * mnvirt + a] *=
                            mv3Dwts[p];
                    }
                }
            }
        };

            ///
            /// @brief computes the energy
            ///
            /// @details This function computes the Q-MP2 energy of a system.
            ///
            /// @return e_mp2 Energie of system
            ///
            double compute (){
                //Setting um the Vector and Matrix Maps to use Eigen methods
                using MatMap = Map<Matrix <double, Dynamic, Dynamic, RowMajor>>;
                using VecMap = Map<VectorXd>;
                
                // Map memory pointer to Eigen Maps
                MatMap map_mmo(mmo, m3Dnpts, mnocc); // occupied orbitals
                MatMap map_mmv(mmv, m3Dnpts, mnvirt); // virtual orbitals 
                MatMap map_mm1Deps_o(mm1Deps_o, m1Dnpts, mnocc); // prefactors
                MatMap map_mm1Deps_v(mm1Deps_v, m1Dnpts, mnvirt); // prefactors

                // Create vector of maps of three dimensional 1Deps_ov 
                vector<MatMap> map_mm1Deps_ov; // prefactors
                map_mm1Deps_ov.reserve(m1Dnpts);
                for (size_t i = 0; i < m1Dnpts; i++){
                    map_mm1Deps_ov.push_back(
                        MatMap(mm1Deps_ov + i * mnocc * mnvirt, mnocc, mnvirt));
                }

                VecMap map_mvf(mvf, mnocc+mnvirt); // orbital energies
                VecMap map_mv1Dpts(mv1Dpts, m1Dnpts); // 1D grid points
                VecMap map_mv1Dwts(mv1Dwts, m1Dnpts); // 1D grid point weights

                VectorXd o_p; //empty Vectors, so that each OpenMP thread
                VectorXd v_p; // can use a private copy
                MatrixXd c2_p;

                double energy = 0;
#pragma omp parallel for reduction(+:energy) schedule(dynamic) default(none)\
                private(o_p, v_p, c2_p)\
                shared(moffset, mnpts_to_proc, m1Dnpts, m3Dnpts, mnocc, mnvirt, \
                        map_mv1Dwts, map_mmo, map_mm1Deps_o, map_mmv, \
                        map_mm1Deps_v, mc_c, map_mm1Deps_ov)\
                collapse(2)
                // loop over points on 1D grid
                for (int k = 0; k < m1Dnpts; k++){
                    // loop over all points on 3D grid in set of elements
                    // to compute from offset position
                    for (int p = moffset; p < moffset + mnpts_to_proc; p++){
                        // create local copy of with prefactor scaled orbitals
                        o_p = map_mmo.row(p)
                            .cwiseProduct(map_mm1Deps_o.row(k));
                        v_p = map_mmv.row(p)
                            .cwiseProduct(map_mm1Deps_v.row(k));
                        c2_p = MatMap(mc_c + p * mnocc * mnvirt, mnocc, mnvirt)
                            .cwiseProduct(map_mm1Deps_ov[k]);

                        // loop over inner grid points, utilizing symmetry
                        for (int q = 0; q <= p; q++){
                            // local copy of orbitals to minimize transpose
                            VectorXd o_q = map_mmo.row(q).transpose();
                            VectorXd v_q = map_mmv.row(q).transpose();
                            MatMap map_mc_c_q(mc_c + q * mnocc * mnvirt, 
                                    mnocc, mnvirt);

                            double jo = 0;
                            for (int a = 0; a < mnvirt; a++){
                                double tmp1 = o_p.dot(map_mc_c_q.col(a));
                                double tmp2 = o_q.dot(c2_p.col(a));
                                jo += tmp1 * tmp2;
                            }//a 
                            
                            double j = (c2_p.cwiseProduct(map_mc_c_q)).sum();
                            
                            double o = (o_p).dot(o_q);
                            
                            double v = (v_p).dot(v_q);
                            
                            double sum = (jo - 2 * j * o) * v;
                           
                            // multiplication of p,q pair function is commutative
                            // so we calculate only unique pairs, so only half
                            // of the duples and double them 
                            if (p != q){
                                sum *= 2.0;
                            }//if

                            // weight the energy contribution of 1D grid 
                            // point
                            energy += map_mv1Dwts(k) * sum; 
                        }//for q
                    }//p
                }//k

                return energy;
            };
    };

}

#endif //LIBQQC_QMP2_ENERGY_H

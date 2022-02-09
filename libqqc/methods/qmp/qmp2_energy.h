#ifndef LIBQQC_QMP2_ENERGY_H
#define LIBQQC_QMP2_ENERGY_H

#include <stddef.h> //needed for size_t

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
            size_t &m3Dnpts; ///< Number of pointson 3D grid
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
                    mv1Dwts(v1Dwts), mv3Dwts (v3Dwts), moffset(offset), mnpts_to_proc(npts_to_proc)
                    {};

            ///
            /// @brief computes the energy
            ///
            /// @details This function computes the Q-MP2 energy of a system.
            ///
            /// @return e_mp2 Energie of system
            ///
            double compute (){

                //Precalc. Scale all slices of the integral by the weight of 
                //each point
                //
#pragma omp parallel for schedule(dynamic) default(none)\
                shared(m3Dnpts, mnocc, mnvirt, mc_c, mv3Dwts)\
                collapse(3)
                for (size_t p = 0; p < m3Dnpts; p++){
                    for (size_t i = 0; i < mnocc; i++){
                        for (size_t a = 0; a < mnvirt; a++){
                            mc_c [p * mnvirt * mnocc + i * mnvirt + a] *=
                                mv3Dwts[p];
                        }
                    }
                }

                double e_mp2 = 0;
#pragma omp parallel for reduction(+:e_mp2) schedule(dynamic) default(none)\
    shared(moffset, mnpts_to_proc, m1Dnpts, m3Dnpts, mnocc, mnvirt, mv1Dwts, mmo, mm1Deps_o, mmv, mm1Deps_v, mc_c, mm1Deps_ov)\
    collapse(2)
                // We loop over the points on the 1D grid quadrature of 
                // the variable used to remove the energy in the denominator
                // These are generally <10 points, so no need to mass. parralelize
                // over this. 
                for (size_t k = 0; k < m1Dnpts; k++){
                    // Then we loop over the points on the 3D grid, 
                    // which are indipendent of each other and can be parallelized
                    for (size_t p = moffset; p < moffset+mnpts_to_proc; p++){
                        // First off we scale a copy of the orbitals and integrals
                        // by the exponent of our energies, which we prescaled,
                        // as exp functions are expensive
                        double v_o_p[mnocc];
                        double v_v_p[mnvirt];
                        double m_c_p[mnocc][mnvirt];
                        for (size_t i = 0; i < mnocc; i++) 
                            v_o_p[i] = mmo[p * mnocc + i] 
                                * mm1Deps_o[k * mnocc + i]; 
                        for (size_t a = 0; a < mnvirt; a++) 
                            v_v_p[a] = mmv[p * mnvirt + a] 
                                * mm1Deps_v[k * mnvirt + a]; 
                        for (size_t i =0; i < mnocc; i++) {
                            for (size_t a = 0; a < mnvirt; a++){
                                m_c_p[i][a] = 
                                    mc_c [p * mnocc * mnvirt + i * mnvirt + a] *
                                    mm1Deps_ov[k * mnocc * mnvirt + i * mnvirt + a]; 
                            }//for a
                        }//for i

                        // And then we loop over the inner 3D grid points. 
                        // These are symmetrix, so we only need to compute 
                        // one triangle of the "matrix". 
                        for (size_t q = 0; q <= p; q++){

                            // jo, j, o, v are the names for the parts of the 
                            // calculations in the original derivation
                            // by Bloomfield. But this is basically matrix 
                            // multiplications, with as much cache alignment
                            // as we ca. 
                            double jo = 0;
                            for (size_t a = 0; a < mnvirt; a++){
                                // we have here two intermediates we can precalc
                                // which saves some cycles
                                double tmp1 = 0;
                                double tmp2 = 0;
                                for (size_t i = 0; i < mnocc; i++){
                                    // these terms are the only ones with
                                    // problematic cache alignments, 
                                    // as we have to move in a and i 
                                    // direction simultaniously. 
                                    // Could be remedied by having a transposed
                                    // copy, but this increases the data footprint.
                                    tmp1 += v_o_p[i] *
                                        mc_c[q * mnocc * mnvirt + i * mnvirt + a];
                                    tmp2 += mmo[q*mnocc+i] * m_c_p[i][a];
                                }//for 
                                jo += tmp1 * tmp2;
                            }//for a
                            
                            //calc j
                            double j = 0;
                            for (size_t i = 0; i < mnocc; i++){
                                for (size_t a = 0; a < mnvirt; a++){
                                    j += m_c_p[i][a] 
                                        * mc_c[q * mnocc * mnvirt + i * mnvirt+a];
                                }
                            }//for i

                            // calc o
                            double o = 0;
                            for (size_t i = 0; i < mnocc; i++){
                                o += v_o_p[i] *  mmo[q * mnocc + i];
                            }//for i

                            // calc v
                            double v = 0;
                            for (size_t a = 0; a < mnvirt; a++){
                                v += v_v_p[a] * mmv[q * mnvirt + a];
                            }

                            double sum = (jo - 2 * j * o) * v;

                            // here we finish exploiting the symmetry and account
                            // for the double values
                            if (p != q){
                                sum *= 2.0;
                            }

                            // and finally we weight the energy part for this 
                            // point on the 1D grid.
                            e_mp2 += sum * mv1Dwts[k];
                        }//q for
                    }//p
                }//for k 
                return e_mp2;
            };//calc_mp2
    };
}

#endif //LIBQQC_QMP2_ENERGY_H

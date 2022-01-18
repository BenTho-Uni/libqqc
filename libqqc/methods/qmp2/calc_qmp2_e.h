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
            
            int &moffset; ///< Offset of 3D grid pts to start calculation from
            int &mnpts_to_proc; ///< Numer of 3D grid pts to process in this iteration

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
            /// @param[in] &offset reference to number of offset elements
            /// @param[in] &npts_to_proc number of points to process in this iteration
            ///
            Qmp2_energy (size_t &p1Dnpts, size_t &p3Dnpts, size_t &nocc, 
                    size_t &nvirt, double *mo, double *mv, double *c_c,
                    double *m1Deps_o, double *m1Deps_v, double *m1Deps_ov, 
                    double *vf, double *v1Dpts, double *v1Dwts, int &offset, 
                    int &npts_to_proc) : m1Dnpts(p1Dnpts), 
                    m3Dnpts(p3Dnpts), mnocc(nocc), mnvirt(nvirt), mmo(mo),
                    mmv(mv), mc_c(c_c), mm1Deps_o(m1Deps_o), mm1Deps_v(m1Deps_v),
                    mm1Deps_ov(m1Deps_ov), mvf(vf), mv1Dpts(v1Dpts), 
                    mv1Dwts(v1Dwts), moffset(offset), mnpts_to_proc(npts_to_proc)
                    {};

            ///
            /// @brief computes the energy
            ///
            /// @details This function computes the Q-MP2 energy of a system
            ///
            /// @param[in,out] ene Energie of system
            ///
            void compute (double &ene){

                ene = 0.0;
#pragma omp parallel for reduction(+:e_mp2) schedule(dynamic) default(none)\
    shared(offset, elements_per_proc, tsize, npts, occ, virt, v_twts, m_o, m_teps_o, m_v, m_teps_v, c_c, c_teps)\
    collapse(2)
                for (size_t k = 0; k < tsize; k++){
                    for (int p = offset; p < offset+elements_per_proc; p++){
                        //scale o_p, v_p, c2_p by e term
                        //
                        double v_o_p[occ];
                        double v_v_p[virt];
                        double m_c_p[occ][virt];
                        for (size_t i = 0; i < occ; i++) 
                            v_o_p [i] = m_o [p*occ+i] * m_teps_o[k*occ+i]; 
                        for (size_t a = 0; a < virt; a++) 
                            v_v_p [a] = m_v [p*virt+a] * m_teps_v[k*virt+a]; 
                        for (size_t i =0; i < occ; i++) {
                            for (size_t a = 0; a < virt; a++){
                                m_c_p [i][a] = c_c [p*occ*virt+i*virt+a] 
                                    * c_teps[k*occ*virt+i*virt+a]; 
                            }//for a
                        }//for i

                        for (int q = 0; q <= p; q++){

                            double jo = 0;
                            for (size_t a = 0; a < virt; a++){
                                //calc temp 1 and temp 2
                                double tmp1 = 0;
                                double tmp2 = 0;
                                for (size_t i = 0; i < occ; i++){
                                    tmp1 += v_o_p[i] * c_c[q*occ*virt+i*virt+a];
                                    tmp2 += m_o[q*occ+i] * m_c_p[i][a];
                                }//for 
                                jo += tmp1 * tmp2;
                            }//for a
                            //calc j
                            double j = 0;
                            for (size_t i = 0; i < occ; i++){
                                for (size_t a = 0; a < virt; a++){
                                    j += m_c_p[i][a]*c_c[q*occ*virt+i*virt+a];
                                }
                            }//for i
                            //Calc o
                            double o = 0;
                            for (size_t i = 0; i <occ; i++){
                                o += v_o_p[i]*m_o[q*occ+i];
                            }//for i
                            //calc v
                            double v = 0;
                            for (size_t a = 0; a < virt; a++){
                                v += v_v_p[a] * m_v[q*virt+a];
                            }

                            double sum = (jo - 2 * j * o) * v;

                            if (p != q){
                                sum *= 2.0;
                            }//if double counting

                            //calc e_mp2 by multiplying with tweights.  
                            e_mp2 += sum * v_twts[k];
                        }//q for
                    }//p
                }//for k 
                return e_mp2;
            };//calc_mp2
    };
}

#endif //LIBQQC_QMP2_ENERGY_H

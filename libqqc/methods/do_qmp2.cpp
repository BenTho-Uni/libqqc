///
/// Methods for the setup of qmp2
/// @file do_mp2.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 01-01-2020
//

#include "do_qmp2.h"

//internal headers

//external headers
#include <cmath>

using namespace std;

namespace libqqc {

    void Do_qmp2 :: run(ostringstream &out){
    
        // Grabbing the calculation data we need
        size_t p1Dnpts = mvault.get_m1Dgrid().get_mnpts();
        size_t p3Dnpts = mvault.get_m3Dgrid().get_mnpts();
        size_t nocc = mvault.get_mnocc();
        size_t nvirt = mvault.get_mnvirt();
        size_t nmo = nocc + nvirt;
        size_t nao = mvault.get_mnao(); 

        double* mcoeff = mvault.get_mmat_coeff();
        double* mfao = mvault.get_mmat_fock();
        double* mcgto = mvault.get_mmat_cgto();
        double* ccao = mvault.get_mcube_coul();
        double* v1Dpts = mvault.get_m1Dgrid().get_mpts();
        double* v1Dwts = mvault.get_m1Dgrid().get_mwts();
        double* v3Dwts = mvault.get_m3Dgrid().get_mwts();

        // setting up the MO quantaties and calculating them

        double* m1Deps_o = new double[p1Dnpts * nocc]();
        double* m1Deps_v = new double[p1Dnpts * nvirt]();
        double* c1Deps_ov = new double[p1Dnpts * nocc * nvirt]();
        double* m_o = new double[p3Dnpts * nocc]();
        double* m_v = new double[p3Dnpts * nvirt]();
        double* c_c = new double[p3Dnpts * nocc * nvirt]();
        double vf[nmo] = {};

        // AO to MO transformations
        // Fock-Matrix F $F_{MO} = C^T F_{AO} C
        // 
        // Save a Transpose of the Matrix for better cache alignment
        //
        double mcoeff_t[ nao * nmo];

#pragma omp parallel for schedule(dynamic) default(none)\
        shared(nao, nmo, mcoeff_t, mcoeff)\
        collapse(2)
        for (size_t i = 0; i < nao; i++){
            for (size_t j = 0; j < nmo; j++){
                mcoeff_t[j * nao + i ] = mcoeff [i * nmo + j];
            }
        }

// This could be collapse(2) but needs a reduction on vf[p]
#pragma omp parallel for reduction(+:vf) schedule(dynamic) default(none)\
        shared(nmo, nao, mfao, mcoeff_t)\
        collapse(2)
        for (size_t p = 0; p < nmo; p ++){
            for (size_t l = 0; l < nao; l++){
                double temp = 0;
                for (size_t k = 0; k < nao; k++){
                    // Matrix Multiplication A*B multiplies the Row of A with 
                    // Column of B. That is a Problem in B, as we have filled it 
                    // Row major which would lead to cache misses. 
                    // We therefore traferce over the transposed matrix instead, 
                    // as we don't have to adhere to simulating "matrix multiplication"
                    // and multiply row A with row of B^T
                    temp += mfao[l * nao + k] * mcoeff_t[p * nao + k];
                }
                vf[p] += mcoeff_t[p * nao + l] * temp;
            }
        }


        // Orbitals O $O_{MO} = O * C$
        //
        size_t pos = 0; // Position on virtual orbital space
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(p3Dnpts, nmo, nao, nocc, nvirt, m_o, m_v, mcgto, mcoeff_t)\
        private(pos) \
        collapse(2)
        for (size_t p = 0; p < p3Dnpts; p++){
            for (size_t q = 0; q < nmo; q++){
                for (size_t k = 0; k < nao; k++){
                    if (q < nocc){
                        m_o[p * nocc + q] += mcgto[p * nao + k] 
                            * mcoeff_t[q * nao + k];
                    }
                    else {
                        pos = q - nocc; // q covers nmo, so substr. num. of occ.
                        m_v[p * nvirt + pos] += mcgto[p * nao + k] 
                            * mcoeff_t[q * nao + k];
                    }
                }
            }
        }

        // (weighted) Coulomb Integral U_{MO}^P: for each slice P 
        // $U_{MO} = rwts^P * C_{occpuid}^T * (u_{AO}^P * C_{virtuals}
        //
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(p3Dnpts, nocc, nvirt, nao, ccao, mcoeff_t, c_c, v3Dwts)\
        collapse(3)
        for (size_t p = 0; p < p3Dnpts; p++){
            for (size_t i = 0; i < nocc; i++){
                for (size_t a = 0; a < nvirt; a++){
                    size_t pos_a = nocc + a;
                    for (size_t l = 0; l < nao; l++){
                        double temp = 0;
                        for (size_t k = 0; k < nao; k++){
                           temp += ccao[p * nao * nao + l * nao + k] 
                               * mcoeff_t[pos_a * nao + k];
                        }
                        c_c [p * nvirt * nocc + i * nvirt + a] += v3Dwts [p] *
                            mcoeff_t[i * nao + l] * temp;
                    }
                }
            }
        }

        // Precalculating the exponential factors
#pragma omp parallel for schedule(dynamic) default(none)\
        shared(p1Dnpts, nocc, nvirt, m1Deps_o, m1Deps_v, c1Deps_ov, \
                v1Dpts, vf)
        for (size_t k = 0; k < p1Dnpts; k++){
            for (size_t i = 0; i < nocc; i++){
                m1Deps_o[k * nocc+ i] = 
                    pow((double) v1Dpts[k], (double) (- vf[i]));
            }//for i

            for (size_t a = 0; a < nvirt; a++){
                size_t pos_f = a + nocc;
                m1Deps_v[k * nvirt + a] = 
                    pow((double) v1Dpts[k], (double) ( vf[pos_f]));
            }//for a

            double inv_t = 1.0 / (double) v1Dpts[k];
            for (size_t i = 0; i < nocc; i++){
                for (size_t a = 0; a < nvirt; a++){
                    c1Deps_ov[k * nocc * nvirt + i * nvirt + a] = 
                        m1Deps_o[k * nocc + i] * m1Deps_v[k * nvirt + a] * inv_t;
                }//for a     
            }//for i
        }//for k 
        
        size_t offset = 0;
        size_t npts_to_proc = p3Dnpts;
        double energy = 0.0;


        Qmp2_energy  qmp2_energy(
                p1Dnpts, 
                p3Dnpts, 
                nocc, 
                nvirt,
                m_o,
                m_v,
                c_c,
                m1Deps_o,
                m1Deps_v,
                c1Deps_ov, 
                vf,
                v1Dpts,
                v1Dwts,
                offset,
                npts_to_proc);

        energy = qmp2_energy.compute();

        out << endl;
        out << "Q-MP(2) Ground State Energy : " << energy << endl;

        delete[] c_c;
        delete[] m_v;
        delete[] m_o;
        delete[] c1Deps_ov;
        delete[] m1Deps_v;
        delete[] m1Deps_o;

    } //Do_qmp2::member_fn

} //namespace libqqc

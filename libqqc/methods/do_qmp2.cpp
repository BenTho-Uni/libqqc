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
#include "../utils/ttimer.h"

using namespace std;

namespace libqqc {

    void Do_qmp2 :: run(ostringstream &out){

        Ttimer timings(0);

        // Grabbing the calculation data we need
        size_t p1Dnpts = mvault.get_m1Dgrid().get_mnpts();
        size_t p3Dnpts = mvault.get_m3Dgrid().get_mnpts();
        size_t nocc = mvault.get_mnocc();
        size_t nvirt = mvault.get_mnvirt();
        size_t nmo = nocc + nvirt;
        size_t nao = mvault.get_mnao(); 

        double* mcgto = mvault.get_mmat_cgto();
        double* mf = mvault.get_mmat_fock();
        double* c_c = mvault.get_mcube_coul();
        double* v1Dpts = mvault.get_m1Dgrid().get_mpts();
        double* v1Dwts = mvault.get_m1Dgrid().get_mwts();
        double* v3Dwts = mvault.get_m3Dgrid().get_mwts();

        // setting up the MO quantaties and calculating them

        double* m1Deps_o = new double[p1Dnpts * nocc]();
        double* m1Deps_v = new double[p1Dnpts * nvirt]();
        double* c1Deps_ov = new double[p1Dnpts * nocc * nvirt]();
        double* m_o = new double[p3Dnpts * nocc]();
        double* m_v = new double[p3Dnpts * nvirt]();
        double vf[nmo] = {};

        // Reading in the Diagonal of the MO Fock matrix
        for (size_t p = 0; p < nmo; p++){
            vf[p] = mf[p * nmo + p];
        }

        // Part CGTO matrix to m_o and m_v
        //
        for (size_t p = 0; p < p3Dnpts; p++){
            for (size_t m = 0; m < nmo; m++){
                if (m < nocc) 
                    m_o[p * nocc + m] = mcgto[p * nmo + m];
                else {
                    size_t pos = m - nocc;
                    m_v[p * nvirt + pos] = mcgto[p * nmo + m];
                }
            }
        }

        //print out for debugging
        cout << "fock" << endl;
        for (int i = 0; i < nmo; i++){
            for (int j = 0; j < nmo; j++){
                cout << mf[i * nmo + j] << 
                    ((j == (nmo-1)) ? "\n" : "\t");
            }
        }
        cout << endl;

        cout << "m_o" << endl;
        for (int p = 0; p < 1; p ++){
            for (size_t m = 0; m < nocc; m++){
                cout << m_o [p * nocc + m] <<
                    ((m == (nocc-1)) ? "\n" : "\t");
            }
        }

        cout << "m_v" << endl;
        for (int p = 0; p < 1; p ++){
            for (size_t m = 0; m < nvirt; m++){
                cout << m_v [p * nvirt + m] <<
                    ((m == (nvirt-1)) ? "\n" : "\t");
            }
        }

        cout << "c_c" << endl;
        for (int p = 0; p < 1; p++){
            for (size_t o = 0; o < nocc; o++){
                for (size_t a = 0; a < nvirt; a++){
                    cout << c_c[p * nocc * nvirt + o * nvirt + a] <<
                        ((a == (nvirt-1)) ? "\n" : "\t");
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

        timings.start_new_clock("Timing Qmp2_energy::compute : ", 1, 0);
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
                v3Dwts,
                offset,
                npts_to_proc);

        energy = qmp2_energy.compute();

        timings.stop_clock(1);
        cout << timings.print_clocks(1);


        out << endl;
        out << "Q-MP(2) Ground State Energy (eV): " << energy;

        delete[] m_v;
        delete[] m_o;
        delete[] c1Deps_ov;
        delete[] m1Deps_v;
        delete[] m1Deps_o;

    } //Do_qmp2::member_fn

} //namespace libqqc

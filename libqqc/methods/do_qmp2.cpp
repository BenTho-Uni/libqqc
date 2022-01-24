///
/// Methods for the setup of qmp2
/// @file do_mp2.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 01-01-2020
//

#include "do_qmp2.h"

//internal headers
#include "qmp/qmp2_energy.h"

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

        // setting up the MO quantaties and calculating them
        double* mcoeff = mvault.get_mmat_coeff();
        double* m_o = new double[p3Dnpts * nocc]();
        double* m_v = new double[p3Dnpts * nvirt]();
        double* c_c = new double[p3Dnpts * nocc * nvirt]();
        double vf[nmo];
        double v1Dpts[p1Dnpts];
        double v1Dwts[p1Dnpts];

        double* m1Deps_o = new double[p1Dnpts * nocc]();
        double* m1Deps_v = new double[p1Dnpts * nvirt]();
        double* c1Deps_ov = new double[p1Dnpts * nocc * nvirt]();

        //mo transformations

        // Precalculating the exponential factors
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

        double energy = 0.0;
        energy = qmp2_energy.compute();

        out << endl;
        out << "Q-MP(2) Ground State Energy : " << energy << endl;

        delete[] m_o;
        delete[] m_v;
        delete[] c_c;
        delete[] m1Deps_o;
        delete[] m1Deps_v;
        delete[] c1Deps_ov;

    } //Do_qmp2::member_fn

} //namespace libqqc

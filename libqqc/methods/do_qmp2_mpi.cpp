///
/// Methods for the MPI setup of qmp2
/// @file do_mp2_mpi.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 01-01-2020
//

#include "do_qmp2.h"

//internal headers
#include "../utils/ttimer.h"

//external headers
#include <cmath>
#include <mpi.h>

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

        // Set up MPI environment and set important variables
        //MPI_Init (NULL, NULL); //Initialize MPI
        int pid, max_id; // pid is rank and max_id is maxrank
        MPI_Comm_rank(MPI_COMM_WORLD, &pid);
        MPI_Comm_size(MPI_COMM_WORLD, &max_id);
        MPI_Status status;

	timings.start_new_clock("Timings do_mp2::run AO to MO transformations : ", 0, 0);

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
        shared(p3Dnpts, nocc, nvirt, nao, ccao, mcoeff_t, c_c)\
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
                        c_c [p * nvirt * nocc + i * nvirt + a] += 
                            mcoeff_t[i * nao + l] * temp;
                    }
                }
            }
        }
	timings.stop_clock(0);

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
        size_t npts_to_proc = p3Dnpts/(2*max_id);
        size_t remaining_elements = p3Dnpts - npts_to_proc * max_id * 2;

        if ((pid == 0) && (mvault.get_mprnt_lvl() >=1)){
            cout << "Master (pid 0) reporting:" << endl 
                <<"There are " << p3Dnpts << " elements and " << max_id 
                << ((max_id == 1) ? " node." : " nodes.") << endl;
            cout << "Elements to process per node -> 2x" 
                << npts_to_proc << ", remaining elements -> " 
                << remaining_elements << endl;
        }

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

        // First batch of points from the beginning of the data, easier work load
	timings.start_new_clock("Starting batch : ", 2, 0);

	// Now we distribute the middle points left over to the first batch, this will make
	// the workload slightly uneven
	size_t npts_to_proc_orig = npts_to_proc;
    int x = remaining_elements % max_id;
    int y = remaining_elements / max_id;
	offset = pid * npts_to_proc + ((pid < x) ? pid : x) + ((y == 1) ? pid : 0);
    npts_to_proc += ((pid < x) ? (1+y) : 0);

	cout << "Node " << pid << " reporting offset: " << offset << " npts_to_proc: " << npts_to_proc << endl;
        energy += qmp2_energy.compute();
	npts_to_proc = npts_to_proc_orig;
	timings.stop_clock(2);

        // Second set of points 
	timings.start_new_clock("End batch : ", 3, 0);
        offset = p3Dnpts - (1 + pid) * npts_to_proc;
	cout << "Node " << pid << " reporting offset: " << offset << " npts_to_proc: " << npts_to_proc << endl;
        energy += qmp2_energy.compute();
	timings.stop_clock(3);

        //now lets differentiate which node does what
        if (pid == 0){
	    timings.start_new_clock("Gathering partial energies from nodes : ", 4, 0);
            double tmp = 0.0;
            // Get all partial energies from servants
            for (int i = 1; i < max_id; i++){
                MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, 
                        MPI_COMM_WORLD, &status);
                energy += tmp;
            }
	    timings.stop_clock(4);

            // Calculate missing elements. 
            // TODO: parallelize this!
	    //timings.start_new_clock("Calculate missing middle batch on master node : ", 5, 0);
            //offset = max_id * npts_to_proc;
            //npts_to_proc = remaining_elements;
            //energy += qmp2_energy.compute();
	    //timings.stop_clock(5);

	    out << timings.print_all_clocks();

            out << endl;
            out << "Q-MP(2) Ground State Energy (eV): " << energy;
        } else{
            // Send servant energies to master
            MPI_Send(&energy, 1, MPI_DOUBLE, 0,0, MPI_COMM_WORLD); 
        }


        delete[] c_c;
        delete[] m_v;
        delete[] m_o;
        delete[] c1Deps_ov;
        delete[] m1Deps_v;
        delete[] m1Deps_o;

    } //Do_qmp2::member_fn

} //namespace libqqc

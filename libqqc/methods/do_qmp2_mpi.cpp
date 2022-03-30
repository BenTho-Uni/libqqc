///
/// Methods for the MPI setup of qmp2
/// @file do_mp2_mpi.cpp
/// @author Benjamin Thomitzni
/// @version 0.1 01-01-2020
//

#include "do_qmp2.h"

//internal headers
#include "../utils/ttimer.h"
#include "../utils/printers/printer_qmp2.h"

//external headers
#include <cmath>
#include <mpi.h>

using namespace std;

namespace libqqc {

    void Do_qmp2 :: run(ostringstream &out){

        int prnt_lvl = mvault.get_mprnt_lvl();
        Ttimer timings(prnt_lvl);

        // Grabbing the calculation data we need
        size_t p1Dnpts = mvault.get_m1Dgrid().get_mnpts();
        size_t p3Dnpts = mvault.get_m3Dgrid().get_mnpts();
        size_t nocc = mvault.get_mnocc();
        size_t nvirt = mvault.get_mnvirt();
        size_t nmo = nocc + nvirt;
        size_t nao = mvault.get_mnao(); 

        double* mf = mvault.get_mmat_fock();
        double* mcgto = mvault.get_mmat_cgto();
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

        // Set up MPI environment and set important variables
        int pid, max_id; // pid is rank and max_id is maxrank
        MPI_Comm_rank(MPI_COMM_WORLD, &pid);
        MPI_Comm_size(MPI_COMM_WORLD, &max_id);
        MPI_Status status;

        for (size_t p = 0; p < nmo; p ++){
                vf[p] = mf[p * nmo + p];
        }

        // Part the cgto matrix into occupied and virtual spaces
        size_t pos = 0; // Position on virtual orbital space
#pragma omp parallel for schedule(dynamic) default(none)\
        private(pos) shared(p3Dnpts, nmo, m_o, mcgto, nocc, nvirt, m_v)
        for (size_t p = 0; p < p3Dnpts; p++){
            for (size_t q = 0; q < nmo; q++){
                if (q < nocc){
                    m_o[p * nocc + q] = mcgto[p * nmo + q];
                }
                else {
                    pos = q - nocc; // q covers nmo, so substr. num. of occ.
                    m_v[p * nvirt + pos] = mcgto[p * nmo + q];
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
        size_t npts_to_proc = p3Dnpts/(2 * max_id);
        size_t remaining_elements = p3Dnpts - npts_to_proc * max_id * 2;
        double energy = 0.0;

        timings.start_new_clock("Timing Qmp2_energy::compute : ", 0, 1);
        timings.start_new_clock("    -- Setting up calculation: ", 1, 2);

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
        // Now we distribute the middle points left over to the first batch, this will make
        // the workload slightly uneven
        size_t npts_to_proc_orig = npts_to_proc;
        int x = remaining_elements % max_id;
        int y = remaining_elements / max_id;
        timings.stop_clock(1);

        timings.start_new_clock("    -- Calc. Batch 1/2: ", 2, 2);
        offset = pid * npts_to_proc + ((pid < x) ? pid : x) + ((y == 1) ? pid : 0);
        npts_to_proc += ((pid < x) ? (1 + y) : y);
        energy += qmp2_energy.compute();

        npts_to_proc = npts_to_proc_orig;
        timings.stop_clock(2);

        // Second set of points 
        timings.start_new_clock("    -- Calc. Batch 2/2: ", 3, 2);
        offset = p3Dnpts - (1 + pid) * npts_to_proc;
        energy += qmp2_energy.compute();

        timings.stop_clock(3);

        timings.stop_clock(0);

        //now lets differentiate which node does what
        if (pid == 0){
            timings.start_new_clock("Gather result fr. nodes : ", 3, 1);
            double tmp = 0.0;
            // Get all partial energies from servants
            for (int i = 1; i < max_id; i++){
                MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, 
                        MPI_COMM_WORLD, &status);
                energy += tmp;
            }
            timings.stop_clock(3);

            out << "* Ground State Energy Correction (eV): " << energy;

            Printer_qmp2 printer(mvault, timings);
            printer.print_final(out);

        } else{
            // Send servant energies to master
            MPI_Send(&energy, 1, MPI_DOUBLE, 0,0, MPI_COMM_WORLD); 
        }


        delete[] m_v;
        delete[] m_o;
        delete[] c1Deps_ov;
        delete[] m1Deps_v;
        delete[] m1Deps_o;

    } //Do_qmp2::member_fn

} //namespace libqqc

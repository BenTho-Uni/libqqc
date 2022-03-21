#ifndef LIBQQC_PRINTER_QMP2_H
#define LIBQQC_PRINTER_QMP2_H

#if LIBQQC_WITH_OPENMP
#include <omp.h>
#endif

#if LIBQQC_WITH_MPI
#include <mpi.h>
#endif

// external headers
#include <string>

// internal headers
#include "../ttimer.h"
#include "../../vaults/vault_qmp2.h"

using namespace std;

namespace libqqc {

    ///
    /// @brief Printer Class For Q-MP(2) calculation
    ///
    /// @details This class prints the Q-MP(2) and library data
    ///

    class Printer_qmp2 {
        private:
            size_t mwidth = 80; //< width of console printout
            string mmethod_name = "Q-MP(2)"; ///< String for method name
            string mauthors = "Benjamin Thomitzni"; ///< String of name of authors

            string mlibqqc_vers = "v0.1"; ///< version of libqqc
            string mloader_vers = "v0.1"; ///< version of loader
            string mvault_vers = "v0.1"; ///< version of vault
            string mdoqmp2_vers = "v0.1"; ///< version of do_qmp2

            bool mb_openmp = false; ///< if OpenMP is enabled
            size_t mnthreads = 0; ///< number of OpenMP threads

            bool mb_mpi = false; ///< version of MPI
            size_t mnprocs = 0; ///< number of MPI processes

            // Method Specific data
            size_t mnao = 0; ///< number of atomic orbitals
            size_t mnmo = 0; ///< number of molecular orbitals
            size_t mnocc = 0; ///< number of occupied orbitals
            size_t mnvirt = 0; ///< number of virtual orbitals
            size_t m3Dnpts = 0; ///< number of 3D grid points
            size_t m1Dnpts = 0; ///< number of 1D grid points
            int mprnt_lvl = 0; ///< requested print level
            
            Ttimer &mtimings; ///< timing of method

        public: 
            ///
            /// @brief Constructor of the Printer_qmp2 class
            ///
            /// @details Prepares the data needed for printout
            ///
            /// @param[in,out] param [param]
            /// @return [return]
            ///
            Printer_qmp2 (Vault_qmp2 &vault, Ttimer &timings) 
                : mnao(vault.get_mnao()), mnmo(vault.get_mnmo()), 
                mnocc(vault.get_mnocc()), mnvirt(vault.get_mnvirt()),
                m3Dnpts(vault.get_m3Dgrid().get_mnpts()),
                m1Dnpts(vault.get_m1Dgrid().get_mnpts()),
                mprnt_lvl(vault.get_mprnt_lvl()),
                mtimings(timings) {
#if LIBQQC_WITH_OPENMP
                    mb_openmp = true;
                    mnthreads = omp_get_num_threads();
#endif

#if LIBQQC_WITH_MPI
                    mb_mpi = true;
                    int max_id; // pid is rank and max_id is maxrank
                    MPI_Comm_size(MPI_COMM_WORLD, &max_id);
                    mnprocs = max_id;
#endif
            };
            ///
            /// @brief [briefdescription]
            ///
            /// @details [longdescription]
            ///
            /// @param[in,out] param [param]
            /// @return [return]
            ///
            int member_fn(int param);
            ///
            /// @brief Modifies outputstream, adds OpenMP information
            ///
            /// @details This function takes in the output stringstream, adds
            /// the OpenMP information and then returns it
            ///
            /// @param[in,out] ostringstream outputstringstream
            ///
            void print_openmp(ostringstream &out){
                    out << make_line("* OpenMP enabled", 
                            'l') << endl
                        << make_line("    -- threads:   " + to_string(mnthreads),
                                'l')
                        << endl; 
            };
            ///
            /// @brief Modifies outputstream, MPI information
            ///
            /// @details This function takes in the output stringstream, adds
            /// the MPI information and then returns it
            ///
            /// @param[in,out] ostringstream outputstringstream
            ///
            void print_mpi(ostringstream &out){
                    out << make_line("* MPI enabled", 
                            'l') << endl
                        << make_line("    -- processes: " + to_string(mnprocs), 'l')
                        << endl;
            };
            ///
            /// @brief Returns output with method information
            ///
            /// @details This function takes in the output string, adds the 
            /// method information and then returns it
            ///
            /// @param[in,out] ostringstream Output string to modify
            ///
            void print_method_qmp2(ostringstream &out){
                // Calculation Details
                out << make_line("Calculation Details", 'm') << endl
                    << make_full_line('-') << endl
                    << make_line("* Method:             " + mmethod_name, 'l') 
                    << endl
                    << make_full_line('-') << endl
                    << make_line("* grid points (3D):   " + to_string(m3Dnpts)
                            , 'l') << endl
                    << make_line("* grid points (1D):   " + to_string(m1Dnpts)
                            , 'l') << endl
                    //<< make_line("* Print level:        " + to_string(mprnt_lvl)
                    //        , 'l') << endl
                    << make_full_line('-') << endl
                    << make_line("* atomic orbitals:    " + to_string(mnao), 
                            'l') << endl
                    << make_line("* molecular orbitals: " + to_string(mnmo), 
                            'l') << endl
                    << make_line("    -- occupied:      " + to_string(mnocc)
                            , 'l') << endl
                    << make_line("    -- virtual:       " + to_string(mnvirt)
                            , 'l') << endl
                    << make_full_line('-') << endl;
            };
            ///
            /// @brief Finishes a given string into final line
            ///
            /// @param[in] string input string
            /// @param[in] char alignment, r/l/m 
            /// @return string
            string make_line(string in, char align){
                size_t length_in = in.length();
                string out = "";

                // Calculating overal space 
                size_t space = ((length_in < (mwidth - 2)) ? 
                            ((mwidth - 2) - length_in) : 0); 
                string left = "";
                string right = "";

                if (align == 'l'){
                    left = ((space > 0 ) ? " " : "" );
                    space--;
                    for (size_t i = 0; i < space; i++){
                        right += " ";
                    }
                } else if (align == 'r'){
                    left = ((space > 0 ) ? " " : "" );
                    space--;
                    for (size_t i = 0; i < space; i++){
                        left += " ";
                    }
                    
                } else {
                    for (size_t i = space; i > 1; i -= 2){
                        left += " ";
                        right += " ";
                    }
                    left += ((space % 2) == 1) ? " " : "";
                }

                //finish out
                out = "|" + left + in + right + "|";
                return out;
            };
            ///
            /// @brief returns line filled with symbol
            ///
            /// @param[in] char character
            /// @return string
            ///
            string make_full_line(char c){
                string out = "";
                for (int i = mwidth - 2; i > 0; i--){
                    out += c;
                }

                out = "|" + out + "|";
                return out;
            };
            ///
            /// @brief makes header/footer
            ///
            /// @param[in] bool 1 for header, 0 for footer
            /// @return string
            ///
            string make_hdr_ftr(bool type){
                string out = "";

                for (int i = mwidth - 2; i > 0; i--){
                    out += "_";
                }

                out = ((type) ? " " : "|") + out + ((type) ? " " : "|");
                
                return out;

            };

            ///
            /// @brief returns final output
            ///
            /// @details Prints output to outputstringstream
            ///
            /// @param[in,out] ostringstream Output stringstream
            ///
            void print_final(ostringstream &out){
                string before = out.str();
                out.str("");

                // Header and library details (authors)
                out << make_hdr_ftr(1) << endl
                    << make_full_line(' ') << endl
                    << make_line("** Quadrature Calculation through libqqc **", 
                            'm') << endl
                    << make_full_line(' ') << endl
                    << make_full_line('+') << endl
                    << make_line("* Author(s): " + mauthors, 'l') << endl
                    << make_full_line('+') << endl;

                // Programm Details
                out << make_line("Programm Details", 'm') << endl
                    << make_full_line('-') 
                    << endl
                    << make_line("* library vers.      " + mlibqqc_vers, 'l') 
                    << endl
                    << make_line("    -- Loader vers.  " + mloader_vers, 'l') 
                    << endl
                    << make_line("    -- Vault vers.   " + mvault_vers, 'l') 
                    << endl
                    << make_line("    -- Do_qmp2 vers. " + mdoqmp2_vers, 'l') 
                    << endl;

                if (mb_openmp || mb_mpi) out << make_full_line('-') << endl;
                if (mb_openmp) print_openmp(out);
                if (mb_mpi) print_mpi(out);
                out << make_full_line('+') << endl;

                // Now we add the method specific information
                print_method_qmp2(out);

                // And we add the results
                out << make_line("Results", 'm') << endl
                    << make_full_line('-') << endl
                    << make_line(before, 'l') << endl;

                // Timings
                if (mprnt_lvl != 0) {
                    out << make_full_line('+') << endl
                        << make_line("Calculation Timings", 'm') << endl
                        << make_full_line('-') << endl;

                   // Get all Timing infromation, then split into strings
                   // by newline, which we feen into the make_line 
                    stringstream ss;
                    ss << mtimings.print_all_clocks();
                    
                    for (string line; getline(ss, line, '\n');)
                        out << make_line(line, 'l') << endl;

                    out << make_full_line('-') << endl
                        << make_line("if no timings are printed,"
                                " choose higher print level", 'm') 
                        << endl;
                }

                // Footer
                out << make_hdr_ftr(0) << endl;
            };
    }; // class Printer_qmp2

} // namespace libqqc

#endif //LIBQQC_PRINTER_QMP2_H

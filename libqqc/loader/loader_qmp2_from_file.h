#ifndef LIBQQC_LOADER_QMP2_FROM_FILE_H
#define LIBQQC_LOADER_QMP2_FROM_FILE_H

#include "../grids/grid.h"

//Additional libraries
#include <stdexcept>

using namespace std;

namespace libqqc {

    ///
    /// @brief Data input into vault
    ///
    /// @details This class handles the input of data into the holder vault class.
    ///

    class Loader_qmp2_from_file {
        private:
            string msrc_folder; ///< string to the directory in which the files are saved
            string mfname_inputs = "inputs.vec"; ///< string of filename of input file
            string mfname_1Dpts = "tpts.mat"; ///< string of filename of pts file for 1D grid
            string mfname_1Dwts = "twts.mat"; ///< string of filename of wrs file for 1D grid
            string mfname_3Dpts = "rpts.mat"; ///< string of filename of pts file for 3D grid
            string mfname_3Dwts = "rwts.mat"; ///< string of filename of wrs file for 3D grid
            string mfname_fock = "f.mat"; ///< string of filename of fock file
            string mfname_coeff = "c.mat"; ///< string of filename of coefficient file
            string mfname_cgto = "cgto.mat"; ///< string of filename of cgto file
            string mfname_coul = "coulomb.cube"; ///< string of filename of coulomb file

        public: 

            ///
            /// @brief Constructor of loader_qmp2_from_file class
            ///
            Loader_qmp2_from_file (string src_folder) : 
                msrc_folder(src_folder) {};

            ///
            /// @brief Constructor of loader_qmp2_from_file class
            ///
            Loader_qmp2_from_file (string src_folder, 
                    string fname_inputs, string fname_1Dpts, 
                    string fname_1Dwts, string fname_3Dpts, 
                    string fname_3Dwts, string fname_fock, 
                    string fname_coeff, string fname_cgto, 
                    string fname_coul) : msrc_folder(src_folder), 
            mfname_inputs(fname_inputs), mfname_1Dpts(fname_1Dpts),
            mfname_1Dwts(fname_1Dwts), mfname_3Dpts(fname_3Dpts),
            mfname_3Dwts(fname_3Dwts), mfname_fock(fname_fock), 
            mfname_coeff(fname_coeff), mfname_cgto(fname_cgto),
            mfname_coul(fname_coul) {};

            ///
            /// @brief loads number of occupied orbitals into reference
            ///
            /// @param[in,out] nocc reference to number of occupied orbitals
            ///
            void load_nocc(size_t &nocc);

            ///
            /// @brief loads number of virtual orbitals into reference
            ///
            /// @param[in,out] nvirt reference to number of virtual orbitals
            ///
            void load_nvirt(size_t &nvirt);

            ///
            /// @brief loads number of atomic orbitals into reference
            ///
            /// @param[in,out] nao reference to number of atomic orbitals
            ///
            void load_nao(size_t &nao);

            ///
            /// @brief loads printing level into reference
            ///
            /// @param[in,out] prnt_lvl reference to printing level
            ///
            void load_prnt_lvl(int &prnt_lvl);

            ///
            /// @brief loads points and weights and sets a grid
            ///
            /// @param[in] filename_pts name of point file as string
            /// @param[in] filename_wts name of weight file as string
            /// @param[in,out] grid grid reference
            ///
            void load_grid(string filename_pts, string filename_wts,
                Grid &grid);

            ///
            /// @brief loads points and weights and sets a 1D grid
            ///
            /// @param[in,out] grid grid reference
            ///
            void load_1Dgrid(Grid &grid){
                load_grid(mfname_1Dpts, mfname_1Dwts, grid);
            };

            ///
            /// @brief loads points and weights and sets a 3D grid
            ///
            /// @param[in,out] grid grid reference
            ///
            void load_3Dgrid(Grid &grid){
                load_grid(mfname_3Dpts, mfname_3Dwts, grid);
            };

            ///
            /// @brief loads values into the Fock matrix
            ///
            /// @param[in,out] mat_fock pointer to Fock matrix
            /// @param[in] dim1 first dimension of the matrix
            /// @param[in] dim2 second dimension of the matrix
            ///
            void load_mat_fock(double* mat_fock);

            ///
            /// @brief loads values into the coefficient matrix
            ///
            /// @param[in,out] mat_coeff pointer to coefficient matrix
            /// @param[in] dim1 first dimension of the matrix
            /// @param[in] dim2 second dimension of the matrix
            ///
            void load_mat_coeff(double* mat_coeff);

            ///
            /// @brief loads values into the CGTO matrix
            ///
            /// @param[in,out] mat_cgto pointer to cgto matrix
            /// @param[in] dim1 first dimension of the matrix
            /// @param[in] dim2 second dimension of the matrix
            ///
            void load_mat_cgto(double* mat_cgto);

            ///
            /// @brief loads values into the coulomb integral tensor
            ///
            /// @param[in,out] mat_fock pointer to coulomb integral tensor
            /// @param[in] dim1 first dimension of the tensor
            /// @param[in] dim2 second dimension of the tensor
            /// @param[in] dim3 third dimension of the tensor
            ///
            void load_cube_coul(double* cube_coul);

    }; // class Loader_qmp2_from_file

} // namespace libqqc

#endif //LIBQQC_LOADER_QMP2_FROM_FILE_H

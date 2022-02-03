#ifndef LIBQQC_LOADER_QMP2_FROM_FILE_H
#define LIBQQC_LOADER_qMP2_FROM_FILE_H

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

        public: 

            ///
            /// @brief Constructor of loader_qmp2_from_file class
            ///
            Loader_qmp2_from_file (string src_folder) : 
                msrc_folder(src_folder) {};

            ///
            /// @brief loads number of occupied orbitals into reference
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] nocc reference to number of occupied orbitals
            ///
            void load_nocc(string filename, size_t &nocc);

            ///
            /// @brief loads number of virtual orbitals into reference
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] nvirt reference to number of virtual orbitals
            ///
            void load_nvirt(string filename, size_t &nvirt);

            ///
            /// @brief loads number of atomic orbitals into reference
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] nao reference to number of atomic orbitals
            ///
            void load_nao(string filename, size_t &nao);

            ///
            /// @brief loads tolerance of 1D grid into reference
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] p1Dtol reference to tolerance
            ///
            void load_1Dtol(string filename, double &p1Dtol);

            ///
            /// @brief loads printing level into reference
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] prnt_lvl reference to printing level
            ///
            void load_prnt_lvl(string filename, int &prnt_lvl);

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
            /// @brief loads values into the Fock matrix
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] mat_fock pointer to Fock matrix
            /// @param[in] dim1 first dimension of the matrix
            /// @param[in] dim2 second dimension of the matrix
            ///
            void load_mat_fock(string filename, 
                    double* mat_fock, size_t dim1, size_t dim2);

            ///
            /// @brief loads values into the coefficient matrix
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] mat_coeff pointer to coefficient matrix
            /// @param[in] dim1 first dimension of the matrix
            /// @param[in] dim2 second dimension of the matrix
            ///
            void load_mat_coeff(string filename, 
                    double* mat_coeff, size_t dim1, size_t dim2);

            ///
            /// @brief loads values into the CGTO matrix
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] mat_cgto pointer to cgto matrix
            /// @param[in] dim1 first dimension of the matrix
            /// @param[in] dim2 second dimension of the matrix
            ///
            void load_mat_cgto(string filename, 
                    double* mat_cgto, size_t dim1, size_t dim2);

            ///
            /// @brief loads values into the coulomb integral tensor
            ///
            /// @param[in] filename name of file as string
            /// @param[in,out] mat_fock pointer to coulomb integral tensor
            /// @param[in] dim1 first dimension of the tensor
            /// @param[in] dim2 second dimension of the tensor
            /// @param[in] dim3 third dimension of the tensor
            ///
            void load_cube_coul(string filename, 
                    double* cube_coul, size_t dim1, size_t dim2, 
                    size_t dim3);

    }; // class Loader_qmp2_from_file

} // namespace libqqc

#endif //LIBQQC_LOADER_QMP2_FROM_FILE_H

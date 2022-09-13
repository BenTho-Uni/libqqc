#ifndef LIBQQC_LOADER_QMP2_FROM_QCHEM_H
#define LIBQQC_LOADER_QMP2_FROM_QCHEM_H

#include "../grids/grid.h"
#include "../grids/isabel-grids.h"


//Additional libraries
#include <stdexcept>

using namespace std;

namespace libqqc {

    ///
    /// @brief Data input into vault
    ///
    /// @details This class handles the input of data into the holder vault class from Q-Chem.
    ///

    class Loader_qmp2_from_qchem {
        private:
        
	public: 
            int counter = 0; //for ell grid

            ///
            /// @brief Constructor of loader_qmp2 class
            ///
            Loader_qmp2_from_qchem () {};

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
            /// @param[in,out] grid grid reference
            ///
            void load_1Dgrid(Grid &grid);

            ///
            /// @brief loads points and weights and sets a grid
            ///
            /// @param[in,out] grid grid reference
            ///
            void load_3Dgrid(Grid &grid, Grid3D<vector<double>, size_t>  &grid3D);

            ///
            /// @brief loads values into the Fock matrix
            ///
            /// @param[in,out] mat_fock pointer to Fock matrix
            ///
            void load_mat_fock(double* mat_fock);

            ///
            /// @brief loads values into the CGTO matrix
            ///
            /// @param[in,out] mat_cgto pointer to cgto matrix
            ///
            void load_mat_cgto(double* mat_cgto);

            ///
            /// @brief loads values into the coulomb integral tensor
            ///
            /// @param[in,out] mat_fock pointer to coulomb integral tensor
            ///
            void load_cube_coul(double* cube_coul);

    }; // class Loader_qmp2_from_qchem

} // namespace libqqc

#endif //LIBQQC_LOADER_QMP2_FROM_QCHEM_H

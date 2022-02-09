#ifndef LIBQQC_DO_QMP2_H
#define LIBQQC_DO_QMP2_H

#include "../vaults/vault_qmp2.h"
#include "qmp/qmp2_energy.h"

// Standard Headers
#include <sstream>

using namespace std;

namespace libqqc {

    ///
    /// @brief Q-MP2 setup and calling the calculation
    ///
    /// @details This class handles the setup needed for the Q-MP2 calculations
    /// and calls the calculations. It also prints the output. 
    ///

    class Do_qmp2 {
        private:
           Vault_qmp2 &mvault;

        public: 
            ///
            /// @brief Constructor for the handling of Q-MP2 calculations
            ///
            /// @details This constructor sets up the references needed,
            /// especially the data holding vault object.
            ///
            /// @param[in,out] pvault vault object which holds the data
            ///
            Do_qmp2(Vault_qmp2 &pvault) : mvault(pvault) {};
            ///
            /// @brief this runs the setup and executes the calculation
            ///
            /// @details This method runs the setup for the Q-MP2 calculation
            /// and executes it. It then prints the results into the output.
            //
            /// @param[in,out] pout output stringstream for printout 
            ///
            void run(ostringstream &out);
    }; // class Do_qmp2

} // namespace libqqc

#endif //LIBQQC_DO_QMP2_H

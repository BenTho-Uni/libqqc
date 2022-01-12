//include file header
#include "test_loader_mp2.h"


// includes of the to be tested headers
#include "../../libqqc/loader/loader_mp2.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Loader_mp2 :: test_load_nocc() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_nvirt() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_nao() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_1Dtol() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_prnt_lvl() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_1Dgrid() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_3Dgrid() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_mat_fock() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_mat_coeff() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_mat_cgto() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: test_load_cube_coul() {
        bool result = false;
        return result;
    }

    bool Test_Loader_mp2 :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_load_nocc = (test_load_nocc()) ? true : false;
        out << "    Testing loader_mp2::load_nocc()         ... " << flush
            << (b_load_nocc ? "passed" : "failed") << endl;

        bool b_load_nvirt = (test_load_nvirt()) ? true : false;
        out << "    Testing loader_mp2::load_nvirt()        ... " << flush
            << (b_load_nvirt ? "passed" : "failed") << endl;

        bool b_load_nao = (test_load_nao()) ? true : false;
        out << "    Testing loader_mp2::load_nao()          ... " << flush
            << (b_load_nao ? "passed" : "failed") << endl;

        bool b_load_1Dtol = (test_load_1Dtol()) ? true : false;
        out << "    Testing loader_mp2::load_1Dtol()        ... " << flush
            << (b_load_1Dtol ? "passed" : "failed") << endl;

        bool b_load_prnt_lvl = (test_load_prnt_lvl()) ? true : false;
        out << "    Testing loader_mp2::load_prnt_lvl()     ... " << flush
            << (b_load_prnt_lvl ? "passed" : "failed") << endl;

        bool b_load_1Dgrid = (test_load_1Dgrid()) ? true : false;
        out << "    Testing loader_mp2::load_1Dgrid()       ... " << flush
            << (b_load_1Dgrid ? "passed" : "failed") << endl;

        bool b_load_3Dgrid = (test_load_3Dgrid()) ? true : false;
        out << "    Testing loader_mp2::load_3Dgrid()       ... " << flush
            << (b_load_3Dgrid ? "passed" : "failed") << endl;

        bool b_load_mat_fock = (test_load_mat_fock()) ? true : false;
        out << "    Testing loader_mp2::load_mat_fock()     ... " << flush
            << (b_load_mat_fock ? "passed" : "failed") << endl;

        bool b_load_mat_coeff = (test_load_mat_coeff()) ? true : false;
        out << "    Testing loader_mp2::load_mat_coeff()    ... " << flush
            << (b_load_mat_coeff ? "passed" : "failed") << endl;

        bool b_load_mat_cgto = (test_load_mat_cgto()) ? true : false;
        out << "    Testing loader_mp2::load_mat_cgto()     ... " << flush
            << (b_load_mat_cgto ? "passed" : "failed") << endl;

        bool b_load_cube_coul = (test_load_cube_coul()) ? true : false;
        out << "    Testing loader_mp2::load_cube_coul()    ... " << flush
            << (b_load_cube_coul ? "passed" : "failed") << endl;

        result = b_load_nocc && b_load_nvirt && b_load_nao && b_load_1Dtol
            && b_load_prnt_lvl && b_load_1Dgrid && b_load_3Dgrid
            && b_load_mat_fock && b_load_mat_coeff & b_load_mat_cgto
            && b_load_cube_coul;
        return result;

    } // run_test

} // namespace

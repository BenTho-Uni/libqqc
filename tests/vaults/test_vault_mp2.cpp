//include file header
#include "test_vault_mp2.h"


// includes of the to be tested headers
#include "../../libqqc/vaults/vault_mp2.h"

//additional headers
#include "../../libqqc/loader/loader_mp2.h"


// namespaces

using namespace std;

namespace libqqc {
    bool Test_Vault_mp2 :: test_check_data_validity() {
        bool result = true;

        Vault_mp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        try {
            vault.check_data_validity();
            result = true;
        } catch (...) {
        }

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mnocc() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mnvirt() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mnnmo() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mnao() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_m1Dtol() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mprnt_lvl() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_m1Dgrid() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_m3Dgrid() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mmat_fock() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mmat_coeff() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mmat_cgto() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: test_get_mcube_coul() {
        bool result = false;

        return result;
    }

    bool Test_Vault_mp2 :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_check_data_validity = (test_check_data_validity()) ? true : false;
        out << "    Testing Vault_mp2::check_data_validity()   ... " << flush
            << (b_check_data_validity ? "passed" : "failed") << endl;

        bool b_get_mnocc = (test_get_mnocc()) ? true : false;
        out << "    Testing Vault_mp2::get_mnocc()             ... " << flush
            << (b_get_mnocc ? "passed" : "failed") << endl;

        bool b_get_mnvirt = (test_get_mnvirt()) ? true : false;
        out << "    Testing Vault_mp2::get_mnvirt()            ... " << flush
            << (b_get_mnvirt ? "passed" : "failed") << endl;

        bool b_get_mnnmo = (test_get_mnnmo()) ? true : false;
        out << "    Testing Vault_mp2::get_mnnmo()             ... " << flush
            << (b_get_mnnmo ? "passed" : "failed") << endl;

        bool b_get_mnao = (test_get_mnao()) ? true : false;
        out << "    Testing Vault_mp2::get_mnao()              ... " << flush
            << (b_get_mnao ? "passed" : "failed") << endl;

        bool b_get_m1Dtol = (test_get_m1Dtol()) ? true : false;
        out << "    Testing Vault_mp2::get_m1Dtol()            ... " << flush
            << (b_get_m1Dtol ? "passed" : "failed") << endl;

        bool b_get_mprnt_lvl = (test_get_mprnt_lvl()) ? true : false;
        out << "    Testing Vault_mp2::test_get_mprnt_lvl()    ... " << flush
            << (b_get_mprnt_lvl ? "passed" : "failed") << endl;

        bool b_get_m1Dgrid = (test_get_m1Dgrid()) ? true : false;
        out << "    Testing Vault_mp2::get_m1Dgrid()           ... " << flush
            << (b_get_m1Dgrid ? "passed" : "failed") << endl;

        bool b_get_m3Dgrid = (test_get_m3Dgrid()) ? true : false;
        out << "    Testing Vault_mp2::get_m3Dgrid()           ... " << flush
            << (b_get_m3Dgrid ? "passed" : "failed") << endl;

        bool b_get_mmat_fock = (test_get_mmat_fock()) ? true : false;
        out << "    Testing Vault_mp2::get_mmat_fock()         ... " << flush
            << (b_get_mmat_fock ? "passed" : "failed") << endl;

        bool b_get_mmat_coeff = (test_get_mmat_coeff()) ? true : false;
        out << "    Testing Vault_mp2::get_mmat_coeff()        ... " << flush
            << (b_get_mmat_coeff ? "passed" : "failed") << endl;

        bool b_get_mmat_cgto = (test_get_mmat_cgto()) ? true : false;
        out << "    Testing Vault_mp2::get_mmat_cgto()         ... " << flush
            << (b_get_mmat_cgto ? "passed" : "failed") << endl;

        bool b_get_mcube_coul = (test_get_mcube_coul()) ? true : false;
        out << "    Testing Vault_mp2::get_mcube_coul()        ... " << flush
            << (b_get_mcube_coul ? "passed" : "failed") << endl;

        result = b_check_data_validity && b_get_mnocc && b_get_mnvirt
            && b_get_mnnmo && b_get_mnao && b_get_m1Dtol && b_get_mprnt_lvl
            && b_get_m1Dgrid && b_get_m3Dgrid && b_get_mmat_fock
            && b_get_mmat_coeff && b_get_mmat_cgto && b_get_mcube_coul;
        return result;

    } // run_test

} // namespace

//include file header
#include "test_vault_qmp2.h"


// includes of the to be tested headers
#include "../../libqqc/vaults/vault_qmp2.h"

//additional headers
#include "../../libqqc/loader/loader_qmp2.h"


// namespaces

using namespace std;

namespace libqqc {
    bool Test_Vault_qmp2 :: test_Vault_qmp2() {
        bool result = true;

        Loader_qmp2 loader;
        Vault_qmp2<Loader_qmp2> vault(loader);
        if (vault.get_mnocc() == 4) result = true;
        if (vault.get_mnvirt() == 5) result = result && true;
        if (vault.get_mnao() == 9) result = result && true;
        if (vault.get_m1Dtol() == 10e-4) result = result && true;
        if (vault.get_mprnt_lvl() == 0) result = result && true;
        if (vault.get_m1Dgrid().get_mnpts() == 3) result = result && true;
        if (vault.get_m1Dgrid().get_mndim() == 1) result = result && true;
        if (vault.get_m1Dgrid().get_mpts()[0] == 1.0) result = result && true;
        if (vault.get_m1Dgrid().get_mwts()[0] == 1.0) result = result && true;
        if (vault.get_m3Dgrid().get_mnpts() == 3) result = result && true;
        if (vault.get_m3Dgrid().get_mndim() == 1) result = result && true;
        if (vault.get_m3Dgrid().get_mpts()[0] == 1.0) result = result && true;
        if (vault.get_m3Dgrid().get_mwts()[0] == 1.0) result = result && true;
        if (vault.get_mmat_fock()[0] == 1.0) result = true;
        if (vault.get_mmat_cgto()[0] == 1.0) result = result && true;
        if (vault.get_mcube_coul()[0] == 1.0) result = result && true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_check_data_validity() {
        bool result = true;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        try {
            vault.check_data_validity();
            result = true;
        } catch (...) {
        }

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mnocc() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        size_t tnocc = vault.get_mnocc();
        if (tnocc == mnocc) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mnvirt() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        size_t tnvirt = vault.get_mnvirt();
        if (tnvirt == mnvirt) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mnnmo() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        size_t tnnmo = vault.get_mnnmo();
        if (tnnmo == mnnmo) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mnao() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        size_t tnao = vault.get_mnao();
        if (tnao == mnao) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_m1Dtol() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        double t1Dtol = vault.get_m1Dtol();
        if (t1Dtol == m1Dtol) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mprnt_lvl() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        int tprnt_lvl = vault.get_mprnt_lvl();
        if (tprnt_lvl == mprnt_lvl) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_m1Dgrid() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        if (m1Dgrid.get_mnpts() == vault.get_m1Dgrid().get_mnpts()) result = true;
        if (m1Dgrid.get_mndim() == vault.get_m1Dgrid().get_mndim()) 
            result = result && true;
        if (m1Dgrid.get_mpts()[0] == vault.get_m1Dgrid().get_mpts()[0]) 
            result = result && true;
        if (m1Dgrid.get_mwts()[0] == vault.get_m1Dgrid().get_mwts()[0]) 
            result = result && true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_m3Dgrid() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        if (m3Dgrid.get_mnpts() == vault.get_m3Dgrid().get_mnpts()) result = true;
        if (m3Dgrid.get_mndim() == vault.get_m3Dgrid().get_mndim()) 
            result = result && true;
        if (m3Dgrid.get_mpts()[0] == vault.get_m3Dgrid().get_mpts()[0]) 
            result = result && true;
        if (m3Dgrid.get_mwts()[0] == vault.get_m3Dgrid().get_mwts()[0]) 
            result = result && true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mmat_fock() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        if (mmat_fock[0] == vault.get_mmat_fock()[0]) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mmat_coeff() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        if (mmat_coeff[0] == vault.get_mmat_coeff()[0]) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mmat_cgto() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        if (mmat_cgto[0] == vault.get_mmat_cgto()[0]) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: test_get_mcube_coul() {
        bool result = false;

        Vault_qmp2 vault(mnocc, mnvirt, mnao, m1Dtol, mprnt_lvl, m1Dgrid,
                m3Dgrid, mmat_fock, mmat_coeff, mmat_cgto,
                mcube_coul);
        if (mcube_coul[0] == vault.get_mcube_coul()[0]) result = true;

        return result;
    }

    bool Test_Vault_qmp2 :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_Vault_qmp2 = test_Vault_qmp2();
        out << "    Testing Vault_qmp2::Vault_qmp2()            ... " << flush
            << (b_Vault_qmp2 ? "passed" : "failed") << endl;

        bool b_check_data_validity = test_check_data_validity();
        out << "    Testing Vault_qmp2::check_data_validity()   ... " << flush
            << (b_check_data_validity ? "passed" : "failed") << endl;

        bool b_get_mnocc = test_get_mnocc();
        out << "    Testing Vault_qmp2::get_mnocc()             ... " << flush
            << (b_get_mnocc ? "passed" : "failed") << endl;

        bool b_get_mnvirt = test_get_mnvirt();
        out << "    Testing Vault_qmp2::get_mnvirt()            ... " << flush
            << (b_get_mnvirt ? "passed" : "failed") << endl;

        bool b_get_mnnmo = test_get_mnnmo();
        out << "    Testing Vault_qmp2::get_mnnmo()             ... " << flush
            << (b_get_mnnmo ? "passed" : "failed") << endl;

        bool b_get_mnao = test_get_mnao();
        out << "    Testing Vault_qmp2::get_mnao()              ... " << flush
            << (b_get_mnao ? "passed" : "failed") << endl;

        bool b_get_m1Dtol = test_get_m1Dtol();
        out << "    Testing Vault_qmp2::get_m1Dtol()            ... " << flush
            << (b_get_m1Dtol ? "passed" : "failed") << endl;

        bool b_get_mprnt_lvl = test_get_mprnt_lvl();
        out << "    Testing Vault_qmp2::test_get_mprnt_lvl()    ... " << flush
            << (b_get_mprnt_lvl ? "passed" : "failed") << endl;

        bool b_get_m1Dgrid = test_get_m1Dgrid();
        out << "    Testing Vault_qmp2::get_m1Dgrid()           ... " << flush
            << (b_get_m1Dgrid ? "passed" : "failed") << endl;

        bool b_get_m3Dgrid = test_get_m3Dgrid();
        out << "    Testing Vault_qmp2::get_m3Dgrid()           ... " << flush
            << (b_get_m3Dgrid ? "passed" : "failed") << endl;

        bool b_get_mmat_fock = test_get_mmat_fock();
        out << "    Testing Vault_qmp2::get_mmat_fock()         ... " << flush
            << (b_get_mmat_fock ? "passed" : "failed") << endl;

        bool b_get_mmat_coeff = test_get_mmat_coeff();
        out << "    Testing Vault_qmp2::get_mmat_coeff()        ... " << flush
            << (b_get_mmat_coeff ? "passed" : "failed") << endl;

        bool b_get_mmat_cgto = test_get_mmat_cgto();
        out << "    Testing Vault_qmp2::get_mmat_cgto()         ... " << flush
            << (b_get_mmat_cgto ? "passed" : "failed") << endl;

        bool b_get_mcube_coul = test_get_mcube_coul();
        out << "    Testing Vault_qmp2::get_mcube_coul()        ... " << flush
            << (b_get_mcube_coul ? "passed" : "failed") << endl;

        result = b_Vault_qmp2 && b_check_data_validity && b_get_mnocc
            && b_get_mnvirt
            && b_get_mnnmo && b_get_mnao && b_get_m1Dtol && b_get_mprnt_lvl
            && b_get_m1Dgrid && b_get_m3Dgrid && b_get_mmat_fock
            && b_get_mmat_coeff && b_get_mmat_cgto && b_get_mcube_coul;
        return result;

    } // run_test

} // namespace

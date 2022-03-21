//include file header
#include "test_loader_qmp2.h"


// includes of the to be tested headers
#include "../../libqqc/loader/loader_qmp2.h"

#include <iomanip>

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Loader_qmp2 :: test_load_nocc() {
        bool result = false;

        size_t nocc = 0;
        size_t nocc_ref = 1;

        Loader_qmp2 loader;
        loader.load_nocc(nocc);

        if (nocc == nocc_ref) result = true;

        return result;
    }

    bool Test_Loader_qmp2 :: test_load_nvirt() {
        bool result = false;

        size_t nvirt = 0;
        size_t nvirt_ref = 2;

        Loader_qmp2 loader;
        loader.load_nvirt(nvirt);

        if (nvirt == nvirt_ref) result = true;

        return result;
    }

    bool Test_Loader_qmp2 :: test_load_nao() {
        bool result = false;

        size_t nao = 0;
        size_t nao_ref = 3;

        Loader_qmp2 loader;
        loader.load_nao(nao);

        if (nao == nao_ref) result = true;
        return result;
    }

    bool Test_Loader_qmp2 :: test_load_prnt_lvl() {
        bool result = false;

        int prnt_lvl = -1;
        int prnt_lvl_ref = 0;

        Loader_qmp2 loader;
        loader.load_prnt_lvl(prnt_lvl);

        if (prnt_lvl == prnt_lvl_ref) result = true;
        return result;
    }

    bool Test_Loader_qmp2 :: test_load_1Dgrid() {
        bool result = false;
        
        Grid grid;

        Loader_qmp2 loader;
        loader.load_1Dgrid(grid);

        size_t npts_ref = 2;
        size_t ndim_ref = 1;

        if ((npts_ref == grid.get_mnpts()) && (ndim_ref == grid.get_mndim()))
            result = true;

        double res_pts = 0;
        double res_wts = 0;

        for (size_t p = 0; p < grid.get_mnpts(); p++){
            for (size_t d = 0; d < grid.get_mndim(); d++){
                res_pts += grid.get_mpts()[p * grid.get_mndim() + d];
            }
                res_wts += grid.get_mwts()[p];
        }

        double res_pts_ref = 2.0;
        double res_wts_ref = 2.0;

        double tol = 1e-8;

        if ((abs(res_pts_ref - res_pts) < tol) &&
                (abs(res_wts_ref - res_wts) < tol)) result = result && true;

        return result;
    }

    bool Test_Loader_qmp2 :: test_load_3Dgrid() {
        bool result = false;
        
        Grid grid;

        Loader_qmp2 loader;
        loader.load_3Dgrid(grid);

        size_t npts_ref = 2;
        size_t ndim_ref = 3;

        if ((npts_ref == grid.get_mnpts()) && (ndim_ref == grid.get_mndim()))
            result = true;

        double res_pts = 0;
        double res_wts = 0;

        for (size_t p = 0; p < grid.get_mnpts(); p++){
            for (size_t d = 0; d < grid.get_mndim(); d++){
                res_pts += grid.get_mpts()[p * grid.get_mndim() + d];
            }
                res_wts += grid.get_mwts()[p];
        }

        double res_pts_ref = 6.0;
        double res_wts_ref = 6.0;

        double tol = 1e-8;

        if ((abs(res_pts_ref - res_pts) < tol) &&
                (abs(res_wts_ref - res_wts) < tol)) result = result && true;

        return result;
    }

    bool Test_Loader_qmp2 :: test_load_mat_fock() {
        bool result = false;

        size_t nmo = 3;

        double mat[nmo * nmo];
        
        Loader_qmp2 loader;
        loader.load_mat_fock(mat);

        double res = 0;

        for (size_t k = 0; k < nmo; k++){
            for (size_t l = 0; l < nmo; l++){
                res += mat[k * nmo + l];
            }
        }

        double res_ref = 9.0;
        double tol = 10e-8;

        if (abs(res_ref - res) < tol) result = true;
        return result;
    }

    bool Test_Loader_qmp2 :: test_load_mat_cgto() {
        bool result = false;

        size_t nmo = 3;
        size_t npts = 2;

        double mat[npts * nmo];
        
        Loader_qmp2 loader;
        loader.load_mat_cgto(mat);

        double res = 0;

        for (size_t p = 0; p < npts; p++){
            for (size_t l = 0; l < nmo; l++){
                res += mat[p * nmo + l];
            }
        }

        double res_ref = 6.0;
        double tol = 10e-8;

        if (abs(res_ref - res) < tol) result = true;
        return result;
    }

    bool Test_Loader_qmp2 :: test_load_cube_coul() {
        bool result = false;

        size_t nocc = 1;
        size_t nvirt = 2;
        size_t npts = 2;

        double cube[npts * nocc * nvirt];
        
        Loader_qmp2 loader;
        loader.load_cube_coul(cube);

        double res = 0;

        for (size_t p = 0; p < npts; p++){
            for (size_t k = 0; k < nocc; k++){
                for (size_t l = 0; l < nvirt; l++){
                    res += cube[p * nocc * nvirt + k * nvirt + l];
                }
            }
        }

        double res_ref = 4.0;
        double tol = 10e-8;

        if (abs(res_ref - res) < tol) result = true;
        return result;
    }

    bool Test_Loader_qmp2 :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_load_nocc = test_load_nocc();
        out << "    Testing loader_qmp2::load_nocc()         ... " << flush
            << (b_load_nocc ? "passed" : "failed") << endl;

        bool b_load_nvirt = test_load_nvirt();
        out << "    Testing loader_qmp2::load_nvirt()        ... " << flush
            << (b_load_nvirt ? "passed" : "failed") << endl;

        bool b_load_nao = test_load_nao();
        out << "    Testing loader_qmp2::load_nao()          ... " << flush
            << (b_load_nao ? "passed" : "failed") << endl;

        bool b_load_prnt_lvl = test_load_prnt_lvl();
        out << "    Testing loader_qmp2::load_prnt_lvl()     ... " << flush
            << (b_load_prnt_lvl ? "passed" : "failed") << endl;

        bool b_load_1Dgrid = test_load_1Dgrid();
        out << "    Testing loader_qmp2::load_1Dgrid()       ... " << flush
            << (b_load_1Dgrid ? "passed" : "failed") << endl;

        bool b_load_3Dgrid = test_load_3Dgrid();
        out << "    Testing loader_qmp2::load_3Dgrid()       ... " << flush
            << (b_load_3Dgrid ? "passed" : "failed") << endl;

        bool b_load_mat_fock = test_load_mat_fock();
        out << "    Testing loader_qmp2::load_mat_fock()     ... " << flush
            << (b_load_mat_fock ? "passed" : "failed") << endl;

        bool b_load_mat_cgto = test_load_mat_cgto();
        out << "    Testing loader_qmp2::load_mat_cgto()     ... " << flush
            << (b_load_mat_cgto ? "passed" : "failed") << endl;

        bool b_load_cube_coul = test_load_cube_coul();
        out << "    Testing loader_qmp2::load_cube_coul()    ... " << flush
            << (b_load_cube_coul ? "passed" : "failed") << endl;

        result = b_load_nocc && b_load_nvirt && b_load_nao
            && b_load_prnt_lvl && b_load_1Dgrid && b_load_3Dgrid
            && b_load_mat_fock && b_load_mat_cgto
            && b_load_cube_coul;
        return result;

    } // run_test

} // namespace

//include file header
#include "test_loader_qmp2_from_file.h"


// includes of the to be tested headers
#include "../../libqqc/loader/loader_qmp2_from_file.h"
#include "../../libqqc/utils/load_from_file.h"
#include "../../libqqc/grids/grid.h"

// namespaces

using namespace std;

namespace libqqc {
    bool Test_Loader_qmp2_from_file :: test_load_nocc() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        size_t occ = 0;
        loader.load_nocc("inputs.vec", occ);

        if (occ == 5) result = true;

        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_nvirt() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        size_t virt = 0;
        loader.load_nvirt("inputs.vec", virt);

        if (virt == 8) result = true;

        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_nao() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        size_t ao = 0;
        loader.load_nao("inputs.vec", ao);

        if (ao == 13) result = true;

        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_1Dtol() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        double p1Dtol = 0;
        loader.load_1Dtol("inputs.vec", p1Dtol);

        if (p1Dtol == 1e-10) result = true;

        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_prnt_lvl() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        int prnt_lvl = 0;
        loader.load_prnt_lvl("inputs.vec", prnt_lvl);

        if (prnt_lvl == 4) result = true;

        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_grid() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");

        // Read in Dimension
        Grid grid;
        loader.load_grid("rpts.mat", "rwts.mat", grid);
        
        result = grid.check_data_validity();

        // Calculate residuals
        size_t dim = grid.get_mndim();
        size_t npts = grid.get_mnpts();
        double* pts = grid.get_mpts();
        double* wts = grid.get_mwts();

        double res_pts = 0;
        double res_wts = 0;

        for (size_t p = 0; p < npts; p++){
            for( size_t i = 0; i < dim; i++){
                res_pts += pts[p * dim + dim];
            }
            res_wts += wts[p];
        }

        double res_pts_ref = 1272.9176009;
        double res_wts_ref = 1588100923.18800473;
        double tol = 10e-8;

        if ((abs(res_pts_ref - res_pts) < tol) && 
                (abs(res_wts_ref - res_wts) < tol)) result = true;


        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_mat_fock() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        size_t nao =  0;
        loader.load_nao("inputs.vec", nao);

        double mat[nao * nao];

        loader.load_mat_fock("f.mat", mat, nao, nao);

        double res = 0;

        for (size_t k = 0; k < nao; k++){
            for (size_t l = 0; l < nao; l++){
                res += mat[k * nao + l];
            }
        }

        double res_ref = -86.5529705;
        double tol = 10e-8;

        if (abs(res_ref - res) < tol) result = true;

        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_mat_coeff() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        size_t nao =  0;
        loader.load_nao("inputs.vec", nao);

        double mat[nao * nao];

        loader.load_mat_coeff("c.mat", mat, nao, nao);

        double res = 0;

        for (size_t k = 0; k < nao; k++){
            for (size_t l = 0; l < nao; l++){
                res += mat[k * nao + l];
            }
        }

        double res_ref = 6.7375941851;
        double tol = 10e-8;

        if (abs(res_ref - res) < tol) result = true;
        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_mat_cgto() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        size_t nao =  0;
        loader.load_nao("inputs.vec", nao);

        Grid grid;
        loader.load_grid("rpts.mat", "rwts.mat", grid);

        size_t npts = grid.get_mnpts();

        double mat[npts * nao];

        loader.load_mat_cgto("cgto.mat", mat, npts, nao);

        double res = 0;

        for (size_t p = 0; p < npts; p++){
            for (size_t l = 0; l < nao; l++){
                res += mat[p * nao + l];
            }
        }

        double res_ref = 3439.4062267;
        double tol = 10e-8;

        if (abs(res_ref - res) < tol) result = true;

        return result;
    }

    bool Test_Loader_qmp2_from_file :: test_load_cube_coul() {
        bool result = false;

        Loader_qmp2_from_file loader("../../data/h2o/");
        size_t nao =  0;
        loader.load_nao("inputs.vec", nao);

        Grid grid;
        loader.load_grid("rpts.mat", "rwts.mat", grid);

        size_t npts = grid.get_mnpts();

        double mat[npts * nao * nao];

        loader.load_cube_coul("coulomb.cube", mat, nao, nao, npts);

        double res = 0;

        for (size_t p = 0; p < npts; p++){
            for (size_t k = 0; k < nao; k++){
                for (size_t l = 0; l < nao; l++){
                    res += mat[p * nao * nao + k * nao + l];
                }
            }
        }

        double res_ref = 35954.1176115;
        double tol = 10e-8;

        if (abs(res_ref - res) < tol) result = true;
        return result;
    }

    bool Test_Loader_qmp2_from_file :: run_all_tests(ostringstream &out) {
        out.str(""); //clearing the output string
        bool result = false;

        bool b_load_nocc = test_load_nocc();
        out << "    Testing loader_qmp2_from_file::load_nocc()         ... " << flush
            << (b_load_nocc ? "passed" : "failed") << endl;

        bool b_load_nvirt = test_load_nvirt();
        out << "    Testing loader_qmp2_from_file::load_nvirt()        ... " << flush
            << (b_load_nvirt ? "passed" : "failed") << endl;

        bool b_load_nao = test_load_nao();
        out << "    Testing loader_qmp2_from_file::load_nao()          ... " << flush
            << (b_load_nao ? "passed" : "failed") << endl;

        bool b_load_1Dtol = test_load_1Dtol();
        out << "    Testing loader_qmp2_from_file::load_1Dtol()        ... " << flush
            << (b_load_1Dtol ? "passed" : "failed") << endl;

        bool b_load_prnt_lvl = test_load_prnt_lvl();
        out << "    Testing loader_qmp2_from_file::load_prnt_lvl()     ... " << flush
            << (b_load_prnt_lvl ? "passed" : "failed") << endl;

        bool b_load_grid = test_load_grid();
        out << "    Testing loader_qmp2_from_file::load_grid()         ... " << flush
            << (b_load_grid ? "passed" : "failed") << endl;

        bool b_load_mat_fock = test_load_mat_fock();
        out << "    Testing loader_qmp2_from_file::load_mat_fock()     ... " << flush
            << (b_load_mat_fock ? "passed" : "failed") << endl;

        bool b_load_mat_coeff = test_load_mat_coeff();
        out << "    Testing loader_qmp2_from_file::load_mat_coeff()    ... " << flush
            << (b_load_mat_coeff ? "passed" : "failed") << endl;

        bool b_load_mat_cgto = test_load_mat_cgto();
        out << "    Testing loader_qmp2_from_file::load_mat_cgto()     ... " << flush
            << (b_load_mat_cgto ? "passed" : "failed") << endl;

        bool b_load_cube_coul = test_load_cube_coul();
        out << "    Testing loader_qmp2_from_file::load_cube_coul()    ... " << flush
            << (b_load_cube_coul ? "passed" : "failed") << endl;

        result = b_load_nocc && b_load_nvirt && b_load_nao && b_load_1Dtol &&
            b_load_prnt_lvl && b_load_grid && b_load_mat_fock && b_load_mat_coeff
            && b_load_mat_cgto && b_load_cube_coul;
        return result;

    } // run_test

} // namespace

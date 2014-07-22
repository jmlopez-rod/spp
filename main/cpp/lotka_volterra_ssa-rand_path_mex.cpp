// File generated on Wed Feb 12, 2014 10:58:53 PM by xcpp.
#define XC_MATLAB
#include "mex.h"
#include <debugoff.h>
#include <excentury/excentury.h>
#include <spp/spp.h>
#include <spp/models/lotka_volterra_ssa.h>
#include <spp/methods/gillespie.h>
// RAND_PATH
#include <spp/collectors/rand_path.h>
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    size_t ncin_ = mxGetScalar(prhs[0]);
    char* pcin_ = mxArrayToString(prhs[1]);
    excentury::STextInterface<excentury::load_mode> XC_LI_(pcin_, ncin_);
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    XC_LI_.close();

    LotkaVolterraSSA model;
    Gillespie method(model);
    RandPath dc(method, npoints, ntrials);

    for (int trial = 0; trial < ntrials; ++trial) {
        method.new_trial();
        dc.new_trial();
        while (method.time < INFINITY) {
            model.eval_prop(method.x);
            method.get_time_step();
            if (method.update_system()) break;
            dc.process_info();
            if (dc.is_complete()) break;
        }
        method.end_trial();
        dc.end_trial();
    }
    dc.end_sim();

    excentury::STextInterface<excentury::dump_mode> XC_DI_;
    if (ntrials == 1) {
        XC_DI_.dump(dc.t[0], "t", dc.t[0][0]);
        XC_DI_.dump(dc.x[0], "x", dc.x[0][0]);
    } else {
        XC_DI_.dump(dc.t, "t", dc.t[0]);
        XC_DI_.dump(dc.x, "x", dc.x[0]);
    }
    XC_DI_.close();
    std::string xc_mex_str_ = XC_DI_.str();
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    double* ncout_ = mxGetPr(plhs[0]);
    ncout_[0] = xc_mex_str_.size();
    plhs[1] = mxCreateString(xc_mex_str_.data());
    mxFree(pcin_);
}

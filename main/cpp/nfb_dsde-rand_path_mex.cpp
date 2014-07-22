// File generated on Sun Jan 05, 2014 10:45:18 PM by xcpp.
#include "mex.h"
#include <debugoff.h>
#include <excentury/excentury.h>
#include <spp/spp.h>
#include <spp/models/nfb_dsde.h>
#define EULER_DSDE_ZERO_BOUNDARY
#include <spp/methods/euler_dsde.h>
// RAND_PATH
#include <spp/collectors/rand_path.h>
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    size_t ncin_ = mxGetScalar(prhs[0]);
    char* pcin_ = mxArrayToString(prhs[1]);
    excentury::stextInterface<excentury::load_mode> XC_LI_(pcin_, ncin_);
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    double delay(1); XC_LI_.load(delay);
    XC_LI_.close();

    double dt = 0.001;
    nfb_dsde model(100);
    euler_dsde method(model, dt, delay);
    rand_path dc(method, npoints, ntrials, 1.0, false);

    // Setting the kernel(delay). History is by default all zeros.
    for (int i=0; i < method.kernel.dim(); ++i) method.kernel[i] = 0;
    if (method.kernel.dim() > 0) method.kernel[0] = 1.0/dt;

    for (int trial = 0; trial < ntrials; ++trial) {
        method.new_trial();
        dc.new_trial();
        while (method.time < INFINITY) {
            model.compute_dd(method.x, dt, method.hist, method.kernel);
            method.get_time_step();
            if(method.update_system()) break;
            if(dc.process_info()) break;
        }
        method.end_trial();
        dc.end_trial();
    }
    dc.end_sim();

    excentury::stextInterface<excentury::dump_mode> XC_DI_;
    if (ntrials == 1) {
        XC_DI_.dump(dc.t[0], "t");
        XC_DI_.dump(dc.x[0], "x");
    } else {
        XC_DI_.dump(dc.t, "t");
        XC_DI_.dump(dc.x, "x");
    }
    XC_DI_.close();
    std::string xc_mex_str_ = XC_DI_.str();
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    double* ncout_ = mxGetPr(plhs[0]);
    ncout_[0] = xc_mex_str_.size();
    plhs[1] = mxCreateString(xc_mex_str_.data());
    mxFree(pcin_);
}

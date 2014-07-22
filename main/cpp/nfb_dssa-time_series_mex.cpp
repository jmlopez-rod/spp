// File generated on Thu Jan 30, 2014 12:59:32 PM by xcpp.
#define XC_MATLAB
#include "mex.h"
#include <debugoff.h>
#include <excentury/excentury.h>
#include <spp/spp.h>
#include <spp/models/nfb_ssa.h>
#include <spp/methods/delayed_ssa.h>
// TIME_SERIES

#define EVENT_STACK_H linked_event_stack.h
#define EVENT_STACK LinkedEventStack
#include SPP_INCLUDE(spp/methods/EVENT_STACK_H)
#include <spp/collectors/time_series.h>
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    size_t ncin_ = mxGetScalar(prhs[0]);
    char* pcin_ = mxArrayToString(prhs[1]);
    excentury::STextInterface<excentury::load_mode> XC_LI_(pcin_, ncin_);
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    double freq(100.0); XC_LI_.load(freq);
    double delay(1); XC_LI_.load(delay);
    XC_LI_.close();

    NfbSSA model;
    DelayedSSA<EVENT_STACK> method(model);
    TimeSeries dc(method, npoints, ntrials, freq);

    for (int trial = 0; trial < ntrials; ++trial) {
        method.new_trial();
        method.dv[0] = delay;
        dc.new_trial();
        while (method.time < INFINITY) {
            model.eval_prop(method.x);
            method.get_time_step();
            if(method.update_system()) break;
            dc.process_info();
            if (dc.is_complete()) break;
        }
        method.end_trial();
        dc.end_trial();
    }
    dc.end_sim();
    #undef EVENT_STACK

    excentury::STextInterface<excentury::dump_mode> XC_DI_;
    XC_DI_.dump(dc.t, "t", dc.t[0]);
    if (ntrials == 1) {
        XC_DI_.dump(dc.x[0], "x", dc.x[0][0]);
    } else {
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

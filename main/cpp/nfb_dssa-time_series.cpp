// File generated on Thu Jan 30, 2014 12:57:19 PM by xcpp.
#include <debugoff.h>
#include <excentury/excentury.h>
#include <excentury/hook/cpp.h>
#include <spp/spp.h>
#include <spp/models/nfb_ssa.h>
#include <spp/methods/delayed_ssa.h>
// TIME_SERIES

#define EVENT_STACK_H linked_event_stack.h
#define EVENT_STACK LinkedEventStack
#include SPP_INCLUDE(spp/methods/EVENT_STACK_H)
#include <spp/collectors/time_series.h>
void xc_help() {
    fprintf(stderr,
    "program: nfb_dssa-time_series\n"
    "\ndescription:\n"
    "    Generate realizations from the Negative Feedback model by\n"
    "    collecting points that the DSSA generates at a given frequency.\n"
    "    This function returns `t` and `x`.\n"
    "\nparameters:\n"
    "    `npoints`: number of points per realization\n"
    "    `ntrials`: number of realizations\n"
    "    `freq`: collection frequency\n"
    "    `delay`: time by which an event is delayed\n"
    "\n");
}
void xc_input() {
    xc_help();
    excentury::TextInterface<excentury::dump_mode> XC_DI_(stdout);
    int npoints(1000); XC_DI_.dump(npoints, "npoints");
    int ntrials(1); XC_DI_.dump(ntrials, "ntrials");
    double freq(100.0); XC_DI_.dump(freq, "freq");
    double delay(1); XC_DI_.dump(delay, "delay");
    XC_DI_.close();
}
int main(int argc, char** argv) {
    /*Generate realizations from the Negative Feedback model by
    collecting points that the DSSA generates at a given frequency.
    This function returns `t` and `x`.*/
    excentury::check_inputs(argc);
    excentury::print_help(argv, xc_help);
    excentury::print_inputs(argv, xc_input);
    excentury::STextInterface<excentury::load_mode> XC_LI_(argv[1]);
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

    excentury::TextInterface<excentury::dump_mode> XC_DI_(stdout);
    XC_DI_.dump(dc.t, "t", dc.t[0]);
    if (ntrials == 1) {
        XC_DI_.dump(dc.x[0], "x", dc.x[0][0]);
    } else {
        XC_DI_.dump(dc.x, "x", dc.x[0]);
    }
    XC_DI_.close();
}


// File generated on Thu Jan 30, 2014 01:10:47 PM by xcpp.
#include <debugoff.h>
#include <excentury/excentury.h>
#include <excentury/hook/cpp.h>
#include <spp/spp.h>
#include <spp/models/nfb_dsde.h>
#define EULER_DSDE_ZERO_BOUNDARY
#include <spp/methods/euler_dsde.h>
// RAND_PATH
#include <spp/collectors/rand_path.h>
void xc_help() {
    fprintf(stderr,
    "program: nfb_dsde-rand_path\n"
    "\ndescription:\n"
    "    Generate realizations from the Negative Feedback model by\n"
    "    collecting each point that the DSSA generates. This function\n"
    "    returns `t` and `x`.\n"
    "\nparameters:\n"
    "    `npoints`: number of points per realization\n"
    "    `ntrials`: number of realizations\n"
    "    `delay`: time by which an event is delayed\n"
    "\n");
}
void xc_input() {
    xc_help();
    excentury::TextInterface<excentury::dump_mode> XC_DI_(stdout);
    int npoints(1000); XC_DI_.dump(npoints, "npoints");
    int ntrials(1); XC_DI_.dump(ntrials, "ntrials");
    double delay(1); XC_DI_.dump(delay, "delay");
    XC_DI_.close();
}
int main(int argc, char** argv) {
    /*Generate realizations from the Negative Feedback model by
    collecting each point that the DSSA generates. This function
    returns `t` and `x`.*/
    excentury::check_inputs(argc);
    excentury::print_help(argv, xc_help);
    excentury::print_inputs(argv, xc_input);
    excentury::STextInterface<excentury::load_mode> XC_LI_(argv[1]);
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    double delay(1); XC_LI_.load(delay);
    XC_LI_.close();

    double dt = 0.001;
    NfbDSDE model(100);
    EulerDSDE method(model, dt, delay);
    RandPath dc(method, npoints, ntrials, 1.0, false);

    // Setting the kernel(delay). History is by default all zeros.
    for (size_t i=0; i < method.kernel.dim(); ++i) method.kernel[i] = 0;
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

    excentury::TextInterface<excentury::dump_mode> XC_DI_(stdout);
    if (ntrials == 1) {
        XC_DI_.dump(dc.t[0], "t");
        XC_DI_.dump(dc.x[0], "x");
    } else {
        XC_DI_.dump(dc.t, "t");
        XC_DI_.dump(dc.x, "x");
    }
    XC_DI_.close();
}


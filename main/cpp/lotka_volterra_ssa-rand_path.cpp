// File generated on Thu Jan 30, 2014 01:06:26 PM by xcpp.
#include <debugoff.h>
#include <excentury/excentury.h>
#include <excentury/hook/cpp.h>
#include <spp/spp.h>
#include <spp/models/lotka_volterra_ssa.h>
#include <spp/methods/gillespie.h>
// RAND_PATH
#include <spp/collectors/rand_path.h>
void xc_help() {
    fprintf(stderr,
    "program: lotka_volterra_ssa-rand_path\n"
    "\ndescription:\n"
    "    Generate realizations from the Lotka-Volterra Model by\n"
    "    collecting each point that Gillespie's stochastic simulation\n"
    "    algorithm generates. This function returns `t` and `x`.\n"
    "\nparameters:\n"
    "    `npoints`: number of points per realization\n"
    "    `ntrials`: number of realizations\n"
    "\n");
}
void xc_input() {
    xc_help();
    excentury::TextInterface<excentury::dump_mode> XC_DI_(stdout);
    int npoints(1000); XC_DI_.dump(npoints, "npoints");
    int ntrials(1); XC_DI_.dump(ntrials, "ntrials");
    XC_DI_.close();
}
int main(int argc, char** argv) {
    /*Generate realizations from the Lotka-Volterra Model by
    collecting each point that Gillespie's stochastic simulation
    algorithm generates. This function returns `t` and `x`.*/
    excentury::check_inputs(argc);
    excentury::print_help(argv, xc_help);
    excentury::print_inputs(argv, xc_input);
    excentury::STextInterface<excentury::load_mode> XC_LI_(argv[1]);
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

    excentury::TextInterface<excentury::dump_mode> XC_DI_(stdout);
    if (ntrials == 1) {
        XC_DI_.dump(dc.t[0], "t", dc.t[0][0]);
        XC_DI_.dump(dc.x[0], "x", dc.x[0][0]);
    } else {
        XC_DI_.dump(dc.t, "t", dc.t[0]);
        XC_DI_.dump(dc.x, "x", dc.x[0]);
    }
    XC_DI_.close();
}


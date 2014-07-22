// File generated on Sun Jan 05, 2014 12:18:30 PM by xcpp.
/*Lotka-Volterra SSA

Set of functions to perform simulations on the Lotka-Volterra model
using Gillespie's Stochastic Simulation Algorith (SSA).*/
#include "mathlink.h"
#include <debugoff.h>

#include <excentury/excentury.h>
#include <spp/spp.h>
#include <models/lotka_volterra_ssa.h>
#include <methods/gillespie.h>
// RAND_PATH
#include <collectors/rand_path.h>
void randPath() {
    /*Generate realizations from the Lotka-Volterra Model by
    collecting each point that Gillespie's stochastic simulation
    algorithm generates. This function returns `t` and `x`.*/
    excentury::MLInterface<excentury::load_mode> XC_LI_;
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    XC_LI_.close();

    lotka_volterra_ssa model;
    gillespie method(model);
    rand_path dc(method, npoints, ntrials);

    for (int trial = 0; trial < ntrials; ++trial) {
        method.new_trial();
        dc.new_trial();
        while (method.time < INFINITY) {
            model.eval_prop(method.x);
            method.get_time_step();
            if(method.update_system()) break;
            if(dc.process_info()) break;
        }
        method.end_trial();
        dc.end_trial();
    }
    dc.end_sim();

    excentury::MLInterface<excentury::dump_mode> XC_DI_;
    if (ntrials == 1) {
        XC_DI_.dump(dc.t[0], "t");
        XC_DI_.dump(dc.x[0], "x");
    } else {
        XC_DI_.dump(dc.t, "t");
        XC_DI_.dump(dc.x, "x");
    }
    XC_DI_.close();
}

//--------------------------------------------------------------------
// TIME_SERIES
#include <collectors/time_series.h>
void timeSeries() {
    /*Generate realizations from the Lotka-Volterra Model by
    collecting points that Gillespie's stochastic simulation
    algorithm generates at a given frequency. This function returns
    `t` and `x`.*/
    excentury::MLInterface<excentury::load_mode> XC_LI_;
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    double freq(100.0); XC_LI_.load(freq);
    XC_LI_.close();

    lotka_volterra_ssa model;
    gillespie method(model);
    time_series dc(method, npoints, ntrials, freq);

    for (int trial = 0; trial < ntrials; ++trial) {
        method.new_trial();
        dc.new_trial();
        while (method.time < INFINITY) {
            model.eval_prop(method.x);
            method.get_time_step();
            if(method.update_system()) break;
            if(dc.process_info()) break;
        }
        method.end_trial();
        dc.end_trial();
    }
    dc.end_sim();

    excentury::MLInterface<excentury::dump_mode> XC_DI_;
    XC_DI_.dump(dc.t, "t");
    if (ntrials == 1) XC_DI_.dump(dc.x[0], "x");
    else XC_DI_.dump(dc.x, "x");
    XC_DI_.close();
}

int main(int argc, char* argv[]) {
    return MLMain(argc, argv);
}

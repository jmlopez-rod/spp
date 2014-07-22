// File generated on Sun Jan 05, 2014 12:53:25 PM by xcpp.
/*Lotka-Volterra SSA

Set of functions to perform simulations on the Lotka-Volterra model
using Gillespie's Stochastic Simulation Algorith (SSA).*/
#include <debugoff.h>
#include <excentury/excentury.h>
#include <spp/spp.h>
#include <models/lotka_volterra_ssa.h>
#include <spp/methods/gillespie.h>

extern "C" {
    void rand_path_py(size_t, char*, size_t&, char*&);
    void rand_path_py_clear();
    void time_series_py(size_t, char*, size_t&, char*&);
    void time_series_py_clear();

}

// RAND_PATH
#include <spp/collectors/rand_path.h>
std::string rand_path_py_str;
void rand_path_py(size_t ncin, char* pcin, size_t& ncout, char*& pcout) {
    excentury::stextInterface<excentury::load_mode> XC_LI_(pcin, ncin);
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

    excentury::stextInterface<excentury::dump_mode> XC_DI_;
    if (ntrials == 1) {
        XC_DI_.dump(dc.t[0], "t");
        XC_DI_.dump(dc.x[0], "x");
    } else {
        XC_DI_.dump(dc.t, "t");
        XC_DI_.dump(dc.x, "x");
    }
    XC_DI_.close();
    rand_path_py_str = XC_DI_.str();
    ncout = rand_path_py_str.size();
    pcout = (char*)rand_path_py_str.data();
}
void rand_path_py_clear() {
    rand_path_py_str.clear();
}

//--------------------------------------------------------------------
// TIME_SERIES
#include <spp/collectors/time_series.h>
std::string time_series_py_str;
void time_series_py(size_t ncin, char* pcin, size_t& ncout, char*& pcout) {
    excentury::stextInterface<excentury::load_mode> XC_LI_(pcin, ncin);
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

    excentury::stextInterface<excentury::dump_mode> XC_DI_;
    XC_DI_.dump(dc.t, "t");
    if (ntrials == 1) XC_DI_.dump(dc.x[0], "x");
    else XC_DI_.dump(dc.x, "x");
    XC_DI_.close();
    time_series_py_str = XC_DI_.str();
    ncout = time_series_py_str.size();
    pcout = (char*)time_series_py_str.data();
}
void time_series_py_clear() {
    time_series_py_str.clear();
}


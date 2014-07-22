// File generated on Thu Jan 30, 2014 08:05:26 AM by xcpp.
/*Negative FeedBack DSSA

Set of functions to perform simulations on the Negative Feedback
model using a delayed Stochastic Simulation Algorithm (DSSA).*/
#define XC_PYTHON
#include <debugoff.h>
#include <excentury/excentury.h>
#include <spp/spp.h>
#include <spp/models/nfb_ssa.h>
#include <spp/methods/delayed_ssa.h>

extern "C" {
    void rand_path_py(size_t, char*, size_t&, char*&);
    void rand_path_py_clear();
    void time_series_py(size_t, char*, size_t&, char*&);
    void time_series_py_clear();

}

// RAND_PATH
#include <spp/methods/linked_event_stack.h>
#include <spp/collectors/rand_path.h>
std::string rand_path_py_str;
void rand_path_py(size_t ncin, char* pcin, size_t& ncout, char*& pcout) {
try {
    excentury::STextInterface<excentury::load_mode> XC_LI_(pcin, ncin);
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    double delay(1); XC_LI_.load(delay);
    XC_LI_.close();

    nfb_ssa model;
    delayed_ssa<linked_event_stack> method(model);
    rand_path dc(method, npoints, ntrials);

    for (int trial = 0; trial < ntrials; ++trial) {
        method.new_trial();
        method.dv[0] = delay;
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

    excentury::STextInterface<excentury::dump_mode> XC_DI_;
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
} catch (excentury::RuntimeError& run_error) {
    ncout = run_error.size();
    pcout = (char*)run_error.c_str();
}
}
void rand_path_py_clear() {
    rand_path_py_str.clear();
}

// TIME_SERIES

#define EVENT_STACK linked_event_stack
#include SPP_INCLUDE(spp/methods/EVENT_STACK.h)
#include <spp/collectors/time_series.h>
std::string time_series_py_str;
void time_series_py(size_t ncin, char* pcin, size_t& ncout, char*& pcout) {
try {
    excentury::STextInterface<excentury::load_mode> XC_LI_(pcin, ncin);
    int npoints(1000); XC_LI_.load(npoints);
    int ntrials(1); XC_LI_.load(ntrials);
    double freq(100.0); XC_LI_.load(freq);
    double delay(1); XC_LI_.load(delay);
    XC_LI_.close();

    nfb_ssa model;
    delayed_ssa<EVENT_STACK> method(model);
    time_series dc(method, npoints, ntrials, freq);

    for (int trial = 0; trial < ntrials; ++trial) {
        method.new_trial();
        method.dv[0] = delay;
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
    #undef EVENT_STACK

    excentury::STextInterface<excentury::dump_mode> XC_DI_;
    XC_DI_.dump(dc.t, "t", dc.t[0]);
    if (ntrials == 1) XC_DI_.dump(dc.x[0], "x", dc.x[0][0]);
    else XC_DI_.dump(dc.x, "x", dc.x[0]);
    XC_DI_.close();
    time_series_py_str = XC_DI_.str();
    ncout = time_series_py_str.size();
    pcout = (char*)time_series_py_str.data();
} catch (excentury::RuntimeError& run_error) {
    ncout = run_error.size();
    pcout = (char*)run_error.c_str();
}
}
void time_series_py_clear() {
    time_series_py_str.clear();
}


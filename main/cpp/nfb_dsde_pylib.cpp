// File generated on Thu Jan 30, 2014 12:25:20 AM by xcpp.
/*Negative FeedBack DSDE

Set of functions to perform simulations on the Negative Feedback
model using a delayed stochastic differential equation simulator
(DSDE).*/
#define XC_PYTHON
#include <debugoff.h>
#include <excentury/excentury.h>
#include <spp/spp.h>
#include <spp/models/nfb_dsde.h>
#define EULER_DSDE_ZERO_BOUNDARY
#include <spp/methods/euler_dsde.h>

extern "C" {
    void rand_path_py(size_t, char*, size_t&, char*&);
    void rand_path_py_clear();

}

// RAND_PATH
#include <spp/collectors/rand_path.h>
std::string rand_path_py_str;
void rand_path_py(size_t ncin, char* pcin, size_t& ncout, char*& pcout) {
try {
    excentury::STextInterface<excentury::load_mode> XC_LI_(pcin, ncin);
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


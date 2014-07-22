// File generated on Wed Feb 12, 2014 10:20:24 PM by xcpp.
/*Tools

This collection of functions are examples of how to use the objects
in excentury/tools.*/
#define XC_PYTHON
//Comment before inclusion of excentury
#include <excentury/excentury.h>

extern "C" {
    void square_root_py(size_t, char*, size_t&, char*&);
    void square_root_py_clear();

}

//Comment for square_root
std::string square_root_py_str;
void square_root_py(size_t ncin, char* pcin, size_t& ncout, char*& pcout) {
try {
    excentury::STextInterface<excentury::load_mode> XC_LI_(pcin, ncin);
    double x(2); XC_LI_.load(x);
    double x0(0); XC_LI_.load(x0);
    int iter(10); XC_LI_.load(iter);
    XC_LI_.close();

    if (x < 0) {
        excentury::error("input `x` must be non-negative");
    }
    excentury::timer tm;
    double a = x;
    x = x0;
    tm.tic();
    for (int i=0; i < iter; ++i) {
        x = x - (x*x - a)/(2.0*x);
    }
    tm.toc(true);
    tm.print_date("%c\n");

    excentury::STextInterface<excentury::dump_mode> XC_DI_;
    XC_DI_.dump(x, "ans");
    XC_DI_.close();
    square_root_py_str = XC_DI_.str();
    ncout = square_root_py_str.size();
    pcout = (char*)square_root_py_str.data();
} catch (excentury::RuntimeError& run_error) {
    ncout = run_error.size();
    pcout = (char*)run_error.c_str();
}
}
void square_root_py_clear() {
    square_root_py_str.clear();
}
//Comment for square_root epilog


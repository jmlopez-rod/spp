/** GILLESPIE.h
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Fri Jan 03, 2014
 */

#ifndef SPP_GILLESPIE_H
#define SPP_GILLESPIE_H
#include "base_method.h"

/* VECTOR x methods:
     x.dim() <==> x.size(): Number of elements
     x[n] <==> x(n): access the nth entry
   MATRIX x methods:
     x.dim(0) <==> Number of rows
     x.dim(1) <==> Number of columns
     x(r, c) <==> x[r+c*x.dim(0)]
     x.size(0) <==> x.dim(0)
     x.size(1) <==> x.dim(0)*x.dim(2) */
class Gillespie: public BaseMethod {
public:
    /* base_method inheritance:
        const char* name;
        double time, time_step;
        VECTOR x, x_prev;
    */
    ModelSSA& model;
    MTRand rg1; // Generate next time step
    MTRand rg2; // Generate event
    double a0;  // Sum of propensity functions
    int event;  // The event chosen

    Gillespie(ModelSSA& m_):
        BaseMethod("Gillespie", m_.num_specs), model(m_)
    {
        trace("Gillespie('%s') executed.\n", m_.name);
    }
    void new_trial() {
        time = 0.0; time_step = 0.0;
        for (int i = 0; i < model.num_specs; ++i) {
            x[i] = x_prev[i] = model.x0[i];
        }
        trace("Gillespie.new_trial() executed.\n");
    }
    void get_time_step() {
        a0 = 0.0; event = 0;
        while (event < model.num_reacs) a0 += model.prop[event++];
        time_step = -log(rg1.rand())/a0;
        trace("Gillespie.get_time_step(): time_step <- %lf\n", time_step);
    }
    bool update_system() {
        double r1, pSum;
        if (a0 == 0.0) return true;
        r1 = a0*rg2.rand();
        pSum = 0.0;
        event = -1;
        while (pSum < r1) pSum += model.prop[++event];
        for (int i = 0; i < model.num_specs; ++i) {
            x_prev[i] = x[i];
            x[i] += model.z(i, event);
        }
        time += time_step;
        #if DEBUG == 3
        trace("Gillespie.update_system() ==> [t: %lf][x: ", time);
        for (int i = 0; i < model.num_specs-1; ++i) {
            trace("%lf,", x[i]);
        }
        trace("%lf]\n", x[model.num_specs-1]);
        #endif
        return false;
    }
    void end_trial() {
        trace("Gillespie.end_trial() executed.\n");
    }
};

#endif

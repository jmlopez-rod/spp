/** DELAYED_SSA.h
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Sun Jan 05, 2014
 *
 * NOTE: Before you include this file into another file you must have
 *  defined an object from which you can template the DelayedSSA
 *  method object. This object needs to have the following methods:
 *
 *      constructor(ssa_model& m)
 *      void reset()
 *      void put(double t_in, int e_in)
 *      void pull(double& t_out, int& e_out)
 *      void update()
 *      double check()
 *      bool isempty()
 *      int size()
 *
 */


#ifndef DELAYED_SSA_H
#define DELAYED_SSA_H

#ifdef SPP_DELAYED_SSA_DEBUG
#define DEBUGOFF SPP_DELAYED_SSA_DEBUG
#include <excentury/debugoff.h>
#endif

#include "base_method.h"

template<class Stack>
class DelayedSSA: public BaseMethod {
public:
    /* base_method inheritance:
        const char* name;
        double time, time_step;
        VECTOR x, x_prev; */
    ModelSSA& model;
    MTRand rg1;         // Generates next time step
    MTRand rg2;         // Generates event
    double a0;          // Sum of propensity functions
    int event;          // The event chosen
    VECTOR dv;          // The delay
    Stack e_stack;      // The event stack
    double t_out;       // The time out of the stack
    int e_out;          // The event out of the stack

    DelayedSSA(ModelSSA& m_):
        BaseMethod("DelayedSSA", m_.num_specs), model(m_),
        dv(m_.num_reacs), e_stack(m_)
    {
        trace("DelayedSSA('%s') executed.\n", m_.name);
    }
    void new_trial() {
        time = 0.0; time_step = 0.0;
        for (int i = 0; i < model.num_specs; ++i) {
            x[i] = x_prev[i] = model.x0[i];
        }
        for (int i = 0; i < model.num_reacs; ++i) {
            dv[i] = 0.0;
        }
        e_stack.reset();
        trace("DelayedSSA.new_trial() executed.\n");
    }
    void get_time_step() {
        a0 = event = 0;
        while (event < model.num_reacs) a0 += model.prop[event++];
        time_step = -log(rg1.rand())/a0;
        trace("DelayedSSA.get_time_step(): time_step <- %lf\n", time_step);
    }
    bool update_system() {
        double r1, prop_sum;
        if (time + time_step < e_stack.check()) {
            time += time_step;
            r1 = a0*rg2.rand();
            prop_sum = 0.0;
            event = -1;
            while (prop_sum < r1) prop_sum += model.prop[++event];
            if (dv[event] > 0.0) {
                e_stack.put(time+dv[event], event);
            } else {
                e_stack.update();
                for (int i = 0; i < model.num_specs; ++i) {
                    x_prev[i] = x[i];
                    x[i] += model.z(i, event);
                }
            }
        } else {
            if (e_stack.isempty()) {
                trace("DelayedSSA.update_system() ==> e_stack is emtpy.");
                return true;
            }
            e_stack.pull(t_out, e_out);
            for (int i = 0; i < model.num_specs; ++i) {
                x_prev[i] = x[i];
                x[i] += model.z(i, e_out);
            }
            time_step = t_out - time;
            time = t_out;
        }
        #if DEBUG == 3
        trace("DelayedSSA.update_system() ==> [t: %16.6lf][x: ", time);
        for (int i = 0; i < model.num_specs-1; ++i) {
            trace("%lf,", x[i]);
        }
        trace("%lf]\n", x[model.num_specs-1]);
        #endif
        return false;
    }
    void end_trial() {
        trace("DelayedSSA.end_trial() executed.\n");
    }
};

#ifdef SPP_DELAYED_SSA_DEBUG
#include <excentury/debug.h>
#endif

#endif

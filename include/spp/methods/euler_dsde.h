/** EULER_DSDE.h
 * Manuel Lopez - jmlopez@math.uh.edu
 * Chinmaya Gupta - chinmaya@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Jul 25, 2013
 */

#ifndef EULER_DSDE_H
#define EULER_DSDE_H
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
class EulerDSDE: public BaseMethod {
public:
    /* BaseMethod inheritance:
        const char* name;
        double time, time_step;
        VECTOR x, x_prev; */
    //@begin-properties
    ModelDSDE& model;
    MTRand rg;
    SimHistory hist;
    VECTOR kernel;
    VECTOR dw;
    //@end-properties

    EulerDSDE(ModelDSDE& m_, double dt_, double max_delay_):
        BaseMethod("EulerDSDE", m_.dim),
        model(m_),
        hist(int(max_delay_/dt_)+1, m_.dim),
        kernel(int(max_delay_/dt_)+1),
        dw(m_.dim)
    {
        time_step = dt_;
        trace("EulerDSDE('%s') executed.\n", m_.name);
        trace("    hist(LEN: %d, DIM: %d)\n", hist.len, hist.dim);
    }
    void new_trial() {
        time = 0.0;
        for (int i = 0; i < model.dim; ++i) {
            x[i] = x_prev[i] = model.x0[i];
        }
        hist.reset();
        hist.append(time, x);
        trace("EulerDSDE.new_trial() executed.\n");
    }
    void get_time_step() {
        // No need to call this function. Timestep is fixed.
    }
    bool update_system() {
        for (int i = 0; i < dw.dim(); ++i) {
            dw[i] = rg.normal(0,1)*sqrt(time_step);
        }
        // The following computes drift*time_step + diffusion*dw
        for (int i = 0; i < model.dim; ++i) {
            x_prev[i] = x[i];
            x[i] += model.drift[i]*time_step;
            for (int j=0; j < model.dim; ++j) {
                x[i] += model.diffusion(i, j)*dw[j];
            }
            #ifdef EULER_DSDE_ZERO_BOUNDARY
            x[i] = fmax(x[i], 0.0);
            #endif
        }
        time += time_step;
        hist.append(time, x);
        #if DEBUG == 3
        trace("EulerDSDE.update_system() ==> [t: %16.6lf][x: ", time);
        for (int i = 0; i < model.dim-1; ++i) {
            trace("%lf,", x[i]);
        }
        trace("%lf]\n", x[model.dim-1]);
        #endif
        return false;
    }
    void end_trial() {
        trace("EulerDSDE.end_trial() executed.\n");
    }
};

#endif

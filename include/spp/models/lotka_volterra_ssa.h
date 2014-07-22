/** LOTKA_VOLTERRA_SSA.h
 *
 * LotkaVolterra - Stochastic Simulation Algorithm
 *
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Thu Jan 30, 2014 11:08:12 AM
 */

#ifndef LOTKA_VOLTERRA_SSA_H
#define LOTKA_VOLTERRA_SSA_H
#include <spp/models/model_ssa.h>

class LotkaVolterraSSA: public ModelSSA {
public:
    /* ModelSSA inheritance:
        const char* name;
        const int num_specs, num_reacs;
        VECTOR x0, prop;
        MATRIX_INT z; */
    // @begin-properties
    double A, B, C, D;
    // @end-properties
    LotkaVolterraSSA(): ModelSSA("LotkaVolterraSSA", 2, 4) {
        A = 1.0;
        B = 0.001;
        C = 1.0;
        D = 0.001;
        x0[0] = 1000;
        x0[1] = 100;
        // The element z(s, r) denotes the change
        // to the species s due to the reaction r 
        z(0, 0) =  1;  z(1, 0) =  0;
        z(0, 1) = -1;  z(1, 1) =  0;
        z(0, 2) =  0;  z(1, 2) = -1;
        z(0, 3) =  0;  z(1, 3) =  1;
        trace("LotkaVolterraSSA() executed.\n");
    }
    void eval_prop(const VECTOR& sm) {
        #define r sm[0]
        #define w sm[1]
        prop[0] = A*r;
        prop[1] = B*r*w;
        prop[2] = C*w;
        prop[3] = D*r*w;
        #undef r
        #undef w
    }
};

#endif

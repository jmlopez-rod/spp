/** I1FFL_SSA.h
 *
 * i1ffl - Stochastic Simulation Algorithm
 *
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Mon May 19, 2014 12:15:36 PM
 */

#ifndef I1FFL_SSA_H
#define I1FFL_SSA_H
#include <spp/models/model_ssa.h>

class I1fflSSA: public ModelSSA {
public:
    /* ModelSSA inheritance:
        const char* name;
        const int num_specs, num_reacs;
        VECTOR x0, prop;
        MATRIX_INT z; */
    // @begin-properties
    double a1, a2, a3;
    double b1, b2, b3;
    double k;
    // @end-properties
    I1fflSSA(double k_, double xs_, double ys_, double zs_):
        ModelSSA("I1fflSSA", 3, 6), k(k_)
    {
        b1 = b2 = b3 = log(2.0);
        a1 = b1*xs_;
        a2 = b2*ys_*(k + xs_*xs_)/(xs_*xs_);
        a3 = b3*zs_*(k + xs_*xs_)/(xs_*xs_)*(ys_*ys_/k + 1);
        x0[0] = 0;
        x0[1] = 0;
        x0[2] = 0;
        // The element z(s, r) denotes the change
        // to the species s due to the reaction r
        for(int i=0; i < z.size(1); ++i) z[i] = 0;
        z(0, 0) =  1;  z(1, 0) =  0;  z(2, 0) =  0;
        z(0, 1) = -1;  z(1, 1) =  0;  z(2, 1) =  0;
        z(0, 2) =  0;  z(1, 2) =  1;  z(2, 2) =  0;
        z(0, 3) =  0;  z(1, 3) = -1;  z(2, 3) =  0;
        z(0, 4) =  0;  z(1, 4) =  0;  z(2, 4) =  1;
        z(0, 5) =  0;  z(1, 5) =  0;  z(2, 5) = -1;

        trace("I1fflSSA() executed.\n");
    }
    #define X sm[0]
    #define Y sm[1]
    #define Z sm[2]
    void eval_prop(const VECTOR& sm) {
        prop[0] = a1;
        prop[1] = b1*X;
        prop[2] = a2*(X*X/(k+X*X));
        prop[3] = b2*Y;
        prop[4] = a3*(X*X/(k+X*X))/(Y*Y/k+1);
        prop[5] = b3*Z;
    }
    #undef X
    #undef Y
    #undef Z
};

#endif

/** NFB_SSA.h
 *
 * Nfb - Stochastic Simulation Algorithm
 *
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Thu Jan 30, 2014 11:04:25 AM
 */

#ifndef NFB_SSA_H
#define NFB_SSA_H
#include <spp/models/model_ssa.h>

class NfbSSA: public ModelSSA {
public:
    /* ModelSSA inheritance:
        const char* name;
        const int num_specs, num_reacs;
        VECTOR x0, prop;
        MATRIX_INT z; */
    // @begin-properties
    double alpha;
    double beta;
    double gammar;
    double R0;
    double C1;
    // @end-properties
    NfbSSA(): ModelSSA("NfbSSA", 1, 2) {
        alpha = 300;
        beta = 0.1;
        gammar = 80.0;
        R0 = 1.0;
        C1 = 4.0;
        x0[0] = 0;
        // The element z(s, r) denotes the change
        // to the species s due to the reaction r
        z(0, 0) =  1;  z(0, 1) = -1;

        trace("NfbSSA() executed.\n");
    }
    void eval_prop(const VECTOR& sm) {
        #define x sm[0]
        prop[0] = alpha/(1.0 + pow(x/C1, 4.0));
        prop[1] = beta*x + gammar*x/(R0+x);
        #undef x
    }
};

#endif

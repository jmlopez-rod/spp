/** ${UPPER}(MODELNAME)_SSA.h
 *
 * $(MODELNAME) - Stochastic Simulation Algorithm
 *
 * $(USER) - $(USER)@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * $(DATE)
 */

#ifndef ${UPPER}(MODELNAME)_SSA_H
#define ${UPPER}(MODELNAME)_SSA_H
#include <spp/models/model_ssa.h>

class $(MODELNAME)SSA: public ModelSSA {
public:
    /* ModelSSA inheritance:
        const char* name;
        const int num_specs, num_reacs;
        VECTOR x0, prop;
        MATRIX_INT z; */
    // @begin-properties
    // @end-properties
    $(MODELNAME)SSA():
        ModelSSA("$(MODELNAME)SSA", $(NUMSPECS), $(NUMREACS))
    {
        $(X0)
        // The element z(s, r) denotes the change
        // to the species s due to the reaction r
        $(Z)
        trace("$(MODELNAME)SSA() executed.\n");
    }
    void eval_prop(const VECTOR& sm) {
        $(PROP)
    }
};

#endif

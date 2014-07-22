/** ${UPPER}(MODELNAME)_DSDE.h
 * $(USER) - $(USER)@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * $(DATE)
 */

#ifndef ${UPPER}(MODELNAME)_DSDE_H
#define ${UPPER}(MODELNAME)_DSDE_H
#include <spp/methods/sim_history.h>
#include <spp/models/model_dsde.h>

class $(MODELNAME)DSDE: public ModelDSDE {
private:
    VECTOR integrated;
public:
    /* sde_model inheritance:
        const int dim;    // Phase space dimension
        VECTOR x0;        // Initial population
        VECTOR drift;     // Same dimension as phase space
        MATRIX diffusion; // Diffusion matrix */
    // @begin-properties
    // @end-properties
    $(MODELNAME)DSDE():
        ModelDSDE("$(MODELNAME)DSDE", $(DIM)),
        integrated($(DIM))
    {
        $(X0)
        trace("$(MODELNAME)DSDE(dim:$(DIM)) executed.\n");
    }
    void compute_dd(const VECTOR& sm,
                    double dt,
                    const sim_history& hist,
                    const VECTOR& kernel)
    {
        trace("$(MODELNAME)DSDE.compute_dd() executed.\n");
    }
};

#endif

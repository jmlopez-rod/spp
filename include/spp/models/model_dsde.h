/** DSDE_MODEL.H
 * Manuel Lopez - jmlopez@math.uh.edu
 * Chinmaya Gupta - chinmaya@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * July 24, 2013
 */

#ifndef MODEL_DSDE_H
#define MODEL_DSDE_H

class ModelDSDE {
public:
    // @begin-properties
    const char* name;
    const int dim;    // Phase space dimension
    VECTOR x0;        // Initial population
    VECTOR drift;     // Same dimension as phase space
    MATRIX diffusion; // Diffusion matrix
    // @end-properties
    ModelDSDE(const char* name_, int dim_):
        name(name_), dim(dim_), x0(dim_),
        drift(dim_), diffusion(dim_, dim_)
    {
        trace("dsde_model(DIM:%d) executed.\n", dim_);
    }
    virtual void compute_dd(const VECTOR&,
                            double,
                            const SimHistory&,
                            const VECTOR&) = 0;
};

#endif

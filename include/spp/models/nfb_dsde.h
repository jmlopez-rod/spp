/** NFB_DSDE.h
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Thu Jan 30, 2014 10:57:36 AM
 */

#ifndef NFB_DSDE_H
#define NFB_DSDE_H
#include <spp/methods/sim_history.h>
#include <spp/models/model_dsde.h>

class NfbDSDE: public ModelDSDE {
private:
    VECTOR integrated;
public:
    /* sde_model inheritance:
        const int dim;    // Phase space dimension
        VECTOR x0;        // Initial population
        VECTOR drift;     // Same dimension as phase space
        MATRIX diffusion; // Diffusion matrix */
    // @begin-properties
    double alpha;
    double beta;
    double gammar;
    double R0;
    double C1;
    double N;
    // @end-properties
    NfbDSDE(double N_):
        ModelDSDE("NfbDSDE", 1),
        integrated(1),
        N(N_)
    {
        double xs = 100.0;
        alpha = N*300/xs;
        beta = 0.1;
        gammar = N*80.0/xs;
        R0 = N*1.0/xs;
        C1 = N*4.0/xs;
        x0[0] = 0;
        x0[0] = 0;
        trace("NfbDSDE(dim:1) executed.\n");
    }
    void compute_dd(const VECTOR& sm,
                    double dt,
                    const SimHistory& h,
                    const VECTOR& k)
    {
        #define x sm[0]
        #define xn 0
        for (size_t i=0; i < 1; ++i) { //Should range from 0 to h.len
            integrated[0] = alpha/(1.0 + pow(h(i,xn)/C1, 4.0))*k[i]*dt;
        }
        drift[0] = integrated[0] - beta*x - gammar*x/(R0 + x);
        diffusion(0,0) = sqrt(fabs(integrated[0]) +
                              fabs(beta*x) +
                              fabs(gammar*x/(R0+x)));
        #undef x
        #undef xn
        trace("NfbDSDE.compute_dd() executed.\n");
    }
};

#endif

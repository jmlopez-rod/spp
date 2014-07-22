/** RAND_PATH.h
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Fri Jan 03, 2014
 */

#ifndef SPP_RAND_PATH_H
#define SPP_RAND_PATH_H

/* VECTOR x methods:
     x.dim() <==> x.size(): Number of elements
     x[n] <==> x(n): access the nth entry
   MATRIX x methods:
     x.dim(0) <==> Number of rows
     x.dim(1) <==> Number of columns
     x(r, c) <==> x[r+c*x.dim(0)]
     x.size(0) <==> x.dim(0)
     x.size(1) <==> x.dim(0)*x.dim(2) */
class RandPath {
public:
    bool complete;
    BaseMethod& sm;
    /* BaseMethod properties:
        const char* name;
        double time, time_step;
        VECTOR x;
        VECTOR xOld; */
    VECTOR_OF(VECTOR) t;
    VECTOR_OF(MATRIX) x;
    const double N;
    const int npoints;
    const int ntrials;
    int trial, total;
    bool discrete;

    RandPath(BaseMethod& sm_,
              int np_, int nt_,
              double N_=1.0, bool discrete_=true):
        sm(sm_),
        complete(false),
        npoints(np_), ntrials(nt_),
        N(N_), discrete(discrete_),
        t(nt_), x(nt_)
    {
        trial = 0;
        trace("RandPath(METHOD:%s) executed.\n", sm.name);
    }
    void new_trial() {
        total = 0;
        t[trial].resize(npoints);
        t[trial][total] = sm.time;
        x[trial].resize(npoints, sm.x.dim());
        for (int pN = 0; pN < sm.x.dim(); ++pN) {
            x[trial](total, pN) = sm.x[pN]/N;
        }
        ++total;
        trace("RandPath.new_trial() executed.\n");
    }
    bool process_info() {
        t[trial][total] = sm.time;
        for (int pN = 0; pN < sm.x.dim(); ++pN) {
            x[trial](total, pN) = sm.x[pN]/N;
        }
        ++total;
        if (total >= npoints) {
            complete = true;
            trace("RandPath.processInfo() is done.\n");
            return true;
        }
        trace("RandPath.process_info() executed.\n");
        return false;
    }
    void end_trial() {
        trial++;
        complete = false;
        trace("RandPath.end_trial() executed.\n");
    }
    void end_sim() {
        trace("RandPath.end_sim() executed.\n");
    }
    bool is_complete() const {
        trace("RandPath.complete() executed: %s\n", 
              (complete ? "true": "false"));
        return complete;
    }
};

#endif

/** PEAKHEIGHT.h
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Mon May 19, 2014 03:34:26 PM
 */

#ifndef PEAKHEIGHT_H
#define PEAKHEIGHT_H

/* VECTOR x methods:
     x.dim() <==> x.size(): Number of elements
     x[n] <==> x(n): access the nth entry
   MATRIX x methods:
     x.dim(0) <==> Number of rows
     x.dim(1) <==> Number of columns
     x(r, c) <==> x[r+c*x.dim(0)]
     x.size(0) <==> x.dim(0)
     x.size(1) <==> x.dim(0)*x.dim(2) */
class PeakHeight {
public:
    bool complete;
    ssa_model& model;
    base_method& sm;
    /* base_method properties:
        
       ssa_model properties (using model):

         */
    PeakHeight(base_method& sm_): sm(sm_), complete(false) {
        // Initialize your collector here
        trace("PeakHeight(METHOD:%s) executed.\n", sm.name);
    }
    void new_trial() {
        // Reset variables if more than one trial is executed.
        trace("PeakHeight.new_trial() executed.\n");
    }
    bool process_info() {
        // This function will be called after the simulation
        // method has updated the system.
        trace("PeakHeight.process_info() executed.\n");
        return false;
    }
    void end_trial() {
        // Is there anything that needs to be done once the trial is
        // done?
        trace("PeakHeight.end_trial() executed.\n");
    }
    void end_sim() {
        // There might be code to write after the simulation is
        // completed. Write it here
        trace("PeakHeight.end_sim() executed.\n");
    }
    bool is_complete() const {
        trace("PeakHeight.complete() executed: %s\n", 
              (complete ? "true": "false"));
        return complete;
    }
};

#endif

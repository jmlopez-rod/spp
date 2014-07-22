/** ${UPPER}(COLLECTORNAME).h
 * $(USER) - $(USER)@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * $(DATE)
 */

#ifndef ${UPPER}(COLLECTORNAME)_H
#define ${UPPER}(COLLECTORNAME)_H

/* VECTOR x methods:
     x.dim() <==> x.size(): Number of elements
     x[n] <==> x(n): access the nth entry
   MATRIX x methods:
     x.dim(0) <==> Number of rows
     x.dim(1) <==> Number of columns
     x(r, c) <==> x[r+c*x.dim(0)]
     x.size(0) <==> x.dim(0)
     x.size(1) <==> x.dim(0)*x.dim(2) */
class $(COLLECTORNAME) {
public:
    bool complete;
    $(MODELNAME)& model;
    $(METHODNAME)& sm;
    /* $(METHODNAME) properties:
        $(STDMETHODPROP)$(METHODPROP)
       $(MODELNAME) properties (using model):

        $(STDMODELPROP)$(MODELPROP) */
    $(COLLECTORNAME)($(METHODNAME)& sm_): sm(sm_), complete(false) {
        // Initialize your collector here
        trace("$(COLLECTORNAME)(METHOD:%s) executed.\n", sm.name);
    }
    void new_trial() {
        // Reset variables if more than one trial is executed.
        trace("$(COLLECTORNAME).new_trial() executed.\n");
    }
    bool process_info() {
        // This function will be called after the simulation
        // method has updated the system.
        trace("$(COLLECTORNAME).process_info() executed.\n");
        return false;
    }
    void end_trial() {
        // Is there anything that needs to be done once the trial is
        // done?
        trace("$(COLLECTORNAME).end_trial() executed.\n");
    }
    void end_sim() {
        // There might be code to write after the simulation is
        // completed. Write it here
        trace("$(COLLECTORNAME).end_sim() executed.\n");
    }
    bool is_complete() const {
        trace("$(COLLECTORNAME).complete() executed: %s\n", 
              (complete ? "true": "false"));
        return complete;
    }
};

#endif

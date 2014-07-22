/** SIM_HISTORY.H

Container to store history from a simulation. This object is
initialized with the number of points it needs to hold as well as the
dimension of the system in the simulation (x.dim()).

When a new point is generated you need to call its function `append`.
The parameters for this function is the time and the VECTOR x from
the simulation.

To obtain data use the overloaded operators [] and (). When viewing
the time in the history use []. When viewing the state of the system
use (). The first parameter is the index in the history you wish to
view, the second is the population number.

Example:

    //Assume X is a vector of size 2
    SimHistory history(4, 2);
    history.append(0, X);
    history.append(1, X);
    history.append(2, X);
    history.append(3, X);
    history.append(4, X);
    printf("4 points ago the time was: %f\n", history[0]);
    printf("The current time is: %f\n", history[3])

This will print:

    4 points ago the time was: 1
    The current time is: 4

NOTE: The indices run from 0 to len-1, where len is the number of
points the SimHistory object holds. The must recent point is at
index len-1. The index 0 will always give you the last point stored
in the object.

Authors: Manuel Lopez - jmlopez@math.uh.edu
         Chinmaya Gupta - chinmaya@math.uh.edu

- Department of Mathematics, 
- University of Houston.
- July 24, 2013
*/

#ifndef SIM_HISTORY_H
#define SIM_HISTORY_H

class SimHistory {
private:
    VECTOR t;
    MATRIX x;
    int first;
    int last;
    int index;
public:
    const int len;
    const int dim;
    SimHistory(int len_, int dim_): len(len_), dim(dim_), 
        t(len_), x(len_, dim_)
    {
        for (int i = 0; i < len_; ++i) {
            t[i] = 0;
            for (int p = 0; p < dim_; ++p) {
                x(i, p) = 0;
            }
        }
        first = 0;
        last = len_ - 1;
        trace("SimHistory(LEN:%d, DIM:%d) executed.\n", len_, dim_);
    }
    double& operator[] (int i) {
        return t[(first+i)%len];
    }
    const double& operator[] (int i) const {
        return t[(first+i)%len];
    }
    double& operator() (int i1, int i2) {
        return x((first+i1)%len, i2);
    }
    const double& operator() (int i1, int i2) const {
        return x((first+i1)%len, i2);
    }

    void append(double t_, const VECTOR& x_) {
        first++;
        if (first >= len) {
            first -= len;
        }
        last = first + len - 1;
        index = last % len;
        t[index] = t_;
        for (int i=0; i < dim; ++i) {
            x(index, i) = x_[i];
        }
    }
    void reset() {
        for (int i = 0; i < t.dim(); ++i) {
            t[i] = 0;
            for (int p = 0; p < x.dim(1); ++p) {
                x(i, p) = 0;
            }
        }
    }
    void print() {
        for (int h=0; h < len; ++h) {
            printf("[%d] %f: ", h, (*this)[h]);
            for (int p=0; p < dim - 1; ++p) {
                printf("%f, ", (*this)(h, p));
            }
            printf("%f\n", (*this)(h, dim - 1));
        }
    }
 };
 
#endif

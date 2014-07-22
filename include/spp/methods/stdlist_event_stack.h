/** LINKED_EVENT_STACK.h
 * jmlopez - jmlopez@math.uh.edu
 * chinmaya - chinmaya@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Sun Jan 05, 2014
 */

#ifndef STDLIST_EVENT_STACK
#define STDLIST_EVENT_STACK
#include <list>

class STDListEventStack {
private:
    class EventNode {
    public:
        double time;
        int event;
        EventNode(): time(0), event(0) {}
        EventNode(double t, int e): time(t), event(e) {}
    };
    std::list<EventNode> data;
    ModelSSA& model;
    int length;
public:
    VECTOR x;
    VECTOR x_prev;
    STDListEventStack(ModelSSA& m):
        model(m), length(0),
        x(m.num_specs), x_prev(m.num_specs)
    {
        for (int i=0; i < model.num_specs; ++i) x[i] = x_prev[i] = 0.0;
    }
    void reset() {
        data.clear();
        length = 0;
        for (int i=0; i < model.num_specs; ++i) x[i] = x_prev[i] = 0.0;
    }
    void put(double t_in, int e_in) {
        if (length == 0) {
            data.push_back(EventNode(t_in, e_in));
        } else {
            for (std::list<EventNode>::reverse_iterator rit=data.rbegin();
                 rit!=data.rend();
                 ++rit)
            {
                if ((*rit).time > t_in) {
                    continue;
                } else {
                    data.insert(rit.base(), EventNode(t_in, e_in));
                    break;
                }
            }
        }
        for (int i = 0; i < model.num_specs; ++i) {
            x_prev[i] = x[i];
            x[i] += model.z(i, e_in);
        }
        ++length;
    }
    void pull(double& t_out, int& e_out) {
        t_out = data.front().time;
        e_out = data.front().event;
        for (int i = 0; i < model.num_specs; ++i) {
            x_prev[i] = x[i];
            x[i] -= model.z(i, e_out);
        }
        data.pop_front();
        --length;
    }
    void update() {
        for (int i=0; i < model.num_specs; ++i) x_prev[i] = x[i];
    }
    double check() {
        if (length > 0) return data.front().time;
        else return INFINITY;
    }
    bool isempty() {
        return length == 0;
    }
    int size() {
        return length;
    }
    void print() {
        for (std::list<EventNode>::iterator it=data.begin();
             it != data.end();
             ++it)
        {
            printf("%g, ", (*it).time);
        }
        printf("\n");
    }
};

#endif

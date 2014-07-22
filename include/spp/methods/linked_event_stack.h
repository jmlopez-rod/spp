/** LINKED_EVENT_STACK.h
 * jmlopez - jmlopez@math.uh.edu
 * Department of Mathematics,
 * University of Houston.
 * Sun Jan 05, 2014
 */

#ifndef LINKED_EVENT_STACK
#define LINKED_EVENT_STACK

class LinkedEventStack {
private:
    class EventNode {
    public:
        EventNode *left, *right;
        double time;
        int event;
        EventNode():
            time(0), event(0),
            left(NULL), right(NULL)
        {}
        EventNode(double t, int e):
            time(t), event(e),
            left(NULL), right(NULL)
        {}
    };
    EventNode *root, *end;
    ModelSSA& model;
    int length;
public:
    VECTOR x;
    VECTOR x_prev;
    LinkedEventStack(ModelSSA& m):
        model(m), length(0), end(NULL),
        x(m.num_specs), x_prev(m.num_specs)
    {
        for (int i=0; i < model.num_specs; ++i) x[i] = x_prev[i] = 0.0;
        root = new EventNode(-INFINITY, -1);
    }
    ~LinkedEventStack() {
        EventNode *temp, *current = root;
        while (current != NULL) {
            temp = current;
            current = current->right;
            delete temp;
        }
        root = NULL;
        end = NULL;
    }
    void reset() {
        EventNode *temp, *current = root->right;
        while (current != NULL) {
            temp = current;
            current = current->right;
            delete temp;
        }
        root->right = NULL;
        end = NULL;
        length = 0;
        for (int i=0; i < model.num_specs; ++i) x[i] = x_prev[i] = 0.0;
    }
    void put(double t_in, int e_in) {
        if (end == NULL) {
            end = new EventNode(t_in, e_in);
            end->left = root;
            root->right = end;
        } else {
            EventNode *current = end;
            while (t_in < current->time) {
                current = current->left;
            }
            EventNode* temp = new EventNode(t_in, e_in);
            temp->right = current->right;
            temp->left = current;
            current->right = temp;
            if (temp->right == NULL) {
                end = temp;
            } else {
                temp->right->left = temp;
            }
        }
        for (int i = 0; i < model.num_specs; ++i) {
            x_prev[i] = x[i];
            x[i] += model.z(i, e_in);
        }
        ++length;
    }
    void pull(double& t_out, int& e_out) {
        t_out = root->right->time;
        e_out = root->right->event;
        for (int i = 0; i < model.num_specs; ++i) {
            x_prev[i] = x[i];
            x[i] -= model.z(i, e_out);
        }
        --length;
        EventNode* temp = root->right;
        EventNode* next = temp->right;
        root->right = next;
        if (next == NULL) {
            end = NULL;
        }else {
            next->left = root;
        }
        delete temp;
    }
    void update() {
        for (int i=0; i < model.num_specs; ++i) {
            x_prev[i] = x[i];
        }
    }
    double check() {
        if (length > 0) return root->right->time;
        else return INFINITY;
    }
    bool isempty() {
        return length == 0;
    }
    int size() {
        return length;
    }
    void print() {
        EventNode* current = root->right;
        while (current != NULL) {
            printf("%g, ", current->time);
            current = current->right;
        }
        printf("\n");
    }
};

#endif

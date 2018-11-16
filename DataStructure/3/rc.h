#ifndef RC_H
#define RC_H

typedef struct RcInner {
    unsigned int count;
    struct RcInner *pre;
    unsigned int idx;
} RcInner;

// A reference-counted smart pointer.
typedef RcInner * Rc;

Rc Rc_new(Rc pre, unsigned int idx);
Rc Rc_clone(Rc rc);
void Rc_drop(Rc rc);

#endif
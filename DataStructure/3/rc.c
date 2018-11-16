#include "rc.h"

#include <stdlib.h>

// Create new Rc.
// Returns NULL on allocation error.
Rc Rc_new(Rc pre, unsigned int idx) {
    Rc rc = (Rc) malloc(sizeof(RcInner));
    if (rc == NULL) {
        return NULL;
    }
    rc->count = 1;
    rc->pre = pre;
    rc->idx = idx;
    return rc;
}

// Increment the counter for this Rc.
Rc Rc_clone(Rc rc) {
    rc->count += 1;
    return rc;
}

// Destroy this Rc.
// This decrements the counter for this Rc. If the counter hits 0,
// the memory is freed and its contents are also destroyed.
void Rc_drop(Rc rc) {
    for (;;) {
        if (rc == NULL) {
            return;
        }
        rc->count -= 1;
        if (rc->count != 0) {
            return;
        }
        // rc->count == 0.
        Rc old = rc;
        // Read rc->pre before we free rc.
        rc = rc->pre;
        free(old);
    }
}
#include "vec.h"

#include <stdlib.h>
#include "rc.h"

void Element_drop(Element *elem) {
    Rc_drop(elem->rc);
}

void Vec_init(Vec *vec) {
    // No allocation if this stays empty!
    vec->cap = 0;
}

// Returns 0 on success, 1 on allocation error.
int Vec_push(Vec *vec, Element elem) {
    // Check if this vec was just initialized. If so, allocate.
    if (vec->cap == 0) {
        vec->ptr = (Element *) malloc(sizeof(Element) * 4);
        if (vec->ptr == NULL) {
            return 1;
        }
        vec->cap = 4;
        vec->len = 1;
        *(vec->ptr) = elem;
        return 0;
    }
    // If vec is full, double capacity and realloc.
    if (vec->len == vec->cap) {
        vec->cap *= 2;
        vec->ptr = (Element *) realloc(vec->ptr, vec->cap * sizeof(Element));
        if (vec->ptr == NULL) {
            return 1;
        }
    }
    (vec->ptr)[vec->len] = elem;
    vec->len += 1;
    return 0;
}

// Returns 0 on success, 1 on empty vec.
// When 1 is returned, location pointed by dst will stay as-is.
int Vec_pop(Vec *vec, Element *dst) {
    if (vec->len == 0) {
        return 1;
    }
    vec -> len -= 1;
    *dst = (vec->ptr)[vec->len];
    return 0;
}

// Destroy this vector and its contents.
void Vec_drop(Vec *vec) {
    if (vec->cap != 0) {
        Element elem;
        while (Vec_pop(vec, &elem) == 0) {
            Element_drop(&elem);
        }
        free(vec->ptr);
    }
}
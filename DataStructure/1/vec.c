#include "vec.h"
#include "term.h"

#include <stdlib.h>

// Initialize the given Vec.
// Given Vec is assumed to be uninitialized; If it was initialized,
// it will be leaked.
void Vec_init(Vec* uninit) {
    uninit->ptr = NULL;
}

// Initialize the given Vec with given capacity.
// Given Vec is assumed to be uninitialized; If it was initialized,
// it will be leaked.
// returns 0 on success, 1 on allocation error.
int Vec_init_with_capacity(Vec* uninit, unsigned int capacity) {
    uninit->ptr = (Term*) malloc(capacity * sizeof(Term));
    if (uninit->ptr == NULL) {
        return 1;
    }
    uninit->len = 0;
    uninit->cap = capacity;
    return 0;
}

// Push given Term onto the given Vec.
// Given Vec should be initialized..
// returns 0 on success, 1 on allocation error.
int Vec_push(Vec* vec, Term term) {
    if (vec->ptr == NULL) {
        vec->len = 0;
        vec->cap = 1;
        vec->ptr = (Term*) malloc(sizeof(Term));
        if (vec->ptr == NULL) {
            return 1;
        }
    } else if (vec->len == vec->cap) {
        vec->cap *= 2;
        vec->ptr = (Term*) realloc(vec->ptr, vec->cap * sizeof(Term));
        if (vec->ptr == NULL) {
            return 1;
        }
    }
    *(vec->ptr + vec->len) = term;
    vec->len += 1;
    return 0;
}

// Index this Vec without performing bounds check.
Term Vec_index_unchecked(Vec vec, unsigned int idx) {
    return (vec.ptr)[idx];
}

// Sort given Vec by index.
void Vec_qsort(Vec vec) {
    qsort(vec.ptr, vec.len, sizeof(Term), Term_sort_cmp_idx);
}

// Drop the given Vec and perform cleanup.
// Given Vec is assumed to be initialized, and will be uninitialized
// after the call.
void Vec_drop(Vec* vec) {
    free(vec->ptr);
}
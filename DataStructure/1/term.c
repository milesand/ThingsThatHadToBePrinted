#include "term.h"

// Comparation function for sorting by index.
int Term_sort_cmp_idx(const void* a, const void* b) {
    return (((Term*) a)->idx) - (((Term*) b)->idx);
}
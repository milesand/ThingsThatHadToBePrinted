#ifndef TERM_H
#define TERM_H

typedef struct {
    unsigned int idx;
    int val;
} Term;

int Term_sort_cmp_idx(const void*, const void*);

#endif
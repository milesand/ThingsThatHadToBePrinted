#ifndef VEC_H
#define VEC_H

#include "term.h"

typedef struct {
    Term* ptr;
    unsigned int len;
    unsigned int cap;
} Vec;


void Vec_init(Vec*);
int Vec_init_with_capacity(Vec*, unsigned int);
int Vec_push(Vec*, Term);
Term Vec_index_unchecked(Vec, unsigned int);
void Vec_qsort(Vec);
void Vec_drop(Vec*);

#endif
#ifndef VEC_H
#define VEC_H

#include "rc.h"

typedef struct {
    Rc rc;
    unsigned int steps;
} Element;

void Element_drop(Element *elem);

// A dynamic, growable array.
// Pretty much a stack.
typedef struct {
    Element *ptr;
    unsigned int len;
    unsigned int cap;
} Vec;

void Vec_init(Vec *vec);
int Vec_push(Vec *vec, Element elem);
int Vec_pop(Vec *vec, Element *dst);
void Vec_drop(Vec *vec);

#endif
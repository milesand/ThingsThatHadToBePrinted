#ifndef TERM_H
#define TERM_H

#include <stdio.h>

typedef struct Term {
    int coef;
    unsigned int expon; // Apparently exp is a function in math.h. We don't use
                        // that header here, and this is a field name so they
                        // probably don't collide. Buuut just to be safe.
} Term;

Term Term_new(int, unsigned int);
int Term_fprint(FILE*, Term, char*, char*, char*, char*, char*);

#endif
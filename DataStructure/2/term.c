#include "term.h"

#include <stdio.h>

// Create a new term.
// This makes working with term struct slightly nicer.
Term Term_new(int coef, unsigned int expon) {
    Term term;
    term.coef = coef;
    term.expon = expon;
    return term;
}

// Helper function for keeping the code slightly DRYer.
// This dispatches to an appropriate fprintf call for the term,
// and returns the fprintf's return value directly.
int Term_fprint(FILE *stream, Term term, char *var, char *expon_zero, 
                char *expon_one, char *coef_one, char *coef_minus_one,
                char *otherwise) {
    if (term.expon == 0) {
        return fprintf(stream, expon_zero, term.coef);
    } else if (term.expon == 1) {
        return fprintf(stream, expon_one, term.coef, var);
    } else if (term.coef == 1) {
        return fprintf(stream, coef_one, var, term.expon);
    } else if (term.coef == -1) {
        return fprintf(stream, coef_minus_one, var, term.expon);
    } else {
        return fprintf(stream, otherwise, term.coef, var, term.expon);
    }
}
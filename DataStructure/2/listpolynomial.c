#include "listpolynomial.h"

#include <stdio.h>
#include "term.h"
#include "list.h"

// Add two polynomials represented by lists and store the result in the out list.
// Contents of Lists in1 and in2 will not be modified.
// in1 and in2 should be sorted in the decreasing order of exponent.
// out is expected to be an empty, valid list. If it was valid but not empty,
// result of addition will be inserted in front of anything that was already
// in the list. Returns 0 on success, 1 on allocation error.
int List_polynomial_add(List *in1, List *in2, List *out) {
    // We could check whether the in-lists are sorted before doing anything
    // and error out if they're not sorted; or actually sort them, maybe.
    // But I believe that is out of scope for this assignment
    // and I don't feel like doing that right now. Maybe later.
    Link src1 = in1->head;
    Link src2 = in2->head;
    Link *dst = &(out->head);
    while (src1 != NULL && src2 != NULL) {
        unsigned int expon1 = src1->term.expon;
        unsigned int expon2 = src2->term.expon;
        if (expon1 > expon2) {
            if (Link_insert(dst, src1->term) != 0) {
                return 1;
            }
            src1 = src1->next;
        } else if (expon1 < expon2) {
            if (Link_insert(dst, src2->term) != 0) {
                return 1;
            }
            src2 = src2->next;
        } else { // expon1 == expon2
            int coef = src1->term.coef + src2->term.coef;
            if (coef != 0) {
                if (Link_insert(dst, Term_new(coef, expon1)) != 0) {
                    return 1;
                };
            }
            src1 = src1->next;
            src2 = src2->next;
        }
        dst = &((*dst)->next);
    }
    while (src1 != NULL) {
        if (Link_insert(dst, src1->term) != 0) {
            return 1;
        }
        src1 = src1->next;
        dst = &((*dst)->next);
    }
    while (src2 != NULL) {
        if (Link_insert(dst, src2->term) != 0) {
            return 1;
        }
        src2 = src2->next;
        dst = &((*dst)->next);
    }
    return 0;
}

// Print given list as a polynomial with given variable name, to the stream.
// returns 0 on success, 1 on IO error.
int List_polynomial_fprint(FILE *stream, List *list, char *var) {
    Node *node = list->head;
    if (node == NULL) {
        if (fputc('0', stream) == EOF) {
            return 1;
        }
        return 0;
    } else {
        if (Term_fprint(stream, node->term, var, "%d", "%s^%u", "-%s^%u", "%d%s^%u") < 0) {
            return 1;
        }
        node = node->next;
    }
    while (node != NULL) {
        if (Term_fprint(stream, node->term, var, "%+d", "+%s^%u", "-%s^%u", "%+d%s^%u") < 0) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}
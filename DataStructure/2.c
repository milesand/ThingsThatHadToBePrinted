#include <stdlib.h>
#include <stdio.h>

typedef struct Term {
    int coef;
    unsigned int expon; // Apparently exp is a function in math.h. We don't use
                        // that header here, and this is a field name so they
                        // probably don't collide. Buuut just to be safe.
} Term;

typedef struct Node {
    Term term;
    struct Node *next;
} Node;

typedef Node* Link;

typedef struct List {
    struct Node *head;
} List;

// Function prototypes.
// Actually this one works well without these. They're just here for completeness.
void List_init(List*);
Node* Node_new(Term);
Term Term_new(int, unsigned int);
int Link_insert(Link*, Term);
int List_insert(List*, Term);
int List_polynomial_add(List*, List*, List*);
int Term_fprint(FILE*, Term, char*, char*, char*, char*, char*);
int List_polynomial_fprint(FILE*, List*, char*);
void List_free(List*);

// Initialize this list.
void List_init(List *list) {
    list->head = NULL;
}

// Create a new Node and return a pointer to it.
// If allocation failed, the pointer will be NULL.
Node* Node_new(Term term) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node != NULL) {
        node->term = term;
        node->next = NULL;
    }
    return node;
}

// Create a new term.
// This makes working with term struct slightly nicer.
Term Term_new(int coef, unsigned int expon) {
    Term term;
    term.coef = coef;
    term.expon = expon;
    return term;
}

// Push given term between this link and the Node pointed by this link.
// Returns 0 on success, 1 on allocation error.
int Link_insert(Link *link, Term term) {
    Node *node = Node_new(term);
    if (node == NULL) {
        return 1;
    }
    node->next = *link;
    *link = node;
    return 0;
}

// Push given term to the head of this list.
// Returns 0 on success, 1 on allocation error.
int List_insert(List *list, Term term) {
    return Link_insert(&(list->head), term);
}

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

// Helper function for keeping the code slightly DRYer.
// This dispatches to an appropriate fprintf call for the term,
// and returns the fprintf's return value directly.
int Term_fprint(FILE *stream, Term term, char *var, char *expon_zero, 
                char *coef_one, char *coef_minus_one, char *otherwise) {
    if (term.expon == 0) {
        return fprintf(stream, expon_zero, term.coef);
    } else if (term.coef == 1) {
        return fprintf(stream, coef_one, var, term.expon);
    } else if (term.coef == -1) {
        return fprintf(stream, coef_minus_one, var, term.expon);
    } else {
        return fprintf(stream, otherwise, term.coef, var, term.expon);
    }
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

// Free memory allocated for this list.
// The list will contain dangling pointer after this; List_init again
// if you want to use it again.
void List_free(List *list) {
    // I thought I should to *something* not in the book, so here it is.
    // Besides, leaking memory is probably not something you'd normally want.
    Node *node = list->head;
    Node *tmp;
    while (node != NULL) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
}

int main() {
    List a;
    List b;
    List c;

    List_init(&a);
    List_init(&b);
    List_init(&c);

    List_insert(&a, Term_new(-9, 0));
    List_insert(&a, Term_new(-2, 2));
    List_insert(&a, Term_new(7, 3));
    List_insert(&a, Term_new(3, 6));

    List_insert(&b, Term_new(1, 0));
    List_insert(&b, Term_new(6, 1));
    List_insert(&b, Term_new(6, 2));
    List_insert(&b, Term_new(-4, 4));
    List_insert(&b, Term_new(-2, 6));

    List_polynomial_add(&a, &b, &c);
    
    List_free(&a);
    List_free(&b);
    int err = List_polynomial_fprint(stdout, &c, "x");
    List_free(&c);
    if (err != 0) {
        // If printing to stderr also fails, there's not much we can do.
        // So no error handling here.
        fputs("Printing to stdout failed", stderr);
        return 1;
    }
    if (putchar('\n') == EOF) {
        // Copied and pasted to make my code WETter.
        // I don't think this kind of copy-paste can be avoided, though.
        fputs("Printing to stdout failed", stderr);
        return 1;
    }

    return 0;
}

// This prints "x^6-4x^4+7x^3+4x^2+6x-8\n" to stdout.
// or maybe "Printing to stdout failed\n" to stderr.
// or maybe nothing to nowhere if both stdout and stderr are dead or something.

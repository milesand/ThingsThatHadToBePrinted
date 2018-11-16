#include <stdio.h>
#include "term.h"
#include "list.h"
#include "listpolynomial.h"

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

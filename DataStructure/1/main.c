#include "sparsematrix.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    SparseMatrix m1, m2, mret;
    int err;
    err = SparseMatrix_init_with_capacity(&m1, 6, 6, 7);
    if (err == 1) {
        fputs("Allocation error", stderr);
        return 1;
    }
    err = SparseMatrix_init_with_capacity(&m2, 6, 6, 6);
    if (err == 1) {
        fputs("Allocation error", stderr);
        return 1;
    };

    // We skip error checks for insert calls because 
    // 1. all locations are constant and makes sense and
    // 2. we know we have enough capacity.
    SparseMatrix_insert(&m1, 0, 0, 1);
    SparseMatrix_insert(&m1, 1, 1, 1);
    SparseMatrix_insert(&m1, 2, 1, 1);
    SparseMatrix_insert(&m1, 3, 2, 1);
    SparseMatrix_insert(&m1, 4, 3, 1);
    SparseMatrix_insert(&m1, 5, 1, 2);
    SparseMatrix_insert(&m1, 5, 3, -1);

    SparseMatrix_insert(&m2, 0, 0, 1);
    SparseMatrix_insert(&m2, 1, 0, 1);
    SparseMatrix_insert(&m2, 2, 1, 1);
    SparseMatrix_insert(&m2, 3, 2, 1);
    SparseMatrix_insert(&m2, 4, 0, 1);
    SparseMatrix_insert(&m2, 5, 0, 1);

    err = SparseMatrix_add(&m1, &m2, &mret) == 2;
    if (err == 2) {
        fputs("Allocation error", stderr);
        return 1;
    }
    SparseMatrix_print(&mret, stdout);

    SparseMatrix_drop(&m1);
    SparseMatrix_drop(&m2);
    SparseMatrix_drop(&mret);
    return 0;
}
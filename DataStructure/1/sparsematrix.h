#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include "vec.h"
#include <stdio.h>

typedef struct {
    Vec terms;
    unsigned int rows;
    unsigned int cols;
} SparseMatrix;

void SparseMatrix_init(SparseMatrix*, unsigned int, unsigned int);
int SparseMatrix_init_with_capacity(SparseMatrix*, unsigned int, unsigned int, unsigned int);
int SparseMatrix_insert(SparseMatrix*, unsigned int, unsigned int, int);
int SparseMatrix_add(SparseMatrix*, SparseMatrix*, SparseMatrix*);
void SparseMatrix_print(SparseMatrix*, FILE*);
void SparseMatrix_drop(SparseMatrix*);

#endif
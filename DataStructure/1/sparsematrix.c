#include "sparsematrix.h"
#include "vec.h"
#include "term.h"

#include <stdio.h>

// Initialize the given SparseMatrix with the given size.
// Given SparseMatrix is assumed to be uninitialized; If it was initialized,
// it will be leaked.
void SparseMatrix_init(SparseMatrix* uninit, unsigned int rows, unsigned int cols) {
    Vec_init(&(uninit->terms));
    uninit->rows = rows;
    uninit->cols = cols;
}

// Initialize the given SparseMatrix with the given size and given capacity for
// underlying storage. Given SparseMatrix is assumed to be uninitialized; If it
// was initialized, it will be leaked.
int SparseMatrix_init_with_capacity(SparseMatrix* uninit, unsigned int rows, unsigned int cols, unsigned int capacity) {
    if (Vec_init_with_capacity(&(uninit->terms), capacity) != 0) {
        return 1;
    };
    uninit->rows = rows;
    uninit->cols = cols;
    return 0;
}

// Insert given value into the given location in the matrix.
// If given location was already populated, I'm not sure what will happen.
// So don't do that.
// Look, this is just a homework and the book does this in a worse way, okay?
// One COULD check all existing cells, but at what cost? (Spoiler: it's O(n).)
// returns 0 on success, 1 on invalid location, 2 on allocation error.
int SparseMatrix_insert(SparseMatrix* m, unsigned int row, unsigned int col, int val) {
    if (row >= m->rows || col >= m->cols) {
        return 1;
    }
    Term term;
    term.idx = ((m->cols) * row + col);
    term.val = val;
    if (Vec_push(&(m->terms), term) != 0) {
        return 2;
    };
    return 0;
}

// Add m1 and m2, and puts the result into mret.
// mret is considered to be uninitialized; If it was initialized,
// it will be leaked.
// returns 0 on success, 1 if m1 and m2 has different sizes, 2 on allocation error.
int SparseMatrix_add(SparseMatrix* m1, SparseMatrix* m2, SparseMatrix* mret) {
    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        return 1;
    }
    SparseMatrix_init(mret, m1->rows, m1->cols);
    Vec terms1 = m1->terms;
    Vec terms2 = m2->terms;
    Vec* termsres = &(mret->terms);
    Vec_qsort(terms1);
    Vec_qsort(terms2);
    unsigned int i = 0, j = 0;
    if (i < terms1.len && j < terms2.len) {
        Term term1 = Vec_index_unchecked(terms1, i);
        Term term2 = Vec_index_unchecked(terms2, j);
        for (;;) {
            if (term1.idx < term2.idx) {
                if (Vec_push(termsres, term1) == 1) {
                    return 2;
                };
                i += 1;
                if (i >= terms1.len) {
                    if (Vec_push(termsres, term2) == 1) {
                        return 2;
                    };
                    j += 1;
                    for (; j < terms2.len; j += 1) {
                        if (Vec_push(termsres, Vec_index_unchecked(terms2, j)) == 1) {
                            return 2;
                        };
                    }
                    break;
                }
                term1 = Vec_index_unchecked(terms1, i);
            } else if (term1.idx > term2.idx) {
                if (Vec_push(termsres, term2) == 1) {
                    return 2;
                };
                j += 1;
                if (j >= terms2.len) {
                    if (Vec_push(termsres, term1) == 1) {
                        return 2;
                    };
                    i += 1;
                    for (; i < terms1.len; i += 1) {
                        if (Vec_push(termsres, Vec_index_unchecked(terms1, i)) == 1) {
                            return 2;
                        };
                    }
                    break;
                }
                term2 = Vec_index_unchecked(terms2, j);
            } else {
                term1.val += term2.val;
                if (Vec_push(termsres, term1) == 1) {
                    return 2;
                };
                i += 1;
                if (i >= terms1.len) {
                    if (Vec_push(termsres, term2) == 1) {
                        return 2;
                    };
                    j += 1;
                    for (; j < terms2.len; j += 1) {
                        if (Vec_push(termsres, Vec_index_unchecked(terms2, j)) == 1) {
                            return 2;
                        };
                    }
                    break;
                }
                term1 = Vec_index_unchecked(terms1, i);
                j += 1;
                if (j >= terms2.len) {
                    if (Vec_push(termsres, term1) == 1) {
                        return 2;
                    };
                    i += 1;
                    for (; i < terms1.len; i += 1) {
                        if (Vec_push(termsres, Vec_index_unchecked(terms1, i)) == 1) {
                            return 2;
                        };
                    }
                    break;
                }
                term2 = Vec_index_unchecked(terms2, j);
            }
        }
    }
    return 0;
}
// Print the gicen matrix to given file.
// No IO Error handling since I am not in the mood for that.
void SparseMatrix_print(SparseMatrix* m, FILE* stream) {
    Vec terms = m->terms;
    Vec_qsort(terms);
    unsigned int col = 0, idx = 0;
    unsigned int i = 0;
    for (; i < terms.len; i += 1) {
        Term term = Vec_index_unchecked(terms, i);
        while (idx < term.idx) {
            idx += 1;
            col += 1;
            if (col == m->cols) {
                col = 0;
                fputs("0\n", stream);
            } else {
                fputs("0 ", stream);
            }
        }
        idx += 1;
        col += 1;
        if (col == m->cols) {
            col = 0;
            fprintf(stream, "%d\n", term.val);
        } else {
            fprintf(stream, "%d ", term.val);
        }
    }
    for (; idx < m->rows * m->cols; idx += 1) {
        col += 1;
        if (col == m->cols) {
            col = 0;
            fputs("0\n", stream);
        } else {
            fputs("0 ", stream);
        }
    }
}

// Drop the given matrix and perform cleanup.
// The matrix will be uninitialized after the call.
void SparseMatrix_drop(SparseMatrix* m) {
    Vec_drop(&(m->terms));
}
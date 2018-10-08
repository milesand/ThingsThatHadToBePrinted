#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int idx;
    int val;
} Term;

typedef struct {
    Term* ptr;
    unsigned int len;
    unsigned int cap;
} Vec;

typedef struct {
    Vec terms;
    unsigned int rows;
    unsigned int cols;
} SparseMatrix;

void Vec_init(Vec*);
int Vec_init_with_capacity(Vec*, unsigned int);
int Vec_push(Vec*, Term);
Term Vec_index_unchecked(Vec, unsigned int);
int Term_sort_cmp(const void*, const void*);
void Vec_qsort(Vec);
void Vec_drop(Vec*);
void SparseMatrix_init(SparseMatrix*, unsigned int, unsigned int);
int SparseMatrix_init_with_capacity(SparseMatrix*, unsigned int, unsigned int, unsigned int);
int SparseMatrix_insert(SparseMatrix*, unsigned int, unsigned int, int);
int SparseMatrix_add(SparseMatrix*, SparseMatrix*, SparseMatrix*);
void SparseMatrix_print(SparseMatrix*, FILE*);
void SparseMatrix_drop(SparseMatrix*);

// Initialize the given Vec.
// Given Vec is assumed to be uninitialized; If it was initialized,
// it will be leaked.
void Vec_init(Vec* uninit) {
    uninit->ptr = NULL;
}

// Initialize the given Vec with given capacity.
// Given Vec is assumed to be uninitialized; If it was initialized,
// it will be leaked.
// returns 0 on success, 1 on allocation error.
int Vec_init_with_capacity(Vec* uninit, unsigned int capacity) {
    uninit->ptr = (Term*) malloc(capacity * sizeof(Term));
    if (uninit->ptr == NULL) {
        return 1;
    }
    uninit->len = 0;
    uninit->cap = capacity;
    return 0;
}

// Push given Term onto the given Vec.
// Given Vec should be initialized..
// returns 0 on success, 1 on allocation error.
int Vec_push(Vec* vec, Term term) {
    if (vec->ptr == NULL) {
        vec->len = 0;
        vec->cap = 1;
        vec->ptr = (Term*) malloc(sizeof(Term));
        if (vec->ptr == NULL) {
            return 1;
        }
    } else if (vec->len == vec->cap) {
        vec->cap *= 2;
        vec->ptr = (Term*) realloc(vec->ptr, vec->cap * sizeof(Term));
        if (vec->ptr == NULL) {
            return 1;
        }
    }
    *(vec->ptr + vec->len) = term;
    vec->len += 1;
    return 0;
}

// Index this Vec without performing bounds check.
Term Vec_index_unchecked(Vec vec, unsigned int idx) {
    return (vec.ptr)[idx];
}

// Comparation function for sorting by index.
int Term_sort_cmp(const void* a, const void* b) {
    return (((Term*) a)->idx) - (((Term*) b)->idx);
}

// Sort given Vec by index.
void Vec_qsort(Vec vec) {
    qsort(vec.ptr, vec.len, sizeof(Term), Term_sort_cmp);
}

// Drop the given Vec and perform cleanup.
// Given Vec is assumed to be initialized, and will be uninitialized
// after the call.
void Vec_drop(Vec* vec) {
    free(vec->ptr);
}

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
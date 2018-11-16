#ifndef LIST_H
#define LIST_H

#include "term.h"

typedef struct Node {
    Term term;
    struct Node *next;
} Node;

typedef Node* Link;

typedef struct List {
    struct Node *head;
} List;

void List_init(List*);
Node* Node_new(Term);
int Link_insert(Link*, Term);
int List_insert(List*, Term);
void List_free(List*);

#endif
#include "list.h"

#include <stdlib.h>
#include "term.h"

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
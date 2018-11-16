#include "maze.h"

#include <stdlib.h>
#include <string.h>
#include "vec.h"
#include "rc.h"

int visit_cell(Cell *cells, Vec *stack, Element *elem, unsigned int idx);
void idx_to_coordinate(unsigned int idx, unsigned int width, Coordinate *dst);

int visit_cell(Cell *cells, Vec *stack, Element *elem, unsigned int idx) {
    switch (cells[idx]) {
    case 'x':
        return 1;
    case '.':
        cells[idx] = ',';
        Element new_elem;
        new_elem.steps = elem->steps + 1;
        new_elem.rc = Rc_new(Rc_clone(elem->rc), idx);
        if (Vec_push(stack, new_elem) != 0) {
            return 2;
        }
    default:
        return 0;
    }
}

void idx_to_coordinate(unsigned int idx, unsigned int width, Coordinate *dst) {
    dst->x = idx % width;
    dst->y = idx / width;
}

// Find a route from start to exit of this maze.
// Solution will be written to location pointed by solution_out.
// Note that this reports the first solution it finds, possibly not the shortest one.
// Will cause buffer overflow if location pointed by solution_out is smaller than
// needed to store the first route found.
//
// Returns:
// * A positive number on success. This number represents number of coordinates written to solution_out.
// * 0 if input was valid, but no solution was found.
// * -1 if there was an allocation error.
// * -2 if input was invalid.
int Maze_solve(Maze *maze, Coordinate *start, Coordinate *solution_out) {
    if (maze == NULL || start == NULL || solution_out == NULL || maze->cells == NULL) {
        return -2;
    }
    if (start->x >= maze->width || start->y >= maze->height) {
        return -2;
    }
    unsigned int start_idx = start->y * maze->width + start->x;
    if ((maze->cells)[start_idx] == 'x') {
        *solution_out = *start;
        return 1;
    }
    unsigned int maze_size = maze->width * maze->height;
    unsigned int last_idx;
    Vec stack;
    Vec_init(&stack);
    Element elem;
    elem.rc = Rc_new(NULL, start_idx);
    if (elem.rc == NULL) {
        Vec_drop(&stack);
        return -1;
    }
    elem.steps = 1;
    // Create a copy of maze->cells because we want to keep maze->cells as-is,
    // but we want a way to record visited locations, too.
    Cell *cells = (Cell *) malloc(sizeof(Cell) * maze_size);
    if (cells == NULL) {
        Vec_drop(&stack);
        return -1;
    }
    memcpy(cells, maze->cells, sizeof(Cell) * maze_size);
    for (;;) { // Main loop
        unsigned int idx = elem.rc->idx;
        int ret;
        if (idx >= 10) {
            // up
            ret = visit_cell(cells, &stack, &elem, idx - 10);
            if (ret == 1) {
                last_idx = idx - 10;
                break;
            } else if (ret == 2) {
                Vec_drop(&stack);
                Element_drop(&elem);
                free(cells);
                return -1;
            }
        }
        if (idx >= 1) {
            // left
            ret = visit_cell(cells, &stack, &elem, idx - 1);
            if (ret == 1) {
                last_idx = idx - 1;
                break;
            } else if (ret == 2) {
                Vec_drop(&stack);
                Element_drop(&elem);
                free(cells);
                return -1;
            }
        }
        if (idx + 10 <= maze_size) {
            // down
            ret = visit_cell(cells, &stack, &elem, idx + 10);
            if (ret == 1) {
                last_idx = idx + 10;
                break;
            } else if (ret == 2) {
                Vec_drop(&stack);
                Element_drop(&elem);
                free(cells);
                return -1;
            }
        }
        if (idx + 1 <= maze_size) {
            // right
            ret = visit_cell(cells, &stack, &elem, idx + 1);
            if (ret == 1) {
                last_idx = idx + 1;
                break;
            } else if (ret == 2) {
                Vec_drop(&stack);
                Element_drop(&elem);
                free(cells);
                return -1;
            }
        }
        Element_drop(&elem);
        if (Vec_pop(&stack, &elem) == 1) {
            Vec_drop(&stack);
            free(cells);
            return 0;
        }
    }
    unsigned int i = elem.steps;
    int ret = i + 1;
    idx_to_coordinate(last_idx, maze->width, &solution_out[i]);
    Rc node = elem.rc;
    do {
        i -= 1;
        idx_to_coordinate(node->idx, maze->width, &solution_out[i]);
        node = node->pre;
    } while (node != NULL);
    Vec_drop(&stack);
    Element_drop(&elem);
    free(cells);
    return ret;
}
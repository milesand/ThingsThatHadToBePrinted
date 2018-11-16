#ifndef MAZE_H
#define MAZE_H

// '#' denotes wall, '.' unvisited cell, 'x' exit.
// ',' will be visited cell, for memo-purpose.
typedef char Cell;

typedef struct {
    unsigned int x;
    unsigned int y;
} Coordinate;

typedef struct {
    unsigned int width;
    unsigned int height;
    Cell *cells;
} Maze;

int Maze_solve(Maze *maze, Coordinate *start, Coordinate *solution_out);

#endif
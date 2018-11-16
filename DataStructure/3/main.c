#include <stdio.h>
#include "maze.h"

int main() {
    Maze maze;
    maze.width = 10;
    maze.height = 10;
    maze.cells =
        "##########"
        "....#....#"
        "#...#....#"
        "#.###..#.#"
        "#...#..#.#"
        "#.#.#..#.#"
        "#.#.#..#.#"
        "#.#.#..#.#"
        "#.#....#.x"
        "##########";
    Coordinate solution[47];
    Coordinate start;
    start.x = 0;
    start.y = 1;
    int ret = Maze_solve(&maze, &start, solution);
    if (ret == 0) {
        puts("No solution found");
        return 1;
    } else if (ret == -1) {
        puts("Allocation Error");
        return 1;
    } else if (ret == -2) {
        puts("Invalid input");
        return 1;
    }
    for (int i = 0; i < ret; i += 1) {
        Coordinate coord = solution[i];
        printf("%d, %d\n", coord.x, coord.y);
    }
    return 0;
}
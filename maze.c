#include "maze.h"
#include <stdlib.h>
#include <time.h>

int maze[MAP_HEIGHT][MAP_WIDTH];

void initMaze() {
    // Initialize borders as walls
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            maze[y][x] = (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) ? 1 : 0;
        }
    }
}

void generateMaze() {
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Generate random walls and paths in the larger maze
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            // Set maze borders to walls
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) {
                maze[y][x] = 1;
            } else {
                maze[y][x] = rand() % 2;  // Randomly generate walls (1) and paths (0)
            }
        }
    }

    // Ensure starting point is open
    maze[1][1] = 0;
}
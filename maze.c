#include "maze.h"
#include <stdlib.h>
#include <time.h>

int maze[MAP_HEIGHT][MAP_WIDTH];

// Directions for moving in the grid (up, right, down, left)
int directions[4][2] = {
    { -1, 0 },  // Up
    { 1, 0 },   // Down
    { 0, -1 },  // Left
    { 0, 1 }    // Right
};

// Function to shuffle directions (used for random movement in the maze)
void shuffleDirections() {
    for (int i = 0; i < 4; i++) {
        int randomIndex = rand() % 4;
        int tempX = directions[i][0];
        int tempY = directions[i][1];
        directions[i][0] = directions[randomIndex][0];
        directions[i][1] = directions[randomIndex][1];
        directions[randomIndex][0] = tempX;
        directions[randomIndex][1] = tempY;
    }
}

// Recursive function to carve out the maze
void carveMaze(int x, int y) {
    shuffleDirections();  // Shuffle directions for randomization

    // Try each direction
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int newX = x + dx * 2;
        int newY = y + dy * 2;

        // Check if the new cell is within bounds and not visited
        if (newX > 0 && newX < MAP_WIDTH - 1 && newY > 0 && newY < MAP_HEIGHT - 1 && maze[newY][newX] == 1) {
            // Remove the wall between the current cell and the new cell
            maze[y + dy][x + dx] = 0;
            maze[newY][newX] = 0;

            // Recursively carve from the new cell
            carveMaze(newX, newY);
        }
    }
}

// Initialize maze with all walls and set up the borders
void initMaze() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            maze[y][x] = 1;  // Set all cells as walls
        }
    }

    // Set borders as walls
    for (int x = 0; x < MAP_WIDTH; x++) {
        maze[0][x] = 1;
        maze[MAP_HEIGHT - 1][x] = 1;
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        maze[y][0] = 1;
        maze[y][MAP_WIDTH - 1] = 1;
    }
}

// Function to generate a maze using recursive backtracking
void generateMaze() {
    srand((unsigned int)time(NULL));  // Seed the random number generator

    // Initialize maze with walls and borders
    initMaze();

    // Start carving from a random position inside the maze (must be odd coordinates)
    maze[1][1] = 0;  // Starting point
    carveMaze(1, 1); // Begin carving the maze

    // Ensure the start and end points are open
    maze[1][1] = 0;  // Start point
    maze[MAP_HEIGHT - 2][MAP_WIDTH - 2] = 0;  // End point
}

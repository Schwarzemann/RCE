#include <GL/gl.h>
#include <math.h>
#include "rce_creature.h"
#include "rce_maze.h"

#define TILE_SIZE 1.0f // Define TILE_SIZE if not defined elsewhere

float creatureX = 5.5f;
float creatureY = 5.5f;
float creatureSpeed = 0.005f;

void initCreature() {
    creatureX = 5.5f;
    creatureY = 5.5f;
}

int isWalkable(int x, int y) {
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && maze[y][x] == 0);
}

void updateCreaturePosition(float playerX, float playerY) {
    int creatureMapX = (int)creatureX;
    int creatureMapY = (int)creatureY;
    int playerMapX = (int)playerX;
    int playerMapY = (int)playerY;

    float minDist = INFINITY;
    int bestX = creatureMapX;
    int bestY = creatureMapY;

    // Check four neighboring cells
    int neighbors[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    for (int i = 0; i < 4; i++) {
        int nx = creatureMapX + neighbors[i][0];
        int ny = creatureMapY + neighbors[i][1];
        if (isWalkable(nx, ny)) {
            float dist = sqrtf((nx - playerMapX) * (nx - playerMapX) + (ny - playerMapY) * (ny - playerMapY));
            if (dist < minDist) {
                minDist = dist;
                bestX = nx;
                bestY = ny;
            }
        }
    }

    // Move creature towards the best cell found
    if (bestX != creatureMapX || bestY != creatureMapY) {
        creatureX += (bestX - creatureMapX) * creatureSpeed;
        creatureY += (bestY - creatureMapY) * creatureSpeed;
    }
}

void drawCreature() {
    glPushMatrix();
    glTranslatef(creatureX * TILE_SIZE, creatureY * TILE_SIZE, 0); // Adjust TILE_SIZE as needed
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the creature
    glBegin(GL_QUADS);
        glVertex2f(-0.25f, -0.25f);
        glVertex2f( 0.25f, -0.25f);
        glVertex2f( 0.25f,  0.25f);
        glVertex2f(-0.25f,  0.25f);
    glEnd();
    glPopMatrix();
}

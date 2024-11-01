#include <GL/glew.h>
#include <GL/gl.h>
#include <math.h>
#include "rce_creature.h"
#include "rce_maze.h"
#include "rce_player.h"

#define TILE_SIZE 2.0f // Temporarily increased to make the creature more visible

float creatureX;
float creatureY;
float creatureSpeed = 0.005f;

void initCreature() {
    creatureX = playerX + 1.0f;  // Start near the player
    creatureY = playerY + 1.0f;
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
    glTranslatef(creatureX * TILE_SIZE, creatureY * TILE_SIZE, 0); // Adjusted to make the creature appear in the maze
    glColor3f(1.0f, 0.0f, 0.0f); // Set to bright red for visibility
    glBegin(GL_QUADS);
        glVertex2f(-0.25f, -0.25f);
        glVertex2f( 0.25f, -0.25f);
        glVertex2f( 0.25f,  0.25f);
        glVertex2f(-0.25f,  0.25f);
    glEnd();
    glPopMatrix();
}

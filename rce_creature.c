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
    creatureX = 3.5f;  // Start at a fixed walkable spot farther from player
    creatureY = 3.5f;
    while (!isWalkable((int)creatureX, (int)creatureY)) {
        creatureX += 1.0f;  // Move right until walkable
        if (creatureX >= MAP_WIDTH - 1) {
            creatureX = 1.5f;
            creatureY += 1.0f;  // Move down if we hit the edge
        }
    }
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

    if (bestX != creatureMapX || bestY != creatureMapY) {
        creatureX += (bestX - creatureMapX) * creatureSpeed;
        creatureY += (bestY - creatureMapY) * creatureSpeed;
    }

    printf("Creature Position: X: %.2f, Y: %.2f\n", creatureX, creatureY); // Debug print
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

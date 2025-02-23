#include <GL/glew.h>
#include <GL/gl.h>
#include <math.h>
#include "rce_creature.h"
#include "rce_maze.h"
#include "rce_player.h"
#include "rce_graphics.h"

#define TILE_SIZE 2.0f // Temporarily increased to make the creature more visible

float creatureX;
float creatureY;
float creatureSpeed = 0.01f;
float rotationAngle = 0.0f; // Moved to global scope

void initCreature() {
    creatureX = 3.5f;  // Start at a walkable spot
    creatureY = 3.5f;
    while (!isWalkable((int)creatureX, (int)creatureY)) {
        creatureX += 1.0f;
        if (creatureX >= MAP_WIDTH - 1) {
            creatureX = 1.5f;
            creatureY += 1.0f;
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

    // Update rotation angle for spinning
    rotationAngle += 0.1f; // Adjust speed of spin (degrees per frame)
    if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
}

void drawCreature() {
    float dx = creatureX - playerX;
    float dy = creatureY - playerY;
    float distToCreature = sqrtf(dx * dx + dy * dy);

    if (distToCreature < 16.0f) {
        int fov = 60;
        float angleStep = fov / (float)screenWidth;
        float creatureAngle = atan2f(dy, dx) * 180.0f / M_PI;
        float relativeAngle = creatureAngle - playerAngle;
        if (relativeAngle < -180.0f) relativeAngle += 360.0f;
        if (relativeAngle > 180.0f) relativeAngle -= 360.0f;

        if (fabsf(relativeAngle) < fov / 2.0f) {
            float screenX = screenWidth / 2.0f + (relativeAngle / angleStep);
            float creatureHeight = screenHeight / (distToCreature + 0.1f);

            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glPushMatrix();
            glLoadIdentity();
            glTranslatef(screenX, screenHeight / 2.0f, -distToCreature); // Use distance for depth
            glScalef(creatureHeight / 2.0f, creatureHeight / 2.0f, creatureHeight / 2.0f);
            glRotatef(rotationAngle, 1.0f, 1.0f, 1.0f);

            glBegin(GL_QUADS);
            // Front face
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, 0.5f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, 0.5f);
            glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-0.5f,  0.5f, 0.5f);

            // Back face
            glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glColor3f(0.0f, 1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
            glColor3f(1.0f, 1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
            glColor3f(0.5f, 0.0f, 0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);

            // Left face
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glColor3f(0.5f, 0.0f, 0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);

            // Right face
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.5f, -0.5f,  0.5f);
            glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
            glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(0.5f,  0.5f, -0.5f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.5f,  0.5f,  0.5f);

            // Top face
            glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.5f,  0.5f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 0.5f, 0.5f,  0.5f);
            glColor3f(1.0f, 1.0f, 1.0f); glVertex3f( 0.5f, 0.5f, -0.5f);
            glColor3f(0.5f, 0.0f, 0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);

            // Bottom face
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
            glColor3f(0.0f, 1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
            glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glEnd();

            glPopMatrix();
            glEnable(GL_TEXTURE_2D);
        }
    }
}
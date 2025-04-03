#include <GL/glew.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include "rce_creature.h"
#include "rce_maze.h"
#include "rce_player.h"
#include "rce_graphics.h"

#define TILE_SIZE 2.0f // Temporarily increased to make the creature more visible

float creatureX;
float creatureY;
float creatureSpeed = 0.0009f;
float rotationAngle = 0.0f; // Moved to global scope
float rotationSpeed = 2.0f;

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

int hasLineOfSight(float startX, float startY, float endX, float endY) {
    float dx = endX - startX;
    float dy = endY - startY;
    float distance = sqrtf(dx * dx + dy * dy);
    float stepX = dx / distance;
    float stepY = dy / distance;

    float currentX = startX;
    float currentY = startY;
    float traveled = 0.0f;

    while (traveled < distance) {
        int mapX = (int)currentX;
        int mapY = (int)currentY;
        if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT || maze[mapY][mapX] == 1) {
            return 0; // Hit a wall
        }
        currentX += stepX * 0.1f;
        currentY += stepY * 0.1f;
        traveled += 0.1f;
    }
    return 1; // Clear path
}

void updateCreaturePosition(float playerX, float playerY) {
    // Calculate direction to player
    float dx = playerX - creatureX;
    float dy = playerY - creatureY;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > 0.1f) { // Only move if not too close
        // Normalize direction
        float dirX = dx / distance;
        float dirY = dy / distance;

        // Calculate next position
        float nextX = creatureX + dirX * creatureSpeed;
        float nextY = creatureY + dirY * creatureSpeed;

        // Collision check: Only move if next position is walkable
        int nextMapX = (int)nextX;
        int nextMapY = (int)nextY;
        if (isWalkable(nextMapX, nextMapY)) {
            creatureX = nextX;
            creatureY = nextY;
        } else {
            // Try sliding along X or Y if direct path is blocked
            nextX = creatureX + dirX * creatureSpeed;
            nextY = creatureY; // Only X movement
            nextMapX = (int)nextX;
            nextMapY = (int)nextY;
            if (isWalkable(nextMapX, nextMapY)) {
                creatureX = nextX;
            } else {
                nextX = creatureX;
                nextY = creatureY + dirY * creatureSpeed; // Only Y movement
                nextMapX = (int)nextX;
                nextMapY = (int)nextY;
                if (isWalkable(nextMapX, nextMapY)) {
                    creatureY = nextY;
                }
            }
        }
    }

    rotationAngle += rotationSpeed;
    if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
}

void logCreatureStatus(float playerX, float playerY) {
    float dx = playerX - creatureX;
    float dy = playerY - creatureY;
    float distance = sqrtf(dx * dx + dy * dy);
    int visible = hasLineOfSight(playerX, playerY, creatureX, creatureY);
    printf("Creature Position: X: %.2f, Y: %.2f | Distance to Player: %.2f | Visible: %s\n",
           creatureX, creatureY, distance, visible ? "Yes" : "No");
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
            if (hasLineOfSight(playerX, playerY, creatureX, creatureY)) {
                float screenX = screenWidth / 2.0f + (relativeAngle / angleStep);
                float creatureHeight = screenHeight / (distToCreature + 0.1f);
                float halfHeight = creatureHeight / 2.0f;

                glPushMatrix();
                glLoadIdentity();
                glTranslatef(screenX, screenHeight / 2.0f, 0.0f);
                glScalef(halfHeight, halfHeight, 1.0f);

                glDisable(GL_TEXTURE_2D);
                glBegin(GL_QUADS);

                float size = 1.0f;
                float angleRad = rotationAngle * M_PI / 180.0f;
                float cosA = cosf(angleRad);
                float sinA = sinf(angleRad);

                // Define the cube 8 vertices
                float vertices[8][3] = {
                    {-size, -size, -size}, // 0: Bottom-left-back
                    { size, -size, -size}, // 1: Bottom-right-back
                    { size,  size, -size}, // 2: Top-right-back
                    {-size,  size, -size}, // 3: Top-left-back
                    {-size, -size,  size}, // 4: Bottom-left-front
                    { size, -size,  size}, // 5: Bottom-right-front
                    { size,  size,  size}, // 6: Top-right-front
                    {-size,  size,  size}  // 7: Top-left-front
                };

                // Rotate vertices around the y-axis
                for (int i = 0; i < 8; i++) {
                    float x = vertices[i][0];
                    float z = vertices[i][2];
                    vertices[i][0] = x * cosA - z * sinA;
                    vertices[i][2] = x * sinA + z * cosA;
                }

                // Perspective shit
                float projected[8][2];
                for (int i = 0; i < 8; i++) {
                    float zFactor = 0.5f / (distToCreature + vertices[i][2] + 1.0f); // Shit code
                    projected[i][0] = vertices[i][0] * zFactor;
                    projected[i][1] = vertices[i][1] * zFactor;
                }

                // Draw the cube faces with basic depth sorting
                // Back face (0-1-2-3)
                glColor3f(0.0f, 1.0f, 0.0f); // Green
                glVertex2f(projected[0][0], projected[0][1]);
                glVertex2f(projected[1][0], projected[1][1]);
                glVertex2f(projected[2][0], projected[2][1]);
                glVertex2f(projected[3][0], projected[3][1]);

                // Left face (0-3-7-4)
                if (vertices[0][0] < 0) {
                    glColor3f(0.0f, 0.0f, 1.0f); // Blue
                    glVertex2f(projected[0][0], projected[0][1]);
                    glVertex2f(projected[3][0], projected[3][1]);
                    glVertex2f(projected[7][0], projected[7][1]);
                    glVertex2f(projected[4][0], projected[4][1]);
                }

                // Right face (1-5-6-2)
                if (vertices[1][0] > 0) {
                    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
                    glVertex2f(projected[1][0], projected[1][1]);
                    glVertex2f(projected[5][0], projected[5][1]);
                    glVertex2f(projected[6][0], projected[6][1]);
                    glVertex2f(projected[2][0], projected[2][1]);
                }

                // Bottom face (0-1-5-4)
                if (vertices[0][1] < 0) {
                    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
                    glVertex2f(projected[0][0], projected[0][1]);
                    glVertex2f(projected[1][0], projected[1][1]);
                    glVertex2f(projected[5][0], projected[5][1]);
                    glVertex2f(projected[4][0], projected[4][1]);
                }

                // Top face (3-2-6-7)
                if (vertices[3][1] > 0) {
                    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
                    glVertex2f(projected[3][0], projected[3][1]);
                    glVertex2f(projected[2][0], projected[2][1]);
                    glVertex2f(projected[6][0], projected[6][1]);
                    glVertex2f(projected[7][0], projected[7][1]);
                }

                // Front face (4-5-6-7) - drawn last
                glColor3f(1.0f, 0.0f, 0.0f); // Red
                glVertex2f(projected[4][0], projected[4][1]);
                glVertex2f(projected[5][0], projected[5][1]);
                glVertex2f(projected[6][0], projected[6][1]);
                glVertex2f(projected[7][0], projected[7][1]);

                glEnd();

                glColor3f(1.0f, 1.0f, 1.0f); // Reset color to white
                glEnable(GL_TEXTURE_2D); // Re-enable textures

                glPopMatrix();
            }
        }
    }
}
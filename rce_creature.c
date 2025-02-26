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

    rotationAngle += 0.5f;
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

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, creatureTextureID);
                glPushMatrix();
                glLoadIdentity();
                glTranslatef(screenX, screenHeight / 2.0f, 0.0f);
                glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
                glScalef(halfHeight, halfHeight, 1.0f);

                glColor3f(1.0f, 1.0f, 1.0f); // White to preserve texture colors
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f); // Bottom-left
                glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f); // Bottom-right
                glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f); // Top-right
                glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f); // Top-left
                glEnd();

                glPopMatrix();
            }
        }
    }
}
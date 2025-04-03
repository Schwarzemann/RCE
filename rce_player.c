#include "rce_player.h"

float playerX = 1.5f;
float playerY = 1.5f;
float playerAngle = 0.0f;
float moveSpeed = 0.09f;
float rotSpeed = 0.55f;

void logPlayerStatus() {
    printf("Player Position: X: %.2f, Y: %.2f\nPlayer Angle: %.2f\n", playerX, playerY, playerAngle);
}

#include "input.h"
#include "maze.h"
#include "player.h"

void handleInput(GLFWwindow *window) {
    float moveDirX = 0.0f;
    float moveDirY = 0.0f;

    // Convert player angle to radians
    float playerAngleRad = DEG_TO_RAD(playerAngle);
    float cosAngle = cosf(playerAngleRad);
    float sinAngle = sinf(playerAngleRad);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        moveDirX += cosAngle;
        moveDirY += sinAngle;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        moveDirX -= cosAngle;
        moveDirY -= sinAngle;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        playerAngle -= rotSpeed * 10.0f / M_PI; // Convert to degrees for consistency
        if (playerAngle < 0) playerAngle += 360.0f; // Keep angle between 0 and 360
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        playerAngle += rotSpeed * 10.0f / M_PI; // Convert to degrees for consistency
        if (playerAngle >= 360.0f) playerAngle -= 360.0f; // Keep angle between 0 and 360
    }

    // Normalize movement direction and apply movement speed
    float magnitude = sqrtf(moveDirX * moveDirX + moveDirY * moveDirY);
    if (magnitude > 0.0f) {
        moveDirX = (moveDirX / magnitude) * moveSpeed;
        moveDirY = (moveDirY / magnitude) * moveSpeed;
    }

    // Calculate potential new position
    float newPlayerX = playerX + moveDirX;
    float newPlayerY = playerY + moveDirY;

    // Collision detection
    int mapX = (int)newPlayerX;
    int mapY = (int)newPlayerY;

    // Check if the new position is not a wall
    if (maze[(int)playerY][mapX] == 0) {
        playerX = newPlayerX;
    }
    if (maze[mapY][(int)playerX] == 0) {
        playerY = newPlayerY;
    }
}

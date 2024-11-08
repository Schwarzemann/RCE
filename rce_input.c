#include "rce_input.h"
#include "rce_maze.h"
#include "rce_player.h"

void processPlayerMovement(float moveDirX, float moveDirY) {
    float newPlayerX = playerX + moveDirX * moveSpeed;
    float newPlayerY = playerY + moveDirY * moveSpeed;

    int mapX = (int)newPlayerX;
    int mapY = (int)newPlayerY;

    if (maze[(int)playerY][mapX] == 0) {
        playerX = newPlayerX;
    }
    if (maze[mapY][(int)playerX] == 0) {
        playerY = newPlayerY;
    }
}

void processPlayerRotation(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerAngle -= rotSpeed * 5.0f;;
        if (playerAngle < 0) playerAngle += 360.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerAngle += rotSpeed * 5.0f;
        if (playerAngle >= 360.0f) playerAngle -= 360.0f;
    }
}

void handleInput(GLFWwindow *window) {
    float moveDirX = 0.0f;
    float moveDirY = 0.0f;
    float playerAngleRad = DEG_TO_RAD(playerAngle);
    float cosAngle = cosf(playerAngleRad);
    float sinAngle = sinf(playerAngleRad);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        moveDirX += cosAngle;
        moveDirY += sinAngle;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        moveDirX -= cosAngle;
        moveDirY -= sinAngle;
    }

    processPlayerMovement(moveDirX, moveDirY);
    processPlayerRotation(window);
}
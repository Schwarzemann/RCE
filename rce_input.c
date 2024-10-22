#include "rce_input.h"
#include "rce_maze.h"
#include "rce_player.h"

void handleInput(GLFWwindow *window) {
    float moveDirX = 0.0f;
    float moveDirY = 0.0f;

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
        playerAngle -= rotSpeed * 10.0f / M_PI;
        if (playerAngle < 0) playerAngle += 360.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        playerAngle += rotSpeed * 10.0f / M_PI;
        if (playerAngle >= 360.0f) playerAngle -= 360.0f;
    }

    // Normalize movement direction
    float magnitude = sqrtf(moveDirX * moveDirX + moveDirY * moveDirY);
    if (magnitude > 0.0f) {
        moveDirX = (moveDirX / magnitude) * moveSpeed;
        moveDirY = (moveDirY / magnitude) * moveSpeed;
    }

    float newPlayerX = playerX + moveDirX;
    float newPlayerY = playerY + moveDirY;

    int mapX = (int)newPlayerX;
    int mapY = (int)newPlayerY;

    if (maze[(int)playerY][mapX] == 0) {
        playerX = newPlayerX;
    }
    if (maze[mapY][(int)playerX] == 0) {
        playerY = newPlayerY;
    }

    // Press 'R' to regenerate the maze
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        generateMaze();
    }
}
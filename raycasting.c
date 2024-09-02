#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAP_WIDTH  10
#define MAP_HEIGHT 10
#define M_PI  3.1415926535897932384626433

int maze[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,0,1,1,1,0,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

float playerX = 1.5f;
float playerY = 1.5f;
float playerAngle = 0.0f;
float moveSpeed = 0.01f;
float rotSpeed = 0.05f;
int screenWidth = 800;
int screenHeight = 600;

void initOpenGL(GLFWwindow **window) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    *window = glfwCreateWindow(screenWidth, screenHeight, "RCE", NULL, NULL);
    if (!*window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(*window);
    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Draw the maze using raycasting
void drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    int fov = 60; // Field of view
    float angleStep = fov / (float)screenWidth;
    float angleOffset = (fov / 2.0f) - angleStep;

    glBegin(GL_QUADS);

    for (int x = 0; x < screenWidth; x++) {
        float rayAngle = playerAngle - angleOffset + angleStep * x;
        float rayX = cosf(rayAngle * M_PI / 180.0f);
        float rayY = sinf(rayAngle * M_PI / 180.0f);

        float distance = 0.0f;
        while (distance < 16.0f) { // Check up to 16 units away
            int mapX = (int)(playerX + rayX * distance);
            int mapY = (int)(playerY + rayY * distance);

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) break;

            if (maze[mapY][mapX] == 1) {
                // Render wall slice
                float wallHeight = screenHeight / (distance + 0.1f);
                float wallTop = screenHeight / 2 - wallHeight / 2;
                float wallBottom = wallTop + wallHeight;
                
                glColor3f(1.0f, 0.0f, 0.0f); // Red color for walls
                glVertex2f(x, wallTop);
                glVertex2f(x + 1, wallTop);
                glVertex2f(x + 1, wallBottom);
                glVertex2f(x, wallBottom);
                break;
            }

            distance += 0.1f; // Increment distance
        }
    }

    glEnd();
}

void handleInput(GLFWwindow *window) {
    float moveDirX = 0.0f;
    float moveDirY = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        moveDirX += cosf(playerAngle);
        moveDirY += sinf(playerAngle);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        moveDirX -= cosf(playerAngle);
        moveDirY -= sinf(playerAngle);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        playerAngle -= rotSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        playerAngle += rotSpeed;
    }

    // Normalize movement direction and apply movement speed
    float magnitude = sqrtf(moveDirX * moveDirX + moveDirY * moveDirY);
    if (magnitude > 0.0f) {
        moveDirX /= magnitude;
        moveDirY /= magnitude;
    }

    playerX += moveDirX * moveSpeed;
    playerY += moveDirY * moveSpeed;
}

// Log player position and angle
void logPlayerStatus() {
    printf("Player Position: X: %.2f, Y: %.2f\nPlayer Angle: %.2f", playerX, playerY, playerAngle);
}

int main() {
    GLFWwindow *window;
    initOpenGL(&window);

    while (!glfwWindowShouldClose(window)) {
        handleInput(window);

        drawMaze();

        logPlayerStatus(); // Log the player’s status every frame

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
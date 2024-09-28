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
GLuint textureID;

void generateBrickTexture() {
    const int texSize = 64;
    unsigned char texture[texSize][texSize][3];

    for (int y = 0; y < texSize; y++) {
        for (int x = 0; x < texSize; x++) {
            int brick = ((x / 8) % 2 == (y / 8) % 2); // Checkerboard pattern
            texture[y][x][0] = brick ? 139 : 205; // R
            texture[y][x][1] = brick ? 69 : 133;  // G
            texture[y][x][2] = brick ? 19 : 63;   // B
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void initOpenGL(GLFWwindow **window) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    *window = glfwCreateWindow(screenWidth, screenHeight, "Raycasted Cube with Texture", NULL, NULL);
    if (!*window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(*window);
    glewInit();
    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    generateBrickTexture();
}

// Draw the maze using raycasting with textured walls
void drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT);

    int fov = 60; // Field of view
    float angleStep = fov / (float)screenWidth;
    float angleOffset = (fov / 2.0f) - angleStep;

    glBindTexture(GL_TEXTURE_2D, textureID);
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
                // Render wall slice with texture mapping
                float wallHeight = screenHeight / (distance + 0.1f);
                float wallTop = screenHeight / 2 - wallHeight / 2;
                float wallBottom = wallTop + wallHeight;

                // Texture coordinates
                float textureX = (playerX + rayX * distance) - mapX;
                glTexCoord2f(textureX, 0.0f); glVertex2f(x, wallTop);
                glTexCoord2f(textureX + 0.1f, 0.0f); glVertex2f(x + 1, wallTop);
                glTexCoord2f(textureX + 0.1f, 1.0f); glVertex2f(x + 1, wallBottom);
                glTexCoord2f(textureX, 1.0f); glVertex2f(x, wallBottom);
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
    printf("Player Position: X: %.2f, Y: %.2f\nPlayer Angle: %.2f\n", playerX, playerY, playerAngle);
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

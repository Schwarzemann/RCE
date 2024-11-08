#include "rce_graphics.h"
#include "rce_maze.h"
#include "rce_player.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int screenWidth = 800;
int screenHeight = 600;
GLuint textureID;


void generateBrickTexture() {
    int width, height, nrChannels;
    unsigned char *data = stbi_load("brick_texture.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the texture into OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);  // Free the image memory after loading the texture
    } else {
        printf("Failed to load texture\n");
    }
}

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
    
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(glewStatus));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    generateBrickTexture();
}

void drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT);

    int fov = 60; // Field of view
    float angleStep = fov / (float)screenWidth;
    float angleOffset = (fov / 2.0f) - angleStep;
    float playerVerticalAngle = 0.0f;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);

    for (int x = 0; x < screenWidth; x++) {
        float rayAngle = playerAngle - angleOffset + angleStep * x;
        float rayX = cosf(rayAngle * M_PI / 180.0f);
        float rayY = sinf(rayAngle * M_PI / 180.0f);

        float distance = 0.0f;
        float hitX, hitY;
        int side; // To track whether the hit was vertical or horizontal

        while (distance < 16.0f) { // Check up to 16 units away
            int mapX = (int)(playerX + rayX * distance);
            int mapY = (int)(playerY + rayY * distance);

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) break;

            if (maze[mapY][mapX] == 1) {
                // Calculate exact hit point
                if (fabsf(rayX) > fabsf(rayY)) {
                    // Horizontal hit
                    hitX = playerX + rayX * distance;
                    hitY = playerY + rayY * distance;
                    side = 0;  // Horizontal hit
                } else {
                    // Vertical hit
                    hitX = playerX + rayX * distance;
                    hitY = playerY + rayY * distance;
                    side = 1;  // Vertical hit
                }

                // Calculate texture coordinate based on the side hit
                float textureX;
                if (side == 0) {
                    textureX = hitY - floorf(hitY);  // Horizontal, use Y-coordinate
                } else {
                    textureX = hitX - floorf(hitX);  // Vertical, use X-coordinate
                }

                // Calculate wall height and apply playerVerticalAngle adjustment
                float wallHeight = screenHeight / (distance + 0.1f);
                float wallTop = (screenHeight / 2 - wallHeight / 2) + playerVerticalAngle * 5.0f;
                float wallBottom = wallTop + wallHeight;

                // Texture coordinates mapped continuously across wall slices
                glTexCoord2f(textureX, 0.0f); glVertex2f(x, wallTop);
                glTexCoord2f(textureX, 0.0f); glVertex2f(x + 1, wallTop);
                glTexCoord2f(textureX, 1.0f); glVertex2f(x + 1, wallBottom);
                glTexCoord2f(textureX, 1.0f); glVertex2f(x, wallBottom);

                break; // Stop at the first wall hit
            }

            distance += 0.1f; // Increment distance
        }
    }

    glEnd();
}

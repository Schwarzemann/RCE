#include "rce_graphics.h"
#include "rce_maze.h"
#include "rce_player.h"
#include "rce_creature.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int screenWidth = 800;
int screenHeight = 600;
GLuint textureID;  // Global texture ID
GLuint creatureTextureID;
GLuint groundTextureID;
GLuint skyTextureID;

GLuint loadTexture(const char *filename) {
    GLuint textureID;
    int width, height, nrChannels;

    // Load the image using stb_image
    stbi_set_flip_vertically_on_load(1);  // Flip the image vertically for OpenGL compatibility
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) {
        // Generate texture ID
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the texture data into OpenGL
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            printf("Warning: Texture format not supported. The texture will not be loaded.\n");
            stbi_image_free(data);
            return 0;  // Return 0 to indicate an error
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        // If the texture fails to load, print an error message
        printf("Error: Failed to load texture from %s\n", filename);
        return 0;  // Return 0 to indicate an error
    }

    return textureID;
}

// Function to load a fallback texture in case of an error
GLuint loadFallbackTexture() {
    unsigned char data[4] = {255, 0, 0, 255};  // Red color as fallback
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return textureID;
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
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1); // Original 2D setup
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending

    textureID = loadTexture("sprites/brick_texture.jpg");
    if (textureID == 0) {
        printf("Failed to load the texture. Using fallback texture.\n");
        textureID = loadFallbackTexture();
    }

    creatureTextureID = loadTexture("sprites/lost_soul.png");
    if (creatureTextureID == 0) {
        printf("Failed to load the creature texture. Using fallback texture.\n");
        creatureTextureID = loadFallbackTexture();
    }

    groundTextureID = loadTexture("sprites/ground_sprite.png");
    if (groundTextureID == 0) {
        printf("Failed to load the ground texture. Using fallback texture.\n");
        groundTextureID = loadFallbackTexture();
    }

    skyTextureID = loadTexture("sprites/SKY9.png");
    if (skyTextureID == 0) {
        printf("Failed to load the sky texture. Using fallback texture.\n");
        skyTextureID = loadFallbackTexture();
    }
}

void drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT);

    int fov = 60;
    float angleStep = fov / (float)screenWidth;
    float angleOffset = (fov / 2.0f) - angleStep;
    float playerVerticalAngle = 0.0f;
    float horizon = screenHeight / 2.0f + playerVerticalAngle * 5.0f;

    // Draw the skybox
    glBindTexture(GL_TEXTURE_2D, skyTextureID);
    glBegin(GL_QUADS);

    // Calculate texture coordinates based on player angle
    float texXLeft = (playerAngle - fov / 2.0f) / 360.0f; // Map angle to texture coordinate (0 to 1)
    float texXRight = (playerAngle + fov / 2.0f) / 360.0f;

    glTexCoord2f(texXLeft, 1.0f); glVertex2f(0, 0); // Top-left
    glTexCoord2f(texXRight, 1.0f); glVertex2f(screenWidth, 0); // Top-right
    glTexCoord2f(texXRight, 0.0f); glVertex2f(screenWidth, horizon); // Bottom-right
    glTexCoord2f(texXLeft, 0.0f); glVertex2f(0, horizon); // Bottom-left

    glEnd();

    // Draw the ground
    glBindTexture(GL_TEXTURE_2D, groundTextureID);
    glBegin(GL_QUADS);
    for (int x = 0; x < screenWidth; x++) {
        float rayAngle = playerAngle - angleOffset + angleStep * x;
        float rayX = cosf(rayAngle * M_PI / 180.0f);
        float rayY = sinf(rayAngle * M_PI / 180.0f);

        // Calculate ground texture coordinates based on distance
        float distance = 0.1f; // Start close to the player
        float maxDistance = 16.0f;
        float horizon = screenHeight / 2.0f + playerVerticalAngle * 5.0f;
        float bottom = screenHeight;

        for (int y = (int)horizon; y < bottom; y++) {
            float perspective = (float)(y - horizon) / (bottom - horizon);
            distance = 0.1f + perspective * maxDistance;
            float groundX = playerX + rayX * distance;
            float groundY = playerY + rayY * distance;

            // Texture coordinates based on ground position
            float texX = groundX - floorf(groundX);
            float texY = groundY - floorf(groundY);

            glTexCoord2f(texX, texY);
            glVertex2f(x, y);
            glTexCoord2f(texX, texY);
            glVertex2f(x + 1, y);
            glTexCoord2f(texX, texY);
            glVertex2f(x + 1, y + 1);
            glTexCoord2f(texX, texY);
            glVertex2f(x, y + 1);
        }
    }
    glEnd();

    // Draw the walls
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);

    for (int x = 0; x < screenWidth; x++) {
        float rayAngle = playerAngle - angleOffset + angleStep * x;
        float rayX = cosf(rayAngle * M_PI / 180.0f);
        float rayY = sinf(rayAngle * M_PI / 180.0f);

        float distance = 0.0f;
        float hitX, hitY;
        int side;

        while (distance < 16.0f) {
            int mapX = (int)(playerX + rayX * distance);
            int mapY = (int)(playerY + rayY * distance);

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) break;

            if (maze[mapY][mapX] == 1) {
                if (fabsf(rayX) > fabsf(rayY)) {
                    hitX = playerX + rayX * distance;
                    hitY = playerY + rayY * distance;
                    side = 0;
                } else {
                    hitX = playerX + rayX * distance;
                    hitY = playerY + rayY * distance;
                    side = 1;
                }

                float textureX = (side == 0) ? (hitY - floorf(hitY)) : (hitX - floorf(hitX));
                float wallHeight = screenHeight / (distance + 0.1f);
                float wallTop = (screenHeight / 2 - wallHeight / 2) + playerVerticalAngle * 5.0f;
                float wallBottom = wallTop + wallHeight;

                glTexCoord2f(textureX, 0.0f); glVertex2f(x, wallTop);
                glTexCoord2f(textureX, 0.0f); glVertex2f(x + 1, wallTop);
                glTexCoord2f(textureX, 1.0f); glVertex2f(x + 1, wallBottom);
                glTexCoord2f(textureX, 1.0f); glVertex2f(x, wallBottom);

                break;
            }
            distance += 0.1f;
        }
    }
    glEnd();
}

void drawTopDownView(GLFWwindow *topDownWindow) {
    glfwMakeContextCurrent(topDownWindow);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up orthographic projection for 2D view (match maze dimensions)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, MAP_WIDTH, MAP_HEIGHT, 0, -1, 1); // Top-down: (0,0) at top-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Disable textures for simple colored drawing
    glDisable(GL_TEXTURE_2D);

    // Draw maze (walls as white, floors as black)
    glBegin(GL_QUADS);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (maze[y][x] == 1) {
                glColor3f(1.0f, 1.0f, 1.0f); // White walls
            } else {
                glColor3f(0.0f, 0.0f, 0.0f); // Black floors
            }
            glVertex2f(x, y);
            glVertex2f(x + 1, y);
            glVertex2f(x + 1, y + 1);
            glVertex2f(x, y + 1);
        }
    }
    glEnd();

    // Draw player as a green circle
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(playerX, playerY); // Center
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * M_PI * i / 20.0f;
        glVertex2f(playerX + 0.2f * cosf(angle), playerY + 0.2f * sinf(angle)); // Radius 0.2
    }
    glEnd();

    // Draw player camera direction as a line
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glBegin(GL_LINES);
    glVertex2f(playerX, playerY);
    float dirX = cosf(playerAngle * M_PI / 180.0f);
    float dirY = sinf(playerAngle * M_PI / 180.0f);
    glVertex2f(playerX + dirX * 0.5f, playerY + dirY * 0.5f); // Extend 0.5 units
    glEnd();

    // Draw creature as a red circle
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(creatureX, creatureY); // Center
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * M_PI * i / 20.0f;
        glVertex2f(creatureX + 0.2f * cosf(angle), creatureY + 0.2f * sinf(angle)); // Radius 0.2
    }
    glEnd();

    // Re-enable textures for main window
    glEnable(GL_TEXTURE_2D);

    glfwSwapBuffers(topDownWindow);
}
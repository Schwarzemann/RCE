#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLFWwindow* window;

float playerX = 0.0f;
float playerY = 0.0f;
float playerAngle = 0.0f;
float playerSpeed = 0.1f;

// Frame rate variables
float lastTime = 0.0f;
float frameRate = 0.0f;

// Sprite animation
const char* spriteFiles[] = {
    "sprites/Lost_Soul_Front.png",
    "sprites/Lost_Soul_Side.png",
    "sprites/Lost_Soul_Side2.png",
    "sprites/Lost_Soul_Back.png",
    "sprites/Lost_Soul_Back2.png",
    "sprites/Lost_Soul_Side4.png",
    "sprites/Lost_Soul_Side3.png"
};
#define NUM_SPRITES 7
GLuint spriteTextures[NUM_SPRITES];
float animationTime = 0.0f; // Time for animation
float animationSpeed = 400.0f; // Frames per second for animation (adjust for speed)

void loadTexture(const char *filename, GLuint *textureID) {
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) {
        glGenTextures(1, textureID);
        glBindTexture(GL_TEXTURE_2D, *textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        printf("Loading %s: Width=%d, Height=%d, Channels=%d\n", filename, width, height, nrChannels);

        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            printf("Warning: Texture format not supported for %s (Channels: %d)\n", filename, nrChannels);
            stbi_image_free(data);
            *textureID = 0; // Indicate failure
            return;
        }

        // Use GLEW to check if glGenerateMipmap is available
        if (GLEW_VERSION_2_0) {
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            printf("Warning: glGenerateMipmap not supported, using GL_NEAREST for %s\n", filename);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        stbi_image_free(data);
    } else {
        printf("Error: Failed to load texture from %s\n", filename);
        *textureID = 0; // Indicate failure
    }
}

void initTextures() {
    printf("Initializing textures...\n");
    for (int i = 0; i < NUM_SPRITES; i++) {
        loadTexture(spriteFiles[i], &spriteTextures[i]);
        if (spriteTextures[i] == 0) {
            printf("Failed to load %s. Using fallback.\n", spriteFiles[i]);
            // Create a simple fallback texture (red square)
            unsigned char data[4] = {255, 0, 0, 255}; // Red
            glGenTextures(1, &spriteTextures[i]);
            glBindTexture(GL_TEXTURE_2D, spriteTextures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            if (GLEW_VERSION_2_0) {
                glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
        } else {
            printf("Successfully loaded %s as texture %u\n", spriteFiles[i], spriteTextures[i]);
        }
    }
}

void renderSprite() {
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - lastTime;
    animationTime += deltaTime * animationSpeed; // Advance animation based on time and speed
    int spriteIndex = (int)(animationTime) % NUM_SPRITES; // Cycle through sprites

    printf("Rendering sprite %d (index %d) at time %.2f\n", spriteIndex, spriteIndex, animationTime);

    if (spriteTextures[spriteIndex] != 0) { // Only render if texture loaded
        // Debug: Check if texture is bound correctly
        printf("Attempting to bind texture %u for sprite %d\n", spriteTextures[spriteIndex], spriteIndex);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, spriteTextures[spriteIndex]);
        
        // Debug: Check current texture binding
        GLint currentTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
        printf("Current bound texture: %d (expected %u)\n", currentTexture, spriteTextures[spriteIndex]);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Try rendering in both 3D and 2D to isolate the issue
        printf("Trying 3D render...\n");
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, 0.0f); // Center in 3D view
        glScalef(2.0f, 2.0f, 1.0f); // Scale for visibility
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Full opacity
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // Bottom-left
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f); // Bottom-right
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f); // Top-right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f); // Top-left
        glEnd();
        glPopMatrix();

        // Try 2D render as a fallback
        printf("Trying 2D render...\n");
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f); // Bottom-left
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.5f, -0.5f); // Bottom-right
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.5f,  0.5f); // Top-right
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f,  0.5f); // Top-left
        glEnd();
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    } else {
        printf("Error: Texture for sprite %d not loaded, rendering red quad.\n", spriteIndex);
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
        glEnd();
    }

    // Check OpenGL state after rendering
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL Error after sprite render: %d\n", error);
    }
}

void renderSprite2D() {
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - lastTime;
    animationTime += deltaTime * animationSpeed;
    int spriteIndex = (int)(animationTime) % NUM_SPRITES;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    if (spriteTextures[spriteIndex] != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, spriteTextures[spriteIndex]);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f,  0.5f);
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    } else {
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.5f,  0.5f);
        glVertex2f(-0.5f,  0.5f);
        glEnd();
    }

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void renderTestQuad() { // Test basic 3D rendering
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f( 0.5f, -0.5f, 0.0f);
    glVertex3f( 0.5f,  0.5f, 0.0f);
    glVertex3f(-0.5f,  0.5f, 0.0f);
    glEnd();
}

void renderText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);

    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Debug Render", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // Enable blending globally for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE); // Ensure faces aren’t culled

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Check OpenGL version and vendor for debugging
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
    printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));

    // Set viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize GLUT
    int argc = 1;
    char* argv[1] = {(char*)"Debug Render"};
    glutInit(&argc, argv);

    // Set up input callbacks
    glfwSetKeyCallback(window, keyCallback);

    // Initialize textures
    initTextures();

    lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        frameRate = 1.0f / deltaTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Test basic rendering with a white quad first
        printf("Rendering test quad...\n");
        renderTestQuad();
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("OpenGL Error after test quad render: %d\n", error);
        }

        // Render sprite animation
        renderSprite();

        error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("OpenGL Error after sprite render: %d\n", error);
        }

        renderText(10, SCREEN_HEIGHT - 30, "Player X: ");
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%.2f", playerX);
        renderText(100, SCREEN_HEIGHT - 30, buffer);

        renderText(10, SCREEN_HEIGHT - 60, "Player Y: ");
        snprintf(buffer, sizeof(buffer), "%.2f", playerY);
        renderText(100, SCREEN_HEIGHT - 60, buffer);

        renderText(10, SCREEN_HEIGHT - 90, "PA: ");
        snprintf(buffer, sizeof(buffer), "%.2f", playerAngle);
        renderText(100, SCREEN_HEIGHT - 90, buffer);

        renderText(10, SCREEN_HEIGHT - 120, "FR: ");
        snprintf(buffer, sizeof(buffer), "%.2f FPS", frameRate);
        renderText(100, SCREEN_HEIGHT - 120, buffer);

        error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("OpenGL Error after text render: %d\n", error);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < NUM_SPRITES; i++) {
        if (spriteTextures[i] != 0) {
            glDeleteTextures(1, &spriteTextures[i]);
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
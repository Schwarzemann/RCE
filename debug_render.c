#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define NUM_COLOR_SCHEMES 4

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLFWwindow* window;

// Cube rotation
float cubeAngleX = 0.0f;
float cubeAngleY = 0.0f;

float playerX = 0.0f;
float playerY = 0.0f;
float playerAngle = 0.0f;
float playerSpeed = 0.1f;

// Frame rate variables
float lastTime = 0.0f;
float frameRate = 0.0f;

// Color schemes
typedef struct {
    float colors[6][4][3]; // RGB colors for each vertex of each face
} ColorScheme;

ColorScheme colorSchemes[NUM_COLOR_SCHEMES];
int currentColorScheme = 0;

void initColorSchemes() {
    // Color Scheme 1: Solid colors for each face
    // Front face
    for (int vertex = 0; vertex < 4; vertex++) {
        colorSchemes[0].colors[0][vertex][0] = 1.0f; // Red
        colorSchemes[0].colors[0][vertex][1] = 0.0f;
        colorSchemes[0].colors[0][vertex][2] = 0.0f;
    }
    // Back face
    for (int vertex = 0; vertex < 4; vertex++) {
        colorSchemes[0].colors[1][vertex][0] = 0.0f; // Green
        colorSchemes[0].colors[1][vertex][1] = 1.0f;
        colorSchemes[0].colors[1][vertex][2] = 0.0f;
    }
    // Top face
    for (int vertex = 0; vertex < 4; vertex++) {
        colorSchemes[0].colors[2][vertex][0] = 0.0f; // Blue
        colorSchemes[0].colors[2][vertex][1] = 0.0f;
        colorSchemes[0].colors[2][vertex][2] = 1.0f;
    }
    // Bottom face
    for (int vertex = 0; vertex < 4; vertex++) {
        colorSchemes[0].colors[3][vertex][0] = 1.0f; // Yellow
        colorSchemes[0].colors[3][vertex][1] = 1.0f;
        colorSchemes[0].colors[3][vertex][2] = 0.0f;
    }
    // Right face
    for (int vertex = 0; vertex < 4; vertex++) {
        colorSchemes[0].colors[4][vertex][0] = 1.0f; // Magenta
        colorSchemes[0].colors[4][vertex][1] = 0.0f;
        colorSchemes[0].colors[4][vertex][2] = 1.0f;
    }
    // Left face
    for (int vertex = 0; vertex < 4; vertex++) {
        colorSchemes[0].colors[5][vertex][0] = 0.0f; // Cyan
        colorSchemes[0].colors[5][vertex][1] = 1.0f;
        colorSchemes[0].colors[5][vertex][2] = 1.0f;
    }

    // Color Scheme 2: Lighter solid colors
    // Front face
    for (int vertex = 0; vertex < 4; vertex++) {
        colorSchemes[1].colors[0][vertex][0] = 1.0f;
        colorSchemes[1].colors[0][vertex][1] = 0.5f;
        colorSchemes[1].colors[0][vertex][2] = 0.0f;
    }
    // Initialize other faces similarly...

    // Color Scheme 3: Grayscale colors
    for (int face = 0; face < 6; face++) {
        float gray = 0.2f + 0.15f * face;
        for (int vertex = 0; vertex < 4; vertex++) {
            colorSchemes[2].colors[face][vertex][0] = gray;
            colorSchemes[2].colors[face][vertex][1] = gray;
            colorSchemes[2].colors[face][vertex][2] = gray;
        }
    }

    // Color Scheme 4: Gradient colors
    // Front face (gradient from red to green to blue to yellow)
    colorSchemes[3].colors[0][0][0] = 1.0f; // Red
    colorSchemes[3].colors[0][0][1] = 0.0f;
    colorSchemes[3].colors[0][0][2] = 0.0f;

    colorSchemes[3].colors[0][1][0] = 0.0f; // Green
    colorSchemes[3].colors[0][1][1] = 1.0f;
    colorSchemes[3].colors[0][1][2] = 0.0f;

    colorSchemes[3].colors[0][2][0] = 0.0f; // Blue
    colorSchemes[3].colors[0][2][1] = 0.0f;
    colorSchemes[3].colors[0][2][2] = 1.0f;

    colorSchemes[3].colors[0][3][0] = 1.0f; // Yellow
    colorSchemes[3].colors[0][3][1] = 1.0f;
    colorSchemes[3].colors[0][3][2] = 0.0f;

    // Copy the gradient to other faces for demonstration
    for (int face = 1; face < 6; face++) {
        for (int vertex = 0; vertex < 4; vertex++) {
            for (int color = 0; color < 3; color++) {
                colorSchemes[3].colors[face][vertex][color] = colorSchemes[3].colors[0][vertex][color];
            }
        }
    }
}

void renderCube() {
    glBegin(GL_QUADS);
    for (int face = 0; face < 6; face++) {
        for (int vertex = 0; vertex < 4; vertex++) {
            glColor3fv(colorSchemes[currentColorScheme].colors[face][vertex]);
            switch (face) {
                case 0: // Front face
                    if (vertex == 0) glVertex3f(-1.0f, -1.0f,  1.0f);
                    if (vertex == 1) glVertex3f( 1.0f, -1.0f,  1.0f);
                    if (vertex == 2) glVertex3f( 1.0f,  1.0f,  1.0f);
                    if (vertex == 3) glVertex3f(-1.0f,  1.0f,  1.0f);
                    break;
                case 1: // Back face
                    if (vertex == 0) glVertex3f(-1.0f, -1.0f, -1.0f);
                    if (vertex == 1) glVertex3f(-1.0f,  1.0f, -1.0f);
                    if (vertex == 2) glVertex3f( 1.0f,  1.0f, -1.0f);
                    if (vertex == 3) glVertex3f( 1.0f, -1.0f, -1.0f);
                    break;
                case 2: // Top face
                    if (vertex == 0) glVertex3f(-1.0f,  1.0f, -1.0f);
                    if (vertex == 1) glVertex3f(-1.0f,  1.0f,  1.0f);
                    if (vertex == 2) glVertex3f( 1.0f,  1.0f,  1.0f);
                    if (vertex == 3) glVertex3f( 1.0f,  1.0f, -1.0f);
                    break;
                case 3: // Bottom face
                    if (vertex == 0) glVertex3f(-1.0f, -1.0f, -1.0f);
                    if (vertex == 1) glVertex3f( 1.0f, -1.0f, -1.0f);
                    if (vertex == 2) glVertex3f( 1.0f, -1.0f,  1.0f);
                    if (vertex == 3) glVertex3f(-1.0f, -1.0f,  1.0f);
                    break;
                case 4: // Right face
                    if (vertex == 0) glVertex3f( 1.0f, -1.0f, -1.0f);
                    if (vertex == 1) glVertex3f( 1.0f,  1.0f, -1.0f);
                    if (vertex == 2) glVertex3f( 1.0f,  1.0f,  1.0f);
                    if (vertex == 3) glVertex3f( 1.0f, -1.0f,  1.0f);
                    break;
                case 5: // Left face
                    if (vertex == 0) glVertex3f(-1.0f, -1.0f, -1.0f);
                    if (vertex == 1) glVertex3f(-1.0f, -1.0f,  1.0f);
                    if (vertex == 2) glVertex3f(-1.0f,  1.0f,  1.0f);
                    if (vertex == 3) glVertex3f(-1.0f,  1.0f, -1.0f);
                    break;
            }
        }
    }
    glEnd();
}

void renderText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // Save the current projection matrix
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT); // Orthographic projection for 2D text
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); // Save the current modelview matrix
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // Disable depth testing for text rendering
    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glRasterPos2f(x, y);

    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }

    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
    glPopMatrix(); // Restore the modelview matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Restore the projection matrix
    glMatrixMode(GL_MODELVIEW);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Change color scheme when Enter is pressed
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        currentColorScheme = (currentColorScheme + 1) % NUM_COLOR_SCHEMES;
    }

    // Close window on Escape key
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

    // Initialize GLUT
    int argc = 1;
    char* argv[1] = {(char*)"Debug Render"};
    glutInit(&argc, argv);

    // Set up input callbacks
    glfwSetKeyCallback(window, keyCallback);

    lastTime = (float)glfwGetTime(); // Initialize lastTime

    initColorSchemes(); // Initialize the color schemes

    while (!glfwWindowShouldClose(window)) {
        // Update time-related variables
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        frameRate = 1.0f / deltaTime; // Calculate frame rate

        // Rotate the cube over time
        cubeAngleY += deltaTime * 50.0f; // Rotate 50 degrees per second

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Set up modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Camera position and direction
        gluLookAt(0.0f, 0.0f, 5.0f,   // Eye position
                  0.0f, 0.0f, 0.0f,   // Look-at point
                  0.0f, 1.0f, 0.0f);  // Up vector

        // Render the cube at the origin
        glPushMatrix();
        glRotatef(cubeAngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(cubeAngleY, 0.0f, 1.0f, 0.0f);
        renderCube();
        glPopMatrix();

        // Render debug text
        renderText(10, SCREEN_HEIGHT - 30, "Player X: ");
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%.2f", playerX);
        renderText(100, SCREEN_HEIGHT - 30, buffer);

        renderText(10, SCREEN_HEIGHT - 60, "Player Y: ");
        snprintf(buffer, sizeof(buffer), "%.2f", playerY);
        renderText(100, SCREEN_HEIGHT - 60, buffer);

        renderText(10, SCREEN_HEIGHT - 90, "PA: \n"); // Player Angle
        snprintf(buffer, sizeof(buffer), "%.2f", playerAngle);
        renderText(100, SCREEN_HEIGHT - 90, buffer);

        renderText(10, SCREEN_HEIGHT - 120, "FR: "); // Frame Rate
        snprintf(buffer, sizeof(buffer), "%.2f FPS", frameRate);
        renderText(100, SCREEN_HEIGHT - 120, buffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

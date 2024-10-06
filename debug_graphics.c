#include "debug_graphics.h"

// Definitions of global variables
float cubeAngleX = 0.0f;
float cubeAngleY = 0.0f;

float playerX = 0.0f;
float playerY = 0.0f;
float playerAngle = 0.0f;
float playerSpeed = 0.1f;

float lastTime = 0.0f;
float frameRate = 0.0f;

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

    // You can fill in the other color schemes similarly...
    // For brevity, only the first color scheme is fully initialized here.

    // Initialize other color schemes...
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

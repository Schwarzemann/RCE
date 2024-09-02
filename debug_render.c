#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h> // For cos, sin
#include <string.h> // For strlen
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

GLFWwindow* window;
float cubeAngleX = 0.0f; // Rotation angle around the X axis
float cubeAngleY = 0.0f; // Rotation angle around the Y axis
float playerX = 0.0f;
float playerY = 0.0f;
float playerAngle = 0.0f;
float playerSpeed = 0.1f;
float mouseSensitivity = 0.1f;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// Frame rate variables
float lastTime = 0.0f;
float frameRate = 0.0f;

void renderCube() {
    glBegin(GL_QUADS);
    
    // Front face
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Top face
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Bottom face
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Left face
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    glEnd();
}

void renderText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT); // Orthographic projection for 2D text
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glRasterPos2f(x, y);

    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    // Update rotation angles based on mouse movement
    cubeAngleY += xoffset;
    cubeAngleX -= yoffset; // Inverted to match typical mouse behavior

    // Clamp vertical rotation to avoid flipping
    if (cubeAngleX > 89.0f) cubeAngleX = 89.0f;
    if (cubeAngleX < -89.0f) cubeAngleX = -89.0f;
}

void handleInput() {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        playerX += cos(playerAngle) * playerSpeed;
        playerY += sin(playerAngle) * playerSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        playerX -= cos(playerAngle) * playerSpeed;
        playerY -= sin(playerAngle) * playerSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        playerAngle -= 0.02f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        playerAngle += 0.02f;
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

    // Set up mouse input
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Set cursor to normal mode

    lastTime = (float)glfwGetTime(); // Initialize lastTime

    while (!glfwWindowShouldClose(window)) {
        // Update time-related variables
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        frameRate = 1.0f / deltaTime; // Calculate frame rate

        handleInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 100.0f);

        // Set up modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-playerX, -playerY, -5.0f);

        // Apply cube rotation based on mouse input
        glPushMatrix();
        glRotatef(cubeAngleX, 1.0f, 0.0f, 0.0f); // Rotate around X axis
        glRotatef(cubeAngleY, 0.0f, 1.0f, 0.0f); // Rotate around Y axis
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

        renderText(10, SCREEN_HEIGHT - 150, "Mouse X: ");
        snprintf(buffer, sizeof(buffer), "%.2f", lastX);
        renderText(100, SCREEN_HEIGHT - 150, buffer);

        renderText(10, SCREEN_HEIGHT - 180, "Mouse Y: ");
        snprintf(buffer, sizeof(buffer), "%.2f", lastY);
        renderText(100, SCREEN_HEIGHT - 180, buffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

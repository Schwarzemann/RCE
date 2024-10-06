#include "debug_globals.h"
#include "debug_graphics.h"
#include "debug_input.h"

GLFWwindow* window; // Define the window variable

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
        char buffer[100];

        snprintf(buffer, sizeof(buffer), "Player X: %.2f", playerX);
        renderText(10, SCREEN_HEIGHT - 30, buffer);

        snprintf(buffer, sizeof(buffer), "Player Y: %.2f", playerY);
        renderText(10, SCREEN_HEIGHT - 60, buffer);

        snprintf(buffer, sizeof(buffer), "PA: %.2f", playerAngle); // Player Angle
        renderText(10, SCREEN_HEIGHT - 90, buffer);

        snprintf(buffer, sizeof(buffer), "FR: %.2f FPS", frameRate); // Frame Rate
        renderText(10, SCREEN_HEIGHT - 120, buffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

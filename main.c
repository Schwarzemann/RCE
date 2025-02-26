#include "rce_common.h"
#include "rce_graphics.h"
#include "rce_input.h"
#include "rce_player.h"
#include "rce_maze.h"
#include "rce_creature.h"

int main() {
    GLFWwindow *mainWindow;
    GLFWwindow *topDownWindow;

    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create main 3D window
    initOpenGL(&mainWindow);

    // Create top-down 2D window
    topDownWindow = glfwCreateWindow(MAP_WIDTH * 20, MAP_HEIGHT * 20, "RCE Top-down view", NULL, mainWindow);
    if (!topDownWindow) {
        fprintf(stderr, "Failed to create top-down window\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    // Set up initial state
    generateMaze();
    initCreature();

    while (!glfwWindowShouldClose(mainWindow) && !glfwWindowShouldClose(topDownWindow)) {
        // Update logic
        handleInput(mainWindow);
        updateCreaturePosition(playerX, playerY);

        // Draw main 3D view
        glfwMakeContextCurrent(mainWindow);
        drawMaze();
        drawCreature();

        // Draw top-down 2D view
        drawTopDownView(topDownWindow);

        // Log status
        logPlayerStatus();
        logCreatureStatus(playerX, playerY);

        // Swap buffers for main window
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(topDownWindow);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
#include "common.h"
#include "graphics.h"
#include "input.h"
#include "player.h"
#include "maze.h"

int main() {
    GLFWwindow *window;
    initOpenGL(&window);
    generateMaze();  // Generate maze only once when the program starts

    while (!glfwWindowShouldClose(window)) {
        handleInput(window);  // Check for inputs, including regenerating maze with 'R'

        drawMaze();           // Render the maze

        logPlayerStatus();    // Log the player’s status every frame

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
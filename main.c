#include "rce_common.h"
#include "rce_graphics.h"
#include "rce_input.h"
#include "rce_player.h"
#include "rce_maze.h"

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
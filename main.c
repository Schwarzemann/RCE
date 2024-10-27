#include "rce_common.h"
#include "rce_graphics.h"
#include "rce_input.h"
#include "rce_player.h"
#include "rce_maze.h"
#include "rce_creature.h"

int main() {
    GLFWwindow *window;
    initOpenGL(&window);
    generateMaze();  // Generate maze only once when the program starts
    initCreature();  // Initialize the creature's starting position

    while (!glfwWindowShouldClose(window)) {
        handleInput(window);           // Check for inputs, including regenerating maze with 'R'
        updateCreaturePosition(playerX, playerY); // Update creature position to follow the player

        drawMaze();                    // Render the maze
        drawCreature();                // Render the creature

        logPlayerStatus();             // Log the player’s status every frame

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
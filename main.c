#include "common.h"
#include "graphics.h"
#include "input.h"
#include "player.h"

int main() {
    GLFWwindow *window;
    initOpenGL(&window);

    while (!glfwWindowShouldClose(window)) {
        handleInput(window);

        drawMaze();

        logPlayerStatus(); // Log the player’s status every frame

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

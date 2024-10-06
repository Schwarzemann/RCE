#include "debug_input.h"

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

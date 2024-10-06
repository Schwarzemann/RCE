#ifndef GLOBALS_H
#define GLOBALS_H

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

extern GLFWwindow* window;

// Cube rotation
extern float cubeAngleX;
extern float cubeAngleY;

// Player variables (if needed)
extern float playerX;
extern float playerY;
extern float playerAngle;
extern float playerSpeed;

// Frame rate variables
extern float lastTime;
extern float frameRate;

// Color schemes
typedef struct {
    float colors[6][4][3]; // RGB colors for each vertex of each face
} ColorScheme;

extern ColorScheme colorSchemes[NUM_COLOR_SCHEMES];
extern int currentColorScheme;

#endif // GLOBALS_H

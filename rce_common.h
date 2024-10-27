#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAP_WIDTH  10
#define MAP_HEIGHT 10
#define M_PI  3.1415926535897932384626433
#define DEG_TO_RAD(angleDegrees) ((angleDegrees) * M_PI / 180.0f)

extern int maze[MAP_HEIGHT][MAP_WIDTH];

extern float playerX;
extern float playerY;
extern float playerAngle;
extern float playerVerticalAngle;
extern float moveSpeed;
extern float rotSpeed;
extern int screenWidth;
extern int screenHeight;
extern GLuint textureID;

#endif

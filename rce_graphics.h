#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "rce_common.h"

void initOpenGL(GLFWwindow **window);
GLuint loadTexture(const char *filename);
void drawMaze();

#endif

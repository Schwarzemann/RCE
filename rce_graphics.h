#ifndef RCE_GRAPHICS_H
#define RCE_GRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern int screenWidth;
extern int screenHeight;
extern GLuint textureID;
extern GLuint creatureTextureID;

GLuint loadTexture(const char *filename);
GLuint loadFallbackTexture();
void initOpenGL(GLFWwindow **window);
void drawMaze();
void drawTopDownView(GLFWwindow *topDownWindow);

#endif
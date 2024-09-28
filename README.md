# RCE
OpenGL Ray Casting Engine written completely in C by the people, for the people.
It is still in a very early stage since this was a stupid little thing that I meddled through sunday but it gave me an idea so here we go.

## Screenshots

![Main RCE Engine running a maze](https://github.com/Schwarzemann/RCE/blob/main/doc/rcegif.gif)
![Debug Renderer doing its job](https://github.com/Schwarzemann/RCE/blob/main/doc/screenshot_2.png)

## How to build
It is pretty straightforward. The code is still pretty simple and doesn't require a Makefile or anything.
However, this will change in the future. In the meantime anyone can use the commands given below.

```
gcc -v -o raycasting raycasting.c -lGL -lGLU -lglut -lglfw -lm
```
```
gcc -v -o debug_render debug_render.c -lGL -lGLU -lglut -lglfw -lm
```

# RCE
OpenGL Ray Casting Engine written completely in C by the people, for the people.
It is still in a very early stage since this was a stupid little thing that I meddled through sunday but it gave me an idea so here we go.

## Screenshots

![Main RCE Engine running a maze](https://github.com/Schwarzemann/RCE/blob/main/doc/rcegif.gif)
![Debug Renderer doing its job](https://github.com/Schwarzemann/RCE/blob/main/doc/drgid.gif)

## How to build
The makefile inside the project can be used to build all the components.
Here's how to use them.

#### Only build the raycasting engine
```
make
```
#### Build the debug renderer as well
```
make debug
```
#### Clean everything after
```
make clean
```

```
gcc -v -o debug_render debug_render.c -lGL -lGLU -lglut -lglfw -lm
```

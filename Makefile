# Compiler and Flags
CC = gcc
CFLAGS = -Wall -g
LIBS = -lGL -lGLU -lglut -lglfw -lGLEW -lm

# Object files for RCE
RCE_OBJECTS = main.o maze.o player.o graphics.o input.o

# Object files for debug_render
DEBUG_OBJECTS = debug_main.o debug_graphics.o debug_input.o

# Default target
all: RCE

# RCE executable
RCE: $(RCE_OBJECTS)
	$(CC) $(CFLAGS) -o RCE $(RCE_OBJECTS) $(LIBS)

# Debug target to build debug_render
debug: debug_render

# debug_render executable
debug_render: $(DEBUG_OBJECTS)
	$(CC) $(CFLAGS) -o debug_render $(DEBUG_OBJECTS) $(LIBS)

# RCE object files compilation
main.o: main.c common.h graphics.h input.h player.h
	$(CC) $(CFLAGS) -c main.c

maze.o: maze.c maze.h common.h
	$(CC) $(CFLAGS) -c maze.c

player.o: player.c player.h common.h
	$(CC) $(CFLAGS) -c player.c

graphics.o: graphics.c graphics.h common.h maze.h player.h
	$(CC) $(CFLAGS) -c graphics.c

input.o: input.c input.h common.h maze.h player.h
	$(CC) $(CFLAGS) -c input.c

# debug_render object files compilation
main_debug.o: debug_main.c debug_globals.h debug_graphics.h debug_input.h
	$(CC) $(CFLAGS) -c debug_main.c -o debug_main.o

graphics_debug.o: debug_graphics.c debug_graphics.h debug_globals.h
	$(CC) $(CFLAGS) -c debug_graphics.c -o debug_graphics.o

input_debug.o: debug_input.c debug_input.h debug_globals.h
	$(CC) $(CFLAGS) -c debug_input.c -o debug_input.o

# Clean up
clean:
	rm -f *.o RCE debug_render

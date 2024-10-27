# Compiler and Flags
CC = gcc
CFLAGS = -Wall -g
LIBS = -lGL -lGLU -lglut -lglfw -lGLEW -lm

# Object files for RCE
RCE_OBJECTS = main.o rce_maze.o rce_player.o rce_graphics.o rce_input.o rce_creature.o

# Object files for debug_render (renamed with rce_ prefix)
DEBUG_OBJECTS = debug_graphics.o

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
main.o: main.c rce_common.h rce_graphics.h rce_input.h rce_player.h
	$(CC) $(CFLAGS) -c main.c

rce_maze.o: rce_maze.c rce_maze.h rce_common.h
	$(CC) $(CFLAGS) -c rce_maze.c

rce_player.o: rce_player.c rce_player.h rce_common.h
	$(CC) $(CFLAGS) -c rce_player.c

rce_graphics.o: rce_graphics.c rce_graphics.h rce_common.h rce_maze.h rce_player.h
	$(CC) $(CFLAGS) -c rce_graphics.c

rce_input.o: rce_input.c rce_input.h rce_common.h rce_maze.h rce_player.h
	$(CC) $(CFLAGS) -c rce_input.c

rce_creature.o: rce_creature.c rce_creature.h rce_maze.h
	$(CC) $(CFLAGS) -c rce_creature.c

# debug_render object files compilation (with rce_ prefix)
debug_graphics.o: debug_graphics.c
	$(CC) $(CFLAGS) -c debug_graphics.c -o debug_graphics.o

# Clean up
clean:
	rm -f *.o RCE debug_render

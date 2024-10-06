CC = gcc
CFLAGS = -Wall -g
LIBS = -lGL -lGLU -lglut -lglfw -lGLEW -lm

OBJECTS = main.o maze.o player.o graphics.o input.o

all: RCE

RCE: $(OBJECTS)
	$(CC) $(CFLAGS) -o RCE $(OBJECTS) $(LIBS)

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

clean:
	rm -f *.o RCE

all:
	gcc -g -std=c99 sdl_pong.c -o pm `sdl2-config --cflags --libs` -lm

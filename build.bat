del pm.exe
gcc -g sdl_pong.c -std=c99 -IC:\mingw_dev_lib\include\SDL2 -I "C:\Program Files (x86)\Windows Kits\8.1\Include\shared" -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o pm

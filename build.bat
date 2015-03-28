REM C:\winkits\Include\shared is "C:\Window Kits\8.0\"

gcc sdl_pong.c -std=c99 -IC:\mingw_dev_lib\include\SDL2 -IC:\winkits\Include\shared -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows -lmingw64 -lSDL2main -lSDL2 -o pm
gcc -g sdl_pong.c -std=c99 -IC:\mingw_dev_lib\include\SDL2 -IC:\winkits\Include\shared -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o pm

@echo off

if not exist bin\win32 mkdir bin\win32
tcc -I src -L lib\win32 -lSDL2 -lSDL2_mixer -D BUILD_DEBUG -bench -g -o bin\win32\FISH.exe src\main_win32.c

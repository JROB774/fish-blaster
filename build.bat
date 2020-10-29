@echo off

if not exist bin mkdir bin
tcc -I src -L lib/win32 -lSDL2 -lSDL2_mixer -D BUILD_DEBUG -bench -g -o bin/win32/FISH.exe src/main.c

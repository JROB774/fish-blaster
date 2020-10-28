@echo off

if not exist bin mkdir bin
tcc -I src -L lib -lSDL2 -lSDL2_mixer -D BUILD_DEBUG -bench -g -o bin/FISH.exe src/main.c

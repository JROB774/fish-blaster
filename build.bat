@echo off

if not exist bin mkdir bin
tcc -I src -L lib -lSDL2 -D BUILD_DEBUG -bench -g -o bin/fish.exe src/main.c

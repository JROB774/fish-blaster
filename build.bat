@echo off

if not exist bin mkdir bin
tcc -bench -g -I src -L lib -lSDL2 -o bin/fish.exe src/main.c

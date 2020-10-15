@echo off

if not exist bin mkdir bin
tcc -bench -o bin/fish.exe src/main.c

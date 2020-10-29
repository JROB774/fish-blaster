@echo off

if not exist bin\web mkdir bin\web
emcc src\main_web.c -s USE_SDL=2 -s USE_SDL_MIXER=2 --preload-file dat -o bin\web\fish.html

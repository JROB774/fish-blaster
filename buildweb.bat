@echo off

if not exist bin\web mkdir bin\web
emcc src\main_web.c -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 --preload-file dat -o bin\web\fish.html

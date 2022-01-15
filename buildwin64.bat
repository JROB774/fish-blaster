@echo off

:: can be either "debug" or "release"
set BuildMode=release

set ExtraFlags=

if %BuildMode% == debug set ExtraFlags=%ExtraFlags% -D BUILD_DEBUG -g
if %BuildMode% == release set ExtraFlags=%ExtraFlags% -Wl,-subsystem=windows

if not exist bin\win32\x64 mkdir bin\win32\x64
tcc\tcc -I src -L lib\win32 -lSDL2 -lSDL2_mixer -bench -m64 -D BUILD_64BIT %ExtraFlags% -o bin\win32\x64\FISH.exe src\main_win32.c res\iconx64.res

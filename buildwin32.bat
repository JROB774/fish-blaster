@echo off

:: can be either "debug" or "release"
set BuildMode=release

set ExtraFlags=

if %BuildMode% == debug set ExtraFlags=%ExtraFlags% -D BUILD_DEBUG -g
if %BuildMode% == release set ExtraFlags=%ExtraFlags% -Wl,-subsystem=windows

if not exist bin\win32\x86 mkdir bin\win32\x86
tcc\tcc -I src -L lib\win32 -lSDL2 -lSDL2_mixer -bench -m32 -D BUILD_32BIT %ExtraFlags% -o bin\win32\x86\FISH.exe src\main_win32.c res\iconx86.res

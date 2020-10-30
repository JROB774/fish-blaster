@echo off

set DebugFlags=-D BUILD_DEBUG -g

:: Build both the 32-bit and 64-bit versions of the application.
if not exist bin\win32\x64 mkdir bin\win32\x64
tcc -I src -L lib\win32 -lSDL2 -lSDL2_mixer -D BUILD_64BIT -bench %DebugFlags% -o bin\win32\x64\FISH.exe src\main_win32.c res\icon64.res
if not exist bin\win32\x86 mkdir bin\win32\x86
i386-win32-tcc -I src -L lib\win32 -lSDL2 -lSDL2_mixer -D BUILD_32BIT -bench %DebugFlags% -o bin\win32\x86\FISH.exe src\main_win32.c res\icon32.res

@echo off

:: can be either "x86" or "x64"
set Architecture=x64
:: can be either "debug" or "release"
set BuildMode=release

set ExtraFlags=

if %Architecture% == x86 set ExtraFlags=%ExtraFlags% -m32 -D BUILD_32BIT
if %Architecture% == x64 set ExtraFlags=%ExtraFlags% -m64 -D BUILD_64BIT

if %BuildMode% == debug set ExtraFlags=%ExtraFlags% -D BUILD_DEBUG -g
if %BuildMode% == release set ExtraFlags=%ExtraFlags% -Wl,-subsystem=windows

if not exist bin\win32\%Architecture% mkdir bin\win32\%Architecture%
tcc\tcc -I src -L lib\win32 -lSDL2 -lSDL2_mixer -bench %ExtraFlags% -o bin\win32\%Architecture%\FISH.exe src\main_win32.c res\icon%Architecture%.res

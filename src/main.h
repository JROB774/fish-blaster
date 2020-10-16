#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// Defining these as blank is necessary in order to get SDL2 working with TCC.
// We also had to modify some typedefs in SDL_config.h, we will look into
// making that part more portable across compiler's in the future. However,
// for now it is fine as it is as we are the only ones compiling this source.
#ifdef __TINYC__
#define SDLCALL
#define DECLSPEC
#endif

#define SDL_MAIN_HANDLED 1
#include <SDL2/SDL.h>

#define INTERNAL static
#define GLOBAL   static

#define CLAMP(x,low,high) (((x)>(high))?(high):(((x)<(low))?(low):(x)))

#define ABS(a) (((a)<0)?-(a):(a))

#define SWAP(x,y,t) do { t tmp__ = x; x = y; y = tmp__; } while (0)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define CAST(t,x) ((t)(x))

typedef int bool;
#define false 0
#define true  1

typedef  uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef   int8_t  S8;
typedef  int16_t S16;
typedef  int32_t S32;
typedef  int64_t S64;

// Headers
#include "renderer.h"
#include "window.h"

// Sources
#include "renderer.c"
#include "window.c"

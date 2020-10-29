#ifndef UTILITY_H
#define UTILITY_H

#define INTERNAL static
#define GLOBAL   static

#define ARRAYSIZE(arr) (sizeof((arr))/sizeof((arr)[0]))

#define CLAMP(x,low,high) (((x)>(high))?(high):(((x)<(low))?(low):(x)))

#define ABS(a) (((a)<0)?-(a):(a))

#define SWAP(x,y,t) do { t tmp__ = x; x = y; y = tmp__; } while (0)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define CAST(t,x) ((t)(x))

typedef struct Rect__ { int x,y,w,h; } Rect;

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

// UTILITY FUNCTIONS

INTERNAL void rotate_vec2 (float* x, float* y, float rad);

INTERNAL float lerp (float a, float b, float t);

INTERNAL float round_to_multiple (float num, float multiple);

INTERNAL void  seed_random        ();
INTERNAL int   random_int         ();
INTERNAL int   random_int_range   (int   min, int   max);
INTERNAL float random_float       ();
INTERNAL float random_float_range (float min, float max);

#endif /* UTILITY_H */

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

typedef int bool;
#define false 0
#define true  1

typedef int HDir;
#define HDIR_R 0
#define HDIR_L 1
typedef int VDir;
#define VDIR_U 0
#define VDIR_D 1

typedef  uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef   int8_t  S8;
typedef  int16_t S16;
typedef  int32_t S32;
typedef  int64_t S64;

#endif /* UTILITY_H */

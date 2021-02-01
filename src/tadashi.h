#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <inttypes.h>
#include <float.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>


#define global static
#define internal static
#define local_persist static
#define ArrayCount(x) (sizeof(x) / sizeof((x)[0]))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef s32 b32;
typedef s32 b32x;

typedef float f32;
typedef double f64;


const u64 U64Max_ = ((u64)-1);
const u32 U32Max_ = ((u32)-1);
const f32 F32Max_ = FLT_MAX;
const f32 INF_ = FLT_MAX;
const f32 F32Min_ = -FLT_MAX;
const f32 Pi32_ = 3.1415926535897f;
const f32 Tau32_ = 6.283185307179f;

#define U32Max U32Max_
#define F32Max F32Max_
#define INF INF_
#define F32Min F32Min_
#define Pi32 Pi32_
#define Tau32 Tau32_
#define U64Max U64Max_

#define auto auto

#define Kilobytes(x) (1024ll*x)
#define Megabytes(x) (1024ll*Kilobytes(x))
#define Gigabytes(x) (1024ll*Megabytes(x))

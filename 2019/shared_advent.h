/*
====================================================================
$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2018 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/

#ifndef SHARED_ADVENT_H
#define SHARED_ADVENT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t memory_index;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;

typedef float f32;
typedef double f64;

#define internal static
#define global static
#define local_persist static

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array[0])))
#define Assert(Expression) if(!(Expression)){int *AFFFFFF = 0; *AFFFFFF = 69;}

#define InvalidCodePath Assert(false)
#define InvalidDefaultCase default:{InvalidCodePath} break;

#define Bytes(V) V
#define Kilobytes(V) (Bytes(V) * 1024LL)
#define Megabytes(V) (Kilobytes(V) * 1024LL)
#define Gigabytes(V) (Megabytes(V) * 1024LL)
#define Terabytes(V) (Gigabytes(V) * 1024LL)

#define COLOR_RGB(R, G, B) ((R << 16) | (G << 8) | (B << 0))

#define Clamp(Value, Min, Max) if((Value) < (Min)){Value = (Min);}else if((Value) > (Max)){Value = (Max);}
#define Minimum(A, B) ((A < B) ? (A) : (B))
#define Maximum(A, B) ((A > B) ? (A) : (B))
#define IsDivisble(Number, Divident) (((Number) % (Divident) == 0) && ((Number) != 0))

#define XORSwap(A, B)       \
((A) ^= (B));               \
((B) ^= (A));               \
((A) ^= (B));

#if !__cplusplus
#define false 0
#define true 1
#endif

typedef struct loaded_file loaded_file;
struct loaded_file
{
    u8 *Contents;
    memory_index Size;
};

extern loaded_file ReadEntireFileAndZeroTerminate(char *FileName);

typedef struct
{
    void *Base;
    memory_index Size;
    memory_index Index;
}  memory_arena;

#define PushSize(Arena, Size) PushArena_(Arena, Size)
#define PushStruct(Arena, Type) PushArena_(Arena, sizeof(Type))
#define PushArray(Arena, Type, Count) PushArena_(Arena, sizeof(Type) * Count)

extern void* PushArena_(memory_arena *Arena, memory_index Size);
extern memory_arena InitMemoryArena(void *Base, memory_index Size);

#pragma pack(push, 1)
typedef struct bmp_header bmp_header;
struct bmp_header
{
    u16 Id;
    u32 Size; // NOTE(fuzes): The size of the BMP file in bytes
    u16 Reserved1;
    u16 Reserved2;
    u32 Offset; // NOTE(fuzes): The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
    
    u32 HeaderSize;
    s32 Width;
    s32 Height;
    u16 ColorPlanes;
    u16 BitsPerPixel;
    u32 Compression;
    u32 ImageSize;
    s32 HorizontalPixelPerMeter;
    s32 VerticalPixelPerMeter;
    s32 ColorPalette;
    s32 ImportantColors;
};
#pragma pack(pop)

#define BYTES_PER_PIXEL 4

extern bmp_header InitBitmapHeader(s32 ImageWidth, s32 ImageHeight);

typedef struct v2 v2;
struct v2
{
    f32 x;
    f32 y;
};

extern f32
V2Inner(v2 A, v2 B);

extern v2
V2Sub(v2 A, v2 B);

extern v2
V2Add(v2 A, v2 B);

#define DEGREE_TO_RADIAN 0.0174533f
#define DegreeToRadian(Degree) ((Degree) * DEGREE_TO_RADIAN)

extern f32
Cos(f32 A);

extern f32
Sin(f32 A);

extern f32
SquareRoot(f32 A);

extern f32
Abs(f32 A);

extern f32
Ceiling(f32 A);

// NOTE(fuzes): Based on the binary gcd algorithm on wikipedia
// https://en.wikipedia.org/wiki/Greatest_common_divisor#Binary_GCD_algorithm
// We are using shifting to divide the number by two (altough the compiler might be smart enough)
// Checking the last bit of a number if it is even.
#define IsEven(Number) (!((Number) & 1))
extern s64
Gcd(s64 A, s64 B);

// NOTE(fuzes): We can use this identity to get the gcd for 3 numbers.
// https://math.stackexchange.com/questions/93731/finding-the-gcd-of-three-numbers
// As others say, one way to do it is using the identity gcd(a,b,c)=gcd(a,(gcd(b,c)).
// This identity is true since the "gcd" is the maximal element of the intersection of 
// the sets of factors of the inputs.
extern s64 Gcd3(s64 A, s64 B, s64 C);

typedef struct tokenizer tokenizer;
struct tokenizer
{
    u8 *At;
    b32 Exit;
};

typedef enum token_type token_type;
enum token_type
{
    Token_Type_FirstNonAsci = 127,
    Token_Type_Identifier,
    Token_Type_Number,
};

typedef struct token token;
struct token
{
    token_type Type;
    u8 *Text;
    u32 TextLength;
    u32 Value;
};

extern token
GetToken(tokenizer *Tokenizer);

extern b32
TokenEquals(tokenizer *Tokenizer, token_type Type);

#endif // SHARED_ADVENT_H

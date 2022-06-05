/*
====================================================================
$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2018 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/

#pragma once

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

internal loaded_file
ReadEntireFileAndZeroTerminate(char *FileName)
{
    loaded_file Result;
    
    FILE *File = fopen(FileName, "rb");
    fseek(File, 0, SEEK_END);
    Result.Size = ftell(File) + 1;
    Result.Contents = (char *)malloc(Result.Size);
    
    fseek(File, 0, SEEK_SET);
    
    fread(Result.Contents, Result.Size, 1, File);
    fclose(File);
    
    Result.Contents[Result.Size - 1] = 0;
    
    return Result;
}

typedef struct
{
    void *Base;
    memory_index Size;
    memory_index Index;
}  memory_arena;

#define PushSize(Arena, Size) PushArena_(Arena, Size)
#define PushStruct(Arena, Type) PushArena_(Arena, sizeof(Type))
#define PushArray(Arena, Type, Count) PushArena_(Arena, sizeof(Type) * Count)

internal void*
PushArena_(memory_arena *Arena, memory_index Size)
{
    Assert((Arena->Index + Size) <= Arena->Size);
    
    void *Result;
    Result = (u8 *)Arena->Base + Arena->Index;
    Arena->Index += Size;
    return Result;
}

internal memory_arena
InitMemoryArena(void *Base, memory_index Size)
{
    memory_arena Result;
    Result.Base = Base;
    Result.Size = Size;
    Result.Index = 0;
    
    return Result;
}

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

internal bmp_header
InitBitmapHeader(s32 ImageWidth, s32 ImageHeight)
{
    u32 BytesPerPixel = 4;
    u32 ImageSize = ImageHeight * ImageWidth * 4;
    
    bmp_header Header = {0};
    Header.Id = 0x4D42;
    Header.Size = sizeof(bmp_header) + ImageSize;
    Header.Offset = sizeof(bmp_header);
    Header.HeaderSize = 40;
    Header.Width = ImageWidth;
    Header.Height = -(s32)ImageHeight;
    Header.ColorPlanes = 1;
    Header.BitsPerPixel = (u16)(BytesPerPixel * 8);
    Header.Compression = 0;
    Header.ImageSize = ImageSize;
    
    return Header;
}

typedef struct v2 v2;
struct v2
{
    f32 x;
    f32 y;
};

internal f32
V2Inner(v2 A, v2 B)
{
    f32 Result;
    Result = A.x * B.x + A.y * B.y;
    return Result;
}

internal v2
V2Sub(v2 A, v2 B)
{
    v2 Result;
    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    
    return Result;
}

internal v2
V2Add(v2 A, v2 B)
{
    v2 Result;
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    
    return Result;
}

#define DEGREE_TO_RADIAN 0.0174533f
#define DegreeToRadian(Degree) ((Degree) * DEGREE_TO_RADIAN)

internal f32
Cos(f32 A)
{
    f32 Result;
    Result = cosf(A);
    return (Result);
}

internal f32
Sin(f32 A)
{
    f32 Result;
    Result = sinf(A);
    return (Result);
}

internal f32
SquareRoot(f32 A)
{
    f32 Result;
    Result = sqrtf(A);
    return Result;
}

internal f32
Abs(f32 A)
{
    f32 Result;
    Result = fabsf(A);
    return(Result);
}

// NOTE(fuzes): Based on the binary gcd algorithm on wikipedia
// https://en.wikipedia.org/wiki/Greatest_common_divisor#Binary_GCD_algorithm
// We are using shifting to divide the number by two (altough the compiler might be smart enough)
// Checking the last bit of a number if it is even.
#define IsEven(Number) (!((Number) & 1))
internal s64
Gcd(s64 A, s64 B)
{
    s64 D = 0;
    while(IsEven(A) && IsEven(B))
    {
        A = A >> 1;
        B = B >> 1;
        ++D;
    }
    
    while(A != B)
    {
        if(IsEven(A))
        {
            A = A >> 1;
        }
        else if(IsEven(B))
        {
            B = B >> 1;
        }
        else if(A > B)
        {
            A = (A - B) >> 1;
        }
        else
        {
            B = (B - A) >> 1;
        }
    }
    
    return (s64)(pow(2, (double)D) * A);
}

// NOTE(fuzes): We can use this identity to get the gcd for 3 numbers.
// https://math.stackexchange.com/questions/93731/finding-the-gcd-of-three-numbers
// As others say, one way to do it is using the identity gcd(a,b,c)=gcd(a,(gcd(b,c)).
// This identity is true since the "gcd" is the maximal element of the intersection of 
// the sets of factors of the inputs.
s64 Gcd3(s64 A, s64 B, s64 C)
{
    s64 Result;
    Result = Gcd(A, Gcd(B, C));
    return(Result);
}

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

internal b32
IsNumber(u8 C)
{
    b32 Result;
    Result = C >= '0' && C <= '9';
    return(Result);
}

internal b32
IsChar(u8 C)
{
    b32 Result;
    Result =  
        (C >= 'a' && C <= 'z') ||
        (C >= 'A' && C <= 'Z');
    
    return(Result);
}

internal void
EatAllWhiteSpace(tokenizer *Tokenizer)
{
    while(Tokenizer->At[0] == ' ')
    {
        ++Tokenizer->At;
    }
}

internal u32
ConvertStringToU32(u8 *Text, u8 Length)
{
    u32 Result = 0;
    u32 Cof = 1;
    
    for(int Index = 0;
        Index < Length;
        ++Index)
    {
        char Value = Text[(Length - 1) - Index] - '0';
        Result += Value * Cof;
        Cof *= 10;
    }
    
    return(Result);
}

internal token
GetToken(tokenizer *Tokenizer)
{
    EatAllWhiteSpace(Tokenizer);
    
    u8 Char = Tokenizer->At[0];
    
    token Result;
    Result.Type = Char;
    Result.Text = Tokenizer->At;
    Result.TextLength = 1;
    Result.Value = 0;
    
    if(IsNumber(Char))
    {
        while(IsNumber(*Tokenizer->At))
        {
            
            ++Tokenizer->At;
        }
        
        Result.Type = Token_Type_Number;
        Result.TextLength = (u32)(Tokenizer->At - Result.Text);
        Result.Value = ConvertStringToU32(Result.Text, Result.TextLength);
    }
    else if(IsChar(Char))
    {
        while(IsChar(*Tokenizer->At) ||
              IsNumber(*Tokenizer->At))
        {
            ++Tokenizer->At;
        }
        
        Result.Type = Token_Type_Identifier;
        Result.TextLength = (u32)(Tokenizer->At - Result.Text);
    }
    else
    {
        ++Tokenizer->At;
    }
    
    return(Result);
}

internal b32
TokenEquals(tokenizer *Tokenizer, token_type Type)
{
    b32 Result = false;
    token Token = GetToken(Tokenizer);
    if(Token.Type == Type)
    {
        Result = true;
    }
    
    return(Result);
}

internal f32
Ceiling(f32 A)
{
    f32 Result;
    Result = ceilf(A);
    return(Result);
}
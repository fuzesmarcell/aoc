/*
====================================================================
$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2018 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/


#include "shared_advent.h"

loaded_file ReadEntireFileAndZeroTerminate(char *FileName)
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

void* PushArena_(memory_arena *Arena, memory_index Size)
{
    Assert((Arena->Index + Size) <= Arena->Size);
    
    void *Result;
    Result = (u8 *)Arena->Base + Arena->Index;
    Arena->Index += Size;
    return Result;
}

memory_arena InitMemoryArena(void *Base, memory_index Size)
{
    memory_arena Result;
    Result.Base = Base;
    Result.Size = Size;
    Result.Index = 0;
    
    return Result;
}

bmp_header InitBitmapHeader(s32 ImageWidth, s32 ImageHeight)
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

f32
V2Inner(v2 A, v2 B)
{
    f32 Result;
    Result = A.x * B.x + A.y * B.y;
    return Result;
}

v2
V2Sub(v2 A, v2 B)
{
    v2 Result;
    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    
    return Result;
}

v2
V2Add(v2 A, v2 B)
{
    v2 Result;
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    
    return Result;
}


f32
Cos(f32 A)
{
    f32 Result;
    Result = cosf(A);
    return (Result);
}

f32
Sin(f32 A)
{
    f32 Result;
    Result = sinf(A);
    return (Result);
}

f32
SquareRoot(f32 A)
{
    f32 Result;
    Result = sqrtf(A);
    return Result;
}

f32
Abs(f32 A)
{
    f32 Result;
    Result = fabsf(A);
    return(Result);
}

f32
Ceiling(f32 A)
{
    f32 Result;
    Result = ceilf(A);
    return(Result);
}

s64
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

s64 Gcd3(s64 A, s64 B, s64 C)
{
    s64 Result;
    Result = Gcd(A, Gcd(B, C));
    return(Result);
}


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

token
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

b32
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

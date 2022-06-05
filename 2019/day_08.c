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

#define WIDTH 25
#define HEIGHT 6

enum
{
    Pixel_Black,
    Pixel_White,
    Pixel_Transparent,
    Pixel_Invalid = 0xFF,
};

int main(int ArgCount, char **Args)
{
    if(ArgCount == 2)
    {
        loaded_file File = ReadEntireFileAndZeroTerminate(Args[1]);
        
        u8 *At = File.Contents;
        
        b32 Running = true;
        u32 LowesZeroDigitCounter = 25 * 9 + 1;
        u32 Result = 0;
        u32 Image[WIDTH * HEIGHT];
        
        for(u32 Index = 0;
            Index < (WIDTH * HEIGHT);
            ++Index)
        {
            Image[Index] = Pixel_Invalid;
        }
        
        while(Running)
        {
            u8 Token = (*At);
            
            if(Token == '\0')
            {
                Running = false;
            }
            else
            {
                u32 NumberZeroCounter = 0;
                u32 NumberOneCounter = 0;
                u32 NumberTwoCounter = 0;
                
                for(u32 Y = 0;
                    Y < HEIGHT;
                    ++Y)
                {
                    for(u32 X = 0;
                        X < WIDTH;
                        ++X)
                    {
                        u8 Value = ((*At++) - '0');
                        switch(Value)
                        {
                            case Pixel_Black:
                            case Pixel_White:
                            {
                                if(Image[((Y * WIDTH) + X)] == Pixel_Invalid)
                                {
                                    Image[((Y * WIDTH) + X)] = Value;
                                }
                                
                            } break;
                            
                            case Pixel_Transparent:
                            {
                                
                            } break;
                        }
                    }
                }
            }
        }
        
        for(u32 Y = 0;
            Y < HEIGHT;
            ++Y)
        {
            for(u32 X = 0;
                X < WIDTH;
                ++X)
            {
                printf("%d", Image[((Y * WIDTH) + X)]);
                
                if(Image[((Y * WIDTH) + X)] == Pixel_White)
                {
                    Image[((Y * WIDTH) + X)] = 0xFFFFFFFF;
                }
                else if(Image[((Y * WIDTH) + X)] == Pixel_Black)
                {
                    Image[((Y * WIDTH) + X)] = 0x0;
                }
                else
                {
                    Image[((Y * WIDTH) + X)] = 0xFF00FFFF;
                }
            }
            
            printf("\n");
        }
        
        bmp_header Header = InitBitmapHeader(WIDTH, HEIGHT);
        FILE *ResultFile = fopen("password.bmp", "wb");
        fwrite(&Header, sizeof(Header), 1, ResultFile);
        fwrite(Image, sizeof(Image), 1, ResultFile);
        fclose(ResultFile);
        
        return 0;
    }
}
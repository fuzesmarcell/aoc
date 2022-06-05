/*
====================================================================
$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2018 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    char *Contents;
    unsigned int Size;
} file_result;

enum
{
    Horizontal,
    Vertical,
};

enum
{
    Right = 'R',
    Left = 'L',
    Up = 'U',
    Down = 'D',
};

enum
{
    Pos_Min,
    Pos_Max,
};

typedef struct
{
    int MinX;
    int MinY;
    int MaxX;
    int MaxY;
    
    int Type;
    int Step;
    int From;
    
} line;

typedef struct
{
    int Count;
    line *Lines;
    
} line_array;

typedef struct
{
    int X;
    int Y;
} v2i;

static file_result
ReadEntireFileAndNullTerminate(char *FileName)
{
    file_result Result;
    
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

static unsigned int
ConvertStringToNumber(char **At, char *Text)
{
    unsigned int Result = 0;
    unsigned int Cof = 1;
    unsigned int Length = 0;
    while(!(*Text == ',' ||
            *Text == '\n' ||
            *Text == '\0'))
    {
        ++Length;
        ++Text;
    }
    
    *At = Text;
    Text = (*At - 1);
    
    for(int Index = 0;
        Index < Length;
        ++Index)
    {
        char Value = Text[-Index] - '0';
        Result += Value * Cof;
        Cof *= 10;
    }
    
    return Result;
}

static int
ParseLineInput(char **Parser, line_array *Array)
{
    char *At = *Parser;
    int Parsing = 1;
    int NumberOfLines = 0;
    int PreviousX = 0;
    int PreviousY = 0;
    while(Parsing)
    {
        char Token = *At++;
        switch(Token)
        {
            case '\0':
            case '\n':
            {
                Parsing = 0;
                *Parser = At;
            } break;
            
            case 'R':
            {
                unsigned int Number = ConvertStringToNumber(&At, At);
                ++NumberOfLines;
                
                if(Array)
                {
                    line *Line = Array->Lines + Array->Count++;
                    Line->Type = Horizontal;
                    Line->MinX = PreviousX;
                    Line->MinY = PreviousY;
                    
                    Line->MaxX = Line->MinX + (int)Number;
                    Line->MaxY = Line->MinY;
                    
                    PreviousX = Line->MaxX;
                    PreviousY = Line->MaxY;
                    
                    Line->Step = Number;
                    Line->From = Pos_Min;
                }
            } break;
            
            case 'L':
            {
                unsigned int Number = ConvertStringToNumber(&At, At);
                ++NumberOfLines;
                
                if(Array)
                {
                    line *Line = Array->Lines + Array->Count++;
                    Line->Type = Horizontal;
                    
                    Line->MaxX = PreviousX;
                    Line->MaxY = PreviousY;
                    
                    Line->MinX = Line->MaxX - (int)Number;
                    Line->MinY = Line->MaxY;
                    
                    PreviousX = Line->MinX;
                    PreviousY = Line->MinY;
                    
                    Line->Step = Number;
                    Line->From = Pos_Max;
                }
            } break;
            
            case 'U':
            {
                unsigned int Number = ConvertStringToNumber(&At, At);
                ++NumberOfLines;
                
                if(Array)
                {
                    line *Line = Array->Lines + Array->Count++;
                    Line->Type = Vertical;
                    Line->MinX = PreviousX;
                    Line->MinY = PreviousY;
                    
                    Line->MaxX = Line->MinX;
                    Line->MaxY = Line->MinY + (int)Number;
                    
                    PreviousX = Line->MaxX;
                    PreviousY = Line->MaxY;
                    
                    Line->Step = Number;
                    Line->From = Pos_Min;
                }
                
            } break;
            
            case 'D':
            {
                unsigned int Number = ConvertStringToNumber(&At, At);
                ++NumberOfLines;
                
                if(Array)
                {
                    line *Line = Array->Lines + Array->Count++;
                    Line->Type = Vertical;
                    
                    Line->MaxX = PreviousX;
                    Line->MaxY = PreviousY;
                    
                    Line->MinX = Line->MaxX;
                    Line->MinY = Line->MaxY - (int)Number;
                    
                    PreviousX = Line->MinX;
                    PreviousY = Line->MinY;
                    
                    Line->Step = Number;
                    Line->From = Pos_Max;
                }
                
            }  break;
        }
    }
    
    return NumberOfLines;
}

static v2i
GetFromPoint(line *Line)
{
    v2i Result;
    Result.X = Line->MaxX;
    Result.Y = Line->MaxY;
    if(Line->From == Pos_Min)
    {
        Result.X = Line->MinX;
        Result.Y = Line->MinY;
    }
    
    return Result;
}

static v2i
GetToPoint(line *Line)
{
    v2i Result;
    Result.X = Line->MinX;
    Result.Y = Line->MinY;
    if(Line->From == Pos_Min)
    {
        Result.X = Line->MaxX;
        Result.Y = Line->MaxY;
    }
    
    return Result;
}

static int
AddStepsIfVisited(line_array *Array, line *LineIn)
{
    int Result = 0;
    
    v2i ToPA = GetToPoint(LineIn);
    
    for(int Index = 0;
        Index < Array->Count;
        ++Index)
    {
        line *Line = Array->Lines + Index;
        
        v2i Point = GetToPoint(Line);
        
        if(Point.X == ToPA.X &&
           Point.Y == ToPA.Y)
        {
            Result = Line->Step;
            break;
        }
    }
    
    return Result;
}

int main(int ArgCount, char **Args)
{
    if(ArgCount == 2)
    {
        file_result File = ReadEntireFileAndNullTerminate(Args[1]);
        
        char *At = File.Contents;
        int LineACount = ParseLineInput(&At, 0);
        int LineBCount = ParseLineInput(&At, 0);
        
        line_array ArrayA;
        ArrayA.Count = 0;
        ArrayA.Lines = malloc(LineACount * sizeof(line));
        
        line_array ArrayB;
        ArrayB.Count = 0;
        ArrayB.Lines = malloc(LineBCount  * sizeof(line));
        
        At = File.Contents;
        ParseLineInput(&At, &ArrayA);
        ParseLineInput(&At, &ArrayB);
        
        int FewestSteps = 9999999;
        int StepsA = 0;
        for(int LineAIndex = 0;
            LineAIndex < ArrayA.Count;
            ++LineAIndex)
        {
            line *LineA = ArrayA.Lines + LineAIndex;
            
            StepsA += AddStepsIfVisited(&ArrayA, LineA);
            int StepsB = 0;
            
            for(int LineBIndex = 0;
                LineBIndex < ArrayB.Count;
                ++LineBIndex)
            {
                line *LineB = ArrayB.Lines + LineBIndex;
                
                StepsB += AddStepsIfVisited(&ArrayB, LineB);
                
                if(LineB->Type != LineA->Type)
                {
                    line *HorLine = LineA;
                    line *VerLine = LineB;
                    
                    if(LineA->Type == Vertical)
                    {
                        VerLine = LineA;
                        HorLine = LineB;
                    }
                    
                    if((HorLine->MinY >= VerLine->MinY) &&
                       (HorLine->MinY <= VerLine->MaxY) &&
                       (VerLine->MinX >= HorLine->MinX) &&
                       (VerLine->MaxX <= HorLine->MaxX))
                    {
                        int Sum = StepsA + StepsB;
                        if(VerLine->From == Pos_Min)
                        {
                            Sum -= abs(VerLine->MaxY - HorLine->MinY);
                        }
                        else
                        {
                            Sum -= abs(VerLine->MinY - HorLine->MinY);
                        }
                        
                        if(HorLine->From == Pos_Min)
                        {
                            Sum -= abs(HorLine->MaxX - VerLine->MinX);
                        }
                        else
                        {
                            Sum -= abs(HorLine->MinX - VerLine->MinX);
                        }
                        
                        if(LineAIndex != 0 && Sum < FewestSteps)
                        {
                            FewestSteps = Sum;
                        }
                    }
                }
            }
        }
        
        printf("Result: %d", FewestSteps);
    }
    
    return 0;
}
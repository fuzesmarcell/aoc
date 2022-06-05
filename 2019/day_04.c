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

int main()
{
    int Buffer[6] = {0};
    int DigitLength = 6;
    int AmountOfPasswords = 0;
    for(int Min = 134792;
        Min <= 675810;
        ++Min)
    {
        int Number = Min;
        int PreviousNumber = -1;
        int AdjacentCounter = 0;
        int AreAdjecentNotInGroup = 0;
        for(int Index = 0;
            Index < DigitLength;
            ++Index)
        {
            Buffer[Index] = Number % 10;
            
            if(PreviousNumber == Buffer[Index])
            {
                ++AdjacentCounter;
            }
            else
            {
                if(AdjacentCounter == 1)
                {
                    AreAdjecentNotInGroup = 1;
                }
                
                AdjacentCounter = 0;
            }
            
            Number /= 10;
            PreviousNumber = Buffer[Index];
        }
        
        if(AdjacentCounter == 1)
        {
            AreAdjecentNotInGroup = 1;
        }
        
        int IsIncreasing = 1;
        for(int Index = 0;
            Index < (DigitLength - 1);
            ++Index)
        {
            int A = Buffer[Index];
            int B = Buffer[Index + 1];
            
            if(A < B)
            {
                IsIncreasing = 0;
                break;
            }
        }
        
        if(IsIncreasing && AreAdjecentNotInGroup)
        {
            ++AmountOfPasswords;
        }
    }
    
    printf("%d", AmountOfPasswords);
    
    return 0;
}
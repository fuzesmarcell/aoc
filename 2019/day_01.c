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

#define ArrayCount(Array) (sizeof((Array))/sizeof((Array[0])))

static int MassInput[] = 
{
    142156, 108763, 77236, 78186,
    110145, 126414, 115436, 133275,
    132634, 82606, 118669, 90307,
    134124, 102597, 128607, 109214,
    50160, 72539, 99033, 145334,
    135409, 97525, 109865, 142319,
    79027, 96924, 72530, 85993,
    109594, 115991, 107998, 112934,
    85198, 112744, 129637, 95515,
    90804, 107052, 89707, 93658,
    60115, 118752, 94315, 59645,
    115668, 139320, 70734, 56771,
    74741, 69284, 92228, 145376,
    103317, 55143, 58370, 54873,
    52424, 95392, 67892, 90858,
    74693, 77363, 51496, 79375,
    71206, 103492, 94065, 72084,
    144311, 67381, 129958, 86741,
    148906, 123383, 147575, 136327,
    118108, 136529, 66356, 70746,
    147569, 107267, 122434, 69688,
    122127, 94072, 110203, 50546,
    57836, 139334, 113240, 96729,
    68516, 74635, 126951, 138948,
    88312, 101477, 129730, 93816
};

#define CalculateFuel(Mass) ((((Mass) / 3) - 2))

int main(int ArgCount, char **Args)
{
    int Result = 0;
    for(int Index = 0;
        Index < ArrayCount(MassInput);
        ++Index)
    {
        int Fuel = CalculateFuel(MassInput[Index]);
        while(Fuel > 0)
        {
            Result += Fuel;
            Fuel = CalculateFuel(Fuel);
        }
    }
    
    printf("%d", Result);
    
    return 0;
}
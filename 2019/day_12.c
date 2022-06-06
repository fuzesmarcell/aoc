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

typedef struct v3i v3i;
struct v3i
{
    s32 x, y, z;
};

typedef struct moon moon;
struct moon
{
    v3i P;
    v3i V;
};

#if 0
global moon Moons[] = 
{
    {{-1, 0, 2}},
    {{2, -10, -7}},
    {{4, -8, 8}},
    {{3, 5, -1}},
};
#endif

#if 0
global moon Moons[] = 
{
    {{-8, -10, 0}},
    {{5, 5, 10}},
    {{2, -7, 3}},
    {{9, -8, -3}},
};
#endif

global moon Moons[] = 
{
    {{5, 4, 4}},
    {{-11, -11, -3}},
    {{0, 7, 0}},
    {{-13, 2, 10}},
};

global moon MoonsInitialState[] = 
{
    {{5, 4, 4}},
    {{-11, -11, -3}},
    {{0, 7, 0}},
    {{-13, 2, 10}},
};

// LCM = 551,272,644,867,044

int main()
{
    s32 TimeStep = 0;
    
    s64 XPeriod = 0;
    s64 YPeriod = 0;
    s64 ZPeriod = 0;
    
    while(TimeStep < 1000000)
    {
        ++TimeStep;
        
        for(u32 I = 0;
            I < ArrayCount(Moons);
            ++I)
        {
            moon *A = Moons + I;
            for(u32 J = 0;
                J < ArrayCount(Moons);
                ++J)
            {
                if(J != I)
                {
                    moon *B = Moons + J;
                    if(A->P.x < B->P.x)
                    {
                        A->V.x += 1;
                    }
                    
                    if(A->P.x > B->P.x)
                    {
                        A->V.x -= 1;
                    }
                    
                    if(A->P.y < B->P.y)
                    {
                        A->V.y += 1;
                    }
                    
                    if(A->P.y > B->P.y)
                    {
                        A->V.y -= 1;
                    }
                    
                    if(A->P.z < B->P.z)
                    {
                        A->V.z += 1;
                    }
                    
                    if(A->P.z > B->P.z)
                    {
                        A->V.z -= 1;
                    }
                }
            }
        }
        
        //printf("After %d step(s):\n", TimeStep);
        b32 AllXEqual = true;
        b32 AllYEqual = true;
        b32 AllZEqual = true;
        for(u32 MoonIndex = 0;
            MoonIndex < ArrayCount(Moons);
            ++MoonIndex)
        {
            moon *Moon = Moons + MoonIndex;
            Moon->P.x += Moon->V.x;
            Moon->P.y += Moon->V.y;
            Moon->P.z += Moon->V.z;
            
#if 0            
            printf("pos=<x=%d, y=%d, z=%d>, vel=<x=%d, y=%d, z=%d>\n",
                   Moon->P.x, Moon->P.y, Moon->P.z,
                   Moon->V.x, Moon->V.y, Moon->V.z);
#endif
            //printf("(%d %d)", Moon->P.z, Moon->V.z);
            
            moon *MoonInit = MoonsInitialState + MoonIndex;
            if(Moon->P.x == MoonInit->P.x &&
               Moon->V.x == 0 &&
               AllXEqual)
            {
                AllXEqual = true;
            }
            else
            {
                AllXEqual = false;
            }
            
            if(Moon->P.y == MoonInit->P.y &&
               Moon->V.y == 0 &&
               AllYEqual)
            {
                AllYEqual = true;
            }
            else
            {
                AllYEqual = false;
            }
            
            if(Moon->P.z == MoonInit->P.z &&
               Moon->V.z == 0 &&
               AllZEqual)
            {
                AllZEqual = true;
            }
            else
            {
                AllZEqual = false;
            }
        }
        
        if(AllXEqual && XPeriod == 0)
        {
            XPeriod = TimeStep;
        }
        
        if(AllYEqual && YPeriod == 0)
        {
            YPeriod = TimeStep;
        }
        
        if(AllZEqual && ZPeriod == 0)
        {
            ZPeriod = TimeStep;
        }
        
        //printf("\n");
    }
    
    u32 Energy = 0;
    for(u32 MoonIndex = 0;
        MoonIndex < ArrayCount(Moons);
        ++MoonIndex)
    {
        moon *Moon = Moons + MoonIndex;
        
        u32 Pot = (u32)(Abs(Moon->P.x) + Abs(Moon->P.y) + Abs(Moon->P.z));
        u32 Kin = Abs(Moon->V.x) + Abs(Moon->V.y) + Abs(Moon->V.z);
        
        Energy += Pot * Kin;
        
    }
    
    printf("Total Energy in the system: %d\n", Energy);
    printf("Period X: %lld\n", (long long)XPeriod);
    printf("Period Y: %lld\n", (long long)YPeriod);
    printf("Period Z: %lld\n", (long long)ZPeriod);
    
    return(0);
}

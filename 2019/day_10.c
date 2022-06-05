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

typedef struct asteroid asteroid;
struct asteroid
{
    b32 Vaporized;
    v2 P;
};

#define MAX_ASTEROIDS 512
#define INVALID_ASTEROID_INDEX 0xFFFF
global asteroid Asteroids[MAX_ASTEROIDS];
global u32 AsteroidCount;

internal b32
IsPointOnLine(v2 A, v2 B, v2 P)
{
    b32 Result;
    Result = false;
    
    f32 Divisor = (A.x - B.x);
    f32 Dividend = (A.y - B.y);
    f32 m = 0;
    f32 c = 0;
    
    if(Divisor != 0)
    {
        m =  Dividend/ Divisor;
        c = (A.y - (m * A.x));
    }
    
    f32 LineTest = 0;
    if(Divisor == 0)
    {
        LineTest = P.x - B.x;
    }
    else
    {
        LineTest = m * P.x + c - P.y;
    }
    
    f32 Epsilon = 0.001f;
    if(LineTest <= Epsilon &&
       LineTest >= -Epsilon)
    {
        Result = true;
    }
    
    return (Result);
}

internal b32
IsPointOnLineSegment(v2 A, v2 B, v2 P)
{
    b32 Result;
    Result = false;
    
    if(IsPointOnLine(A, B, P))
    {
        v2 DA = V2Sub(A, B);
        v2 DB = V2Sub(B, A);
        if(V2Inner(V2Sub(P, B), DA) >= 0 &&
           V2Inner(V2Sub(P, A), DB) >= 0)
        {
            Result = true;
        }
    }
    
    return (Result);
}

internal f32
HitSphere(v2 Center, f32 Radius, v2 RayO, v2 RayD)
{
    f32 Result;
    v2 Oc = V2Sub(RayO, Center);
    f32 a = V2Inner(RayD, RayD);
    f32 b = 2.0f * V2Inner(Oc, RayD);
    f32 c = V2Inner(Oc, Oc) - Radius * Radius;
    f32 Discriminent = b*b - 4*a*c;
    if(Discriminent < 0)
    {
        Result = -1.0f;
    }
    else
    {
        Result = ((-b - SquareRoot(Discriminent)) / (2.0f *a));
    }
    
    return(Result);
}

int main(int ArgCount, char **Args)
{
    if(ArgCount == 2)
    {
        loaded_file File = ReadEntireFileAndZeroTerminate(Args[1]);
        
        u8 *At = File.Contents;
        
        b32 Parsing = true;
        v2 CurrentP = {0.0f};
        while(Parsing)
        {
            u8 Token = *At++;
            switch(Token)
            {
                case '\0':
                {
                    Parsing = false;
                } break;
                
                case '#':
                {
                    Assert(AsteroidCount + 1 < MAX_ASTEROIDS);
                    asteroid *Asteroid = Asteroids + AsteroidCount++;
                    Asteroid->P = CurrentP;
                    CurrentP.x += 1.0f;
                } break;
                
                case '.':
                {
                    CurrentP.x += 1.0f;
                } break;
                
                case '\n':
                {
                    CurrentP.y += 1.0f;
                    CurrentP.x = 0.0f;
                } break;
            }
        }
        
        // TODO(fuzes): We are just going to do the triple loop and see how
        // efficient it is and later we might revist it if needed.
        
        u32 Best = 0;
        v2 BestP = {0};
        u32 BestIndex = 0;
        for(u32 I = 0;
            I < AsteroidCount;
            ++I)
        {
            v2 Station = Asteroids[I].P;
            u32 Counter = 0;
            for(u32 J = 0;
                J < AsteroidCount;
                ++J)
            {
                if(I != J)
                {
                    b32 IsInSight = true;
                    v2 InSight = Asteroids[J].P;
                    
                    for(u32 K = 0;
                        K < AsteroidCount;
                        ++K)
                    {
                        if(K != I &&
                           K != J)
                        {
                            v2 Test = Asteroids[K].P;
                            if(IsPointOnLineSegment(InSight, Station, Test))
                            {
                                IsInSight = false;
                            }
                        }
                    }
                    
                    if(IsInSight)
                    {
                        ++Counter;
                    }
                }
            }
            
            if(Counter > Best)
            {
                Best = Counter;
                BestP = Station;
                BestIndex = I;
            }
        }
        
        printf("Best: (%.0f, %.0f) -> %d\n", BestP.x, BestP.y, Best);
        
        f32 Radian = -DegreeToRadian(90);
        v2 Position = BestP;
        u32 VaporizedCount = 9;
        while(VaporizedCount)
        {
            f32 ClosestT = 999999.0f;
            v2 Direction = {Cos(Radian), Sin(Radian)};
            u32 ClosestIndex = INVALID_ASTEROID_INDEX;
            for(u32 AsteroidIndex = 0;
                AsteroidIndex < AsteroidCount;
                ++AsteroidIndex)
            {
                if(AsteroidIndex != BestIndex)
                {
                    asteroid *Asteroid = Asteroids + AsteroidIndex;
                    if(!Asteroid->Vaporized)
                    {
                        f32 HitT = HitSphere(Asteroid->P, 0.05f, Position, Direction);
                        if(HitT > 0.0f &&
                           HitT < ClosestT)
                        {
                            ClosestT = HitT;
                            ClosestIndex = AsteroidIndex;
                        }
                    }
                }
            }
            
            if(ClosestIndex != INVALID_ASTEROID_INDEX)
            {
                asteroid *ClosesAsteroid = Asteroids + ClosestIndex;
                if(!ClosesAsteroid->Vaporized)
                {
                    printf("(%.0f, %.0f)\n", ClosesAsteroid->P.x, ClosesAsteroid->P.y);
                    ClosesAsteroid->Vaporized = true;
                    --VaporizedCount;
                }
            }
            
            Radian += DegreeToRadian(2.0f);
        }
    }
    
    return (0);
}
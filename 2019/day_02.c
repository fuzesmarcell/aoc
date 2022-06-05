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

enum
{
    OP_Add = 1,
    OP_Mul = 2,
    OP_End = 99,
};

int main(int ArgCount, char **Args)
{
    for(int NounIndex = 0;
        NounIndex < 100;
        ++NounIndex)
    {
        for(int VerbIndex = 0;
            VerbIndex < 100;
            ++VerbIndex)
        {
            int Input[] = {1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,6,1,19,1,19,5,23,2,9,23,27,1,5,27,31,1,5,31,35,1,35,13,39,1,39,9,43,1,5,43,47,1,47,6,51,1,51,13,55,1,55,9,59,1,59,13,63,2,63,13,67,1,67,10,71,1,71,6,75,2,10,75,79,2,10,79,83,1,5,83,87,2,6,87,91,1,91,6,95,1,95,13,99,2,99,13,103,1,103,9,107,1,10,107,111,2,111,13,115,1,10,115,119,1,10,119,123,2,13,123,127,2,6,127,131,1,13,131,135,1,135,2,139,1,139,6,0,99,2,0,14,0};
            Input[1] = NounIndex;
            Input[2] = VerbIndex;
            
            int Running = 1;
            int InstructionIndex = 0;
            while(Running)
            {
                int OpCode = Input[InstructionIndex++];
                
                switch(OpCode)
                {
                    case OP_Add:
                    {
                        int IndexA = Input[InstructionIndex++];
                        int IndexB = Input[InstructionIndex++];
                        int ResultIndex = Input[InstructionIndex++];
                        
                        int A = Input[IndexA];
                        int B = Input[IndexB];
                        
                        Input[ResultIndex] = A + B;
                        
                    } break;
                    
                    case OP_Mul:
                    {
                        int IndexA = Input[InstructionIndex++];
                        int IndexB = Input[InstructionIndex++];
                        int ResultIndex = Input[InstructionIndex++];
                        
                        int A = Input[IndexA];
                        int B = Input[IndexB];
                        
                        Input[ResultIndex] = A * B;
                        
                    } break;
                    
                    case OP_End:
                    {
                        Running = 0;
                    } break;
                    
                    default:
                    {
                        Running = 0;
                    } break;
                }
            }
            
            if(Input[0] == 19690720)
            {
                int Result = (100 * NounIndex) + VerbIndex;
                printf("%d", Result);
                return 0;
            }
        }
    }
    
    return 0;
}
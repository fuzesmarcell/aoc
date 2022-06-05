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

#include <stdio.h>

s32 ByteCode[] = {3,225,1,225,6,6,1100,1,238,225,104,0,1102,31,68,225,1001,13,87,224,1001,224,-118,224,4,224,102,8,223,223,1001,224,7,224,1,223,224,223,1,174,110,224,1001,224,-46,224,4,224,102,8,223,223,101,2,224,224,1,223,224,223,1101,13,60,224,101,-73,224,224,4,224,102,8,223,223,101,6,224,224,1,224,223,223,1101,87,72,225,101,47,84,224,101,-119,224,224,4,224,1002,223,8,223,1001,224,6,224,1,223,224,223,1101,76,31,225,1102,60,43,225,1102,45,31,225,1102,63,9,225,2,170,122,224,1001,224,-486,224,4,224,102,8,223,223,101,2,224,224,1,223,224,223,1102,29,17,224,101,-493,224,224,4,224,102,8,223,223,101,1,224,224,1,223,224,223,1102,52,54,225,1102,27,15,225,102,26,113,224,1001,224,-1560,224,4,224,102,8,223,223,101,7,224,224,1,223,224,223,1002,117,81,224,101,-3645,224,224,4,224,1002,223,8,223,101,6,224,224,1,223,224,223,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,8,226,677,224,102,2,223,223,1005,224,329,1001,223,1,223,1108,677,226,224,102,2,223,223,1006,224,344,101,1,223,223,108,677,226,224,102,2,223,223,1006,224,359,101,1,223,223,7,677,226,224,102,2,223,223,1005,224,374,101,1,223,223,1007,226,677,224,102,2,223,223,1005,224,389,101,1,223,223,8,677,677,224,102,2,223,223,1006,224,404,1001,223,1,223,1007,677,677,224,1002,223,2,223,1006,224,419,101,1,223,223,1108,677,677,224,1002,223,2,223,1005,224,434,1001,223,1,223,1107,226,677,224,102,2,223,223,1005,224,449,101,1,223,223,107,226,226,224,102,2,223,223,1006,224,464,101,1,223,223,1108,226,677,224,1002,223,2,223,1005,224,479,1001,223,1,223,7,677,677,224,102,2,223,223,1006,224,494,1001,223,1,223,1107,677,226,224,102,2,223,223,1005,224,509,101,1,223,223,107,677,677,224,1002,223,2,223,1006,224,524,101,1,223,223,1008,677,677,224,1002,223,2,223,1006,224,539,101,1,223,223,7,226,677,224,1002,223,2,223,1005,224,554,101,1,223,223,108,226,226,224,1002,223,2,223,1006,224,569,101,1,223,223,1008,226,677,224,102,2,223,223,1005,224,584,101,1,223,223,8,677,226,224,1002,223,2,223,1005,224,599,101,1,223,223,1007,226,226,224,1002,223,2,223,1005,224,614,101,1,223,223,1107,226,226,224,1002,223,2,223,1006,224,629,101,1,223,223,107,677,226,224,1002,223,2,223,1005,224,644,1001,223,1,223,1008,226,226,224,1002,223,2,223,1006,224,659,101,1,223,223,108,677,677,224,1002,223,2,223,1005,224,674,1001,223,1,223,4,223,99,226};

enum
{
    Op_Add = 1,
    Op_Mul,
    Op_In,
    Op_Out,
    Op_Jt,
    Op_Jf,
    Op_Less,
    Op_Eq,
    
    Op_Terminate = 99,
};

enum
{
    Mode_Position,
    Mode_Intermediate,
};

typedef struct
{
    u8 OpCode;
    u8 Mode[3];
} instruction;

typedef struct
{
    s32 *ByteCode;
    u32 Index;
} context;


internal instruction
GetNextInstruction(context *Context)
{
    instruction Result = {0};
    s32 Number = Context->ByteCode[Context->Index++];
    u32 Counter = 0;
    u32 ModeIndex = 0;
    u32 Cof = 1;
    do
    {
        ++Counter;
        s32 Digit = Number % 10;
        Number /= 10;
        
        if(Counter <= 2)
        {
            Result.OpCode += Digit * Cof;
            Cof *= 10;
        }
        else
        {
            Result.Mode[ModeIndex++] = Digit;
        }
        
    } while(Number > 0);
    
    return Result;
}

internal void
GetParameterValues(context *Context, s32 *Parameters, u32 Length, instruction *Instruction)
{
    for(u32 Index = 0;
        Index < Length;
        ++Index)
    {
        switch(Instruction->Mode[Index])
        {
            case Mode_Intermediate:
            {
                Parameters[Index] = Context->ByteCode[Context->Index++];
            } break;
            
            case Mode_Position:
            {
                Parameters[Index] = Context->ByteCode[Context->ByteCode[Context->Index++]];
            } break;
        }
    }
}

int main()
{
    context Context;
    Context.ByteCode = ByteCode;
    Context.Index = 0;
    
    s32 Input[1] = {5};
    u32 InputCount = 0;
    
    b32 Running = true;
    while(Running)
    {
        instruction Instruction = GetNextInstruction(&Context);
        
        switch(Instruction.OpCode)
        {
            case Op_Add:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(&Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                Context.ByteCode[Context.ByteCode[Context.Index++]] = Parameters[0] + Parameters[1];
            } break;
            
            case Op_Mul:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(&Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                Context.ByteCode[Context.ByteCode[Context.Index++]] = Parameters[0] * Parameters[1];
            } break;
            
            case Op_Jt:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(&Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0])
                {
                    Context.Index = Parameters[1];
                }
                
            } break;
            
            case Op_Jf:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(&Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0] == 0)
                {
                    Context.Index = Parameters[1];
                }
                
            } break;
            
            case Op_Less:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(&Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0] < Parameters[1])
                {
                    Context.ByteCode[Context.ByteCode[Context.Index++]] = 1;
                }
                else
                {
                    Context.ByteCode[Context.ByteCode[Context.Index++]] = 0;
                }
                
            } break;
            
            case Op_Eq:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(&Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0] == Parameters[1])
                {
                    Context.ByteCode[Context.ByteCode[Context.Index++]] = 1;
                }
                else
                {
                    Context.ByteCode[Context.ByteCode[Context.Index++]] = 0;
                }
                
            } break;
            
            case Op_In:
            {
                s32 InputValue = Input[InputCount++];
                s32 Position = Context.ByteCode[Context.Index++];
                Context.ByteCode[Position] = InputValue;
                
            } break;
            
            case Op_Out:
            {
                if(Instruction.Mode[0] == Mode_Intermediate)
                {
                    s32 Value = Context.ByteCode[Context.Index++];
                    printf("%d\n", Value);
                }
                else
                {
                    s32 Position = Context.ByteCode[Context.Index++];
                    s32 Value = Context.ByteCode[Position];
                    printf("%d\n", Value);
                }
                
            } break;
            
            case Op_Terminate:
            {
                Running = false;
            } break;
            
            InvalidDefaultCase;
        }
    }
    
    return 0;
}
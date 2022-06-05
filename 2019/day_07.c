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

#include <string.h>

s32 IntCode[] = {3,8,1001,8,10,8,105,1,0,0,21,38,63,72,85,110,191,272,353,434,99999,3,9,102,4,9,9,101,2,9,9,102,3,9,9,4,9,99,3,9,1001,9,4,9,102,2,9,9,1001,9,5,9,1002,9,5,9,101,3,9,9,4,9,99,3,9,1001,9,2,9,4,9,99,3,9,1001,9,3,9,102,2,9,9,4,9,99,3,9,101,2,9,9,102,2,9,9,1001,9,2,9,1002,9,4,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,99,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,1001,9,1,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,2,9,9,4,9,99};

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

typedef struct instruction instruction;
struct instruction
{
    u8 OpCode;
    u8 Mode[3];
};

typedef struct context context;
struct context
{
    s32 *ByteCode;
    u32 Index;
};

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

#define AMPLIFIER_INPUT_COUNT 2

typedef enum intcode_event intcode_event;
enum intcode_event
{
    Event_Type_Halt,
    Event_Type_Output,
};

typedef struct intcode_result intcode_result;
struct intcode_result
{
    intcode_event Event;
    s32 Signal;
};

internal intcode_result
RunIntCode(context *Context, s32 Phase, s32 InputSignal)
{
    intcode_result Result;
    Result.Event = Event_Type_Halt;
    Result.Signal = 0;
    
    s32 Input[AMPLIFIER_INPUT_COUNT] = {Phase, InputSignal};
    u32 InputCount = 0;
    
    b32 Running = true;
    while(Running)
    {
        instruction Instruction = GetNextInstruction(Context);
        
        switch(Instruction.OpCode)
        {
            case Op_Add:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                Context->ByteCode[Context->ByteCode[Context->Index++]] = Parameters[0] + Parameters[1];
            } break;
            
            case Op_Mul:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                Context->ByteCode[Context->ByteCode[Context->Index++]] = Parameters[0] * Parameters[1];
            } break;
            
            case Op_Jt:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0])
                {
                    Context->Index = Parameters[1];
                }
                
            } break;
            
            case Op_Jf:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0] == 0)
                {
                    Context->Index = Parameters[1];
                }
                
            } break;
            
            case Op_Less:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0] < Parameters[1])
                {
                    Context->ByteCode[Context->ByteCode[Context->Index++]] = 1;
                }
                else
                {
                    Context->ByteCode[Context->ByteCode[Context->Index++]] = 0;
                }
                
            } break;
            
            case Op_Eq:
            {
                s32 Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0] == Parameters[1])
                {
                    Context->ByteCode[Context->ByteCode[Context->Index++]] = 1;
                }
                else
                {
                    Context->ByteCode[Context->ByteCode[Context->Index++]] = 0;
                }
                
            } break;
            
            case Op_In:
            {
                s32 InputValue = Input[InputCount++];
                s32 Position = Context->ByteCode[Context->Index++];
                Context->ByteCode[Position] = InputValue;
                
            } break;
            
            case Op_Out:
            {
                s32 Value = 0;
                if(Instruction.Mode[0] == Mode_Intermediate)
                {
                    Value = Context->ByteCode[Context->Index++];
                }
                else
                {
                    s32 Position = Context->ByteCode[Context->Index++];
                    Value = Context->ByteCode[Position];
                }
                
                Result.Event = Event_Type_Output;
                Result.Signal = Value;
                Running = false;
                
            } break;
            
            case Op_Terminate:
            {
                Result.Event = Event_Type_Halt;
                Running = false;
            } break;
            
            InvalidDefaultCase;
        }
    }
    
    return Result;
}

internal void
LoadProgramIntoMemory(context *Context, s32 *ByteCode, memory_index Size)
{
    Context->ByteCode = malloc(Size);
    memcpy(Context->ByteCode, ByteCode, Size);
}

internal void
ResetContext(context *Context, s32 *ByteCode, memory_index Size)
{
    Context->Index = 0;
    memcpy(Context->ByteCode, ByteCode, Size);
}

internal void
Swap(u8 *A, u8 *B)
{
    u8 Tmp;
    Tmp = *A;
    *A = *B;
    *B = Tmp;
}

#define SEQUENCE_LENGTH 5

internal void
Permute(u8 *Sequence, u32 Start, u32 End, s32 *InputSignal, context *Amplifiers)
{
    if(Start == End)
    {
        for(u32 Index = 0;
            Index < SEQUENCE_LENGTH;
            ++Index)
        {
            context *Context = Amplifiers + Index;
            ResetContext(Context, IntCode, sizeof(IntCode));
        }
        
        s32 Signal = 0;
        
        b32 IsRunning = true;
        b32 InitPhase = true;
        while(IsRunning)
        {
            for(u32 PhaseIndex = 0;
                PhaseIndex < SEQUENCE_LENGTH;
                ++PhaseIndex)
            {
                context *Context = Amplifiers + PhaseIndex;
                intcode_result IntResult;
                if(InitPhase)
                {
                    IntResult = RunIntCode(Context, Sequence[PhaseIndex], Signal);
                }
                else
                {
                    IntResult = RunIntCode(Context, Signal, 0);
                }
                
                switch(IntResult.Event)
                {
                    case Event_Type_Halt:
                    {
                        // NOTE(fuzes): Indicates that the last amplifier is also halting
                        // thus it is the end of our run.
                        if(PhaseIndex == 4)
                        {
                            IsRunning = false;
                        }
                        
                    } break;
                    
                    case Event_Type_Output:
                    {
                        Signal = IntResult.Signal;
                    }
                }
            }
            
            InitPhase = false;
        }
        
        if(Signal > *InputSignal)
        {
            *InputSignal = Signal;
        }
    }
    else
    {
        for(u32 Index = Start;
            Index <= End;
            ++Index)
        {
            Swap((Sequence + Start), (Sequence + Index));
            Permute(Sequence, Start + 1, End, InputSignal, Amplifiers);
            Swap((Sequence + Start), (Sequence + Index));
        }
    }
}

int main()
{
    context Amplifiers[SEQUENCE_LENGTH];
    
    for(u32 Index = 0;
        Index < ArrayCount(Amplifiers);
        ++Index)
    {
        context *Context = Amplifiers + Index;
        Context->Index = 0;
        LoadProgramIntoMemory(Context, IntCode, sizeof(IntCode));
    }
    
    u8 PhaseSequence[] = {5, 6, 7, 8, 9};
    s32 Result = 0;
    Permute(PhaseSequence, 0, ArrayCount(PhaseSequence) - 1, &Result, Amplifiers);
    
    printf("%d", Result);
    
    return 0;
}
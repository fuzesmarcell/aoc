/*
====================================================================
$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2018 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/

#pragma once

typedef enum opcode_type opcode_type;
enum opcode_type
{
    Op_Add = 1,
    Op_Mul,
    Op_In,
    Op_Out,
    Op_Jt,
    Op_Jf,
    Op_Less,
    Op_Eq,
    Op_AdjRelBase,
    
    Op_Terminate = 99,
};

typedef enum mode_type mode_type;
enum mode_type
{
    Mode_Position,
    Mode_Intermediate,
    Mode_Relative,
};

typedef struct instruction instruction;
struct instruction
{
    opcode_type OpCode;
    mode_type Mode[3];
};

typedef struct context context;
struct context
{
    s64 *ByteCode;
    u32 Index;
    s32 RelativeBase;
};

typedef enum intcode_event intcode_event;
enum intcode_event
{
    Event_Type_Halt,
    Event_Type_Output,
};

#define MAX_OUTPUTS 10

typedef struct intcode_result intcode_result;
struct intcode_result
{
    intcode_event Event;
    s64 Outputs[MAX_OUTPUTS];
    u32 NumberOfOutputs;
};

internal instruction
GetNextInstruction(context *Context)
{
    instruction Result = {0};
    s64 Number = Context->ByteCode[Context->Index++];
    
    Result.OpCode = Number % 100;
    Number /= 100;
    
    u32 ModeIndex = 0;
    do
    {
        Result.Mode[ModeIndex++] = Number % 10;;
        Number /= 10;
        
    } while(Number > 0);
    
    return Result;
}

typedef struct parameter parameter;
struct parameter
{
    s64 Value;
    s64 Index;
};

internal void
GetParameterValues(context *Context, parameter *Parameters, u32 Length, instruction *Instruction)
{
    for(u32 Index = 0;
        Index < Length;
        ++Index)
    {
        parameter *Param = Parameters + Index;
        switch(Instruction->Mode[Index])
        {
            case Mode_Intermediate:
            {
                Param->Index = Context->Index++;
                Param->Value = Context->ByteCode[Param->Index];
            } break;
            
            case Mode_Position:
            {
                Param->Index = Context->ByteCode[Context->Index++];
                Assert(Param->Index >= 0);
                Param->Value = Context->ByteCode[Param->Index];
            } break;
            
            case Mode_Relative:
            {
                Param->Index = Context->ByteCode[Context->Index++] + Context->RelativeBase;
                Assert(Param->Index >= 0);
                Param->Value = Context->ByteCode[Param->Index];
            } break;
        }
    }
}

internal intcode_result
RunIntCode(context *Context, s64 *Input, u32 InputLength, u32 OutputLength)
{
    intcode_result Result;
    Result.Event = Event_Type_Halt;
    Result.NumberOfOutputs = 0;
    
    u32 InputCount = 0;
    u32 OutputCount = 0;
    
    b32 Running = true;
    while(Running)
    {
        instruction Instruction = GetNextInstruction(Context);
        
        switch(Instruction.OpCode)
        {
            case Op_Add:
            {
                parameter Parameters[3] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                Context->ByteCode[Parameters[2].Index] = Parameters[0].Value + Parameters[1].Value;
            } break;
            
            case Op_Mul:
            {
                parameter Parameters[3] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                Context->ByteCode[Parameters[2].Index] = Parameters[0].Value * Parameters[1].Value;
            } break;
            
            case Op_Jt:
            {
                parameter Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0].Value)
                {
                    Context->Index = Parameters[1].Value;
                }
                
            } break;
            
            case Op_Jf:
            {
                parameter Parameters[2] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0].Value == 0)
                {
                    Context->Index = Parameters[1].Value;
                }
                
            } break;
            
            case Op_Less:
            {
                parameter Parameters[3] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0].Value < Parameters[1].Value)
                {
                    Context->ByteCode[Parameters[2].Index] = 1;
                }
                else
                {
                    Context->ByteCode[Parameters[2].Index] = 0;
                }
                
            } break;
            
            case Op_Eq:
            {
                parameter Parameters[3] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                if(Parameters[0].Value == Parameters[1].Value)
                {
                    Context->ByteCode[Parameters[2].Index] = 1;
                }
                else
                {
                    Context->ByteCode[Parameters[2].Index] = 0;
                }
                
            } break;
            
            case Op_In:
            {
                Assert((InputCount + 1) <= InputLength);
                
                s64 InputValue = Input[InputCount++];
                s64 Position = 0;
                if(Instruction.Mode[0] == Mode_Position)
                {
                    Position = Context->ByteCode[Context->Index++];
                }
                else if(Instruction.Mode[0] == Mode_Relative)
                {
                    Position = Context->ByteCode[Context->Index++] + Context->RelativeBase;
                }
                else
                {
                    InvalidCodePath;
                }
                
                Assert(Position >= 0);
                Context->ByteCode[Position] = InputValue;
                
            } break;
            
            case Op_Out:
            {
                parameter Parameters[1] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                ++OutputCount;
                
                Assert((Result.NumberOfOutputs + 1) < MAX_OUTPUTS);
                Result.Outputs[Result.NumberOfOutputs++] = Parameters[0].Value;
                
                if(OutputCount == OutputLength)
                {
                    Result.Event = Event_Type_Output;
                    Running = false;
                }
                
            } break;
            
            case Op_Terminate:
            {
                Result.Event = Event_Type_Halt;
                Running = false;
            } break;
            
            case Op_AdjRelBase:
            {
                parameter Parameters[1] = {0};
                GetParameterValues(Context, Parameters, ArrayCount(Parameters), &Instruction);
                
                Context->RelativeBase += Parameters[0].Value;
                
            } break;
            
            InvalidDefaultCase;
        }
    }
    
    return Result;
}

internal void
LoadProgramIntoMemory(context *Context, s64 *ByteCode, memory_index Size,
                      memory_index DataSegmentSize)
{
    memory_index MemorySize = Size + DataSegmentSize;
    Context->ByteCode = calloc(1, MemorySize);
    memcpy(Context->ByteCode, ByteCode, Size);
}

internal void
ResetContext(context *Context, s64 *ByteCode, memory_index Size)
{
    Context->Index = 0;
    memcpy(Context->ByteCode, ByteCode, Size);
}

internal context
InitContext(s64 *ByteCode, memory_index Size, memory_index DataSegmentSize)
{
    context Context;
    Context.ByteCode = 0;
    Context.Index = 0;
    Context.RelativeBase = 0;
    
    LoadProgramIntoMemory(&Context, ByteCode, Size, DataSegmentSize);
    
    return(Context);
}
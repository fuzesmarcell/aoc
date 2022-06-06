/*
====================================================================
$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2018 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/

#ifndef INTCODE_H
#define INTCODE_H

#include <string.h>

#include "shared_advent.h"

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

typedef struct parameter parameter;
struct parameter
{
    s64 Value;
    s64 Index;
};

extern intcode_result RunIntCode(context *Context, s64 *Input, u32 InputLength, u32 OutputLength);

extern void LoadProgramIntoMemory(context *Context, s64 *ByteCode, memory_index Size,
                                  memory_index DataSegmentSize);

extern void ResetContext(context *Context, s64 *ByteCode, memory_index Size);

extern context InitContext(s64 *ByteCode, memory_index Size, memory_index DataSegmentSize);

#endif // INTCODE_H

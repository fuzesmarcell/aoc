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

/*
176 ORE => 6 VJHF
139 ORE => 4 NVRVD
144 ORE => 7 JNWZP
145 ORE => 6 MNCFX
1 NVRVD => 8 CXFTF
1 VJHF, 6 MNCFX => 4 RFSQX
17 NVRVD, 3 JNWZP => 8 VPVL
22 VJHF, 37 MNCFX => 5 FWMGM
2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG
5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV
5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC
53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL
             145      1408
             
             
53 STKFG			 
106 VPVL, 371 FWMGM, 106 CXFTF, 583 MNCFX => (53 * 1) 53 STKFG 0 remainder
106 VPVL
238 NVRVD, 42 JNWZP => (14 * 8 = 112) 106 VPVL 6 remainder
238 NVRVD
8340 ORE => (60 * 4 = 240) 238 NVRVD 2 remainder
42 JNWZP
864 ORE => (6 * 7 = 42) 42 JNWZP 0 remainder
------------------------------------------------------------------
Result -> 864 + 8340 = 9204 ORE
------------------------------------------------------------------
******************************************************************
371 FWMGM
1650 VJHF, 2775 MNCFX => (75 * 5 = 375) 371 FWMGM 4 remainder
1650 VJHF
48400 ORE => (275 * 6 = 1650) 1650 VJHF 0 remainder
2775 MNCFX
67135 ORE => (463 * 6 = 2778) 2775 MNCFX 1 remainder
------------------------------------------------------------------
Result -> 48400 + 67135 = 115535 ORE
------------------------------------------------------------------
******************************************************************
106 CXFTF
14 NVRVD => (14 * 8 = 112) 106 CXFTF 6 remainder
14 NVRVD - 2 = 12 NVRV/* 2 remainder from above line 21
12 NVRVD
417 ORE => (3 * 4 = 12) 12 NVRVD 0 remainder
------------------------------------------------------------------
Result -> 417 = 417 ORE
------------------------------------------------------------------
******************************************************************
583 MNCFX - 1 remainder from line 33
582 MNCFX
14065 ORE => (97 * 6 = 582) 582 MNCFX 0 remainder
------------------------------------------------------------------
Result -> 14065 = 14065 ORE
------------------------------------------------------------------
******************************************************************

139221
*/

#define CHEMICAL_NAME_LENGTH 32
#define MAX_CHEMICAL_CHAIN_LENGTH 12
typedef struct reaction reaction;
struct reaction
{
    u32 Quantity;
    u32 Length;
    u8 Name[CHEMICAL_NAME_LENGTH];
};

typedef struct reaction_chain reaction_chain;
struct reaction_chain
{
    reaction Output;
    u32 Remainder;
    
    u32 InputCount;
    reaction Inputs[MAX_CHEMICAL_CHAIN_LENGTH];
};

#define MAX_AMOUNT_OF_CHAINS 100
global reaction_chain GlobalChain[MAX_AMOUNT_OF_CHAINS];
global u32 GlobalChainCount;

internal reaction
ParseReaction(tokenizer *Tokenizer, u32 Quantity)
{
    reaction Result = {0};
    
    Result.Quantity = Quantity;
    
    b32 Parsing = true;
    while(Parsing)
    {
        token Token = GetToken(Tokenizer);
        
        switch(Token.Type)
        {
            case ',':
            case '\n':
            case '=':
            {
                Parsing = false;
            } break;
            
            case '\0':
            {
                Parsing = false;
                Tokenizer->Exit = true;
            } break;
            
            case Token_Type_Identifier:
            {
                Result.Length = Token.TextLength;
                for(u32 Index = 0;
                    Index < Token.TextLength;
                    ++Index)
                {
                    Result.Name[Index] = Token.Text[Index];
                }
                
            } break;
        }
    }
    
    return(Result);
}

internal b32
CStringEuqualToLengthString(u8 *Name, u8 *Text, u32 TextLength)
{
    u8 *At = Name;
    for(u32 CharIndex = 0;
        CharIndex < TextLength;
        ++CharIndex, ++At)
    {
        if((*At == 0) ||
           (Text[CharIndex] != *At))
        {
            return(false);
        }
    }
    
    b32 Match = (*At == 0);
    
    return(Match);
}

internal b32
StringEqual(u8 *A, u32 ALength, u8 *B, u32 BLength)
{
    b32 Result = false;
    if(ALength == BLength)
    {
        Result = true;
        for(u32 Index = 0;
            Index < ALength;
            ++Index)
        {
            if(A[Index] != B[Index])
            {
                Result = false;
                break;
            }
        }
    }
    
    return(Result);
}

internal reaction_chain*
GetReactionChainFromOutput(u8 *Name)
{
    reaction_chain* Result = 0;
    for(u32 Index = 0;
        Index < GlobalChainCount;
        ++Index)
    {
        reaction_chain *Chain = GlobalChain + Index;
        
        if(CStringEuqualToLengthString(Name, Chain->Output.Name, Chain->Output.Length))
        {
            Result = Chain;
            break;
        }
    }
    
    return(Result);
}

internal reaction_chain*
GetReactionChainFromReaction(reaction *R)
{
    reaction_chain *Result = 0;
    for(u32 Index = 0;
        Index < GlobalChainCount;
        ++Index)
    {
        reaction_chain *Chain = GlobalChain + Index;
        if(StringEqual(R->Name, R->Length, Chain->Output.Name, Chain->Output.Length))
        {
            Result = Chain;
            break;
        }
    }
    
    return(Result);
}

internal u32
CalculateOre(reaction_chain *Chain, reaction *R, u32 Multiplier)
{
    u32 Result = 0;
    
    if(Chain->InputCount == 1 &&
       CStringEuqualToLengthString("ORE", Chain->Inputs[0].Name, Chain->Inputs[0].Length))
    {
        u32 N = R->Quantity;
        
        if(Chain->Remainder < N)
        {
            N = N - Chain->Remainder;
            Chain->Remainder = 0;
            
            u32 A = Chain->Output.Quantity;
            u32 O = Chain->Inputs[0].Quantity;
            u32 Cof = (((N - 1) / A) + 1);
            Result = (Cof * O);
            
            Chain->Remainder = (Cof * A) - N;
        }
        else if(Chain->Remainder == N)
        {
            Chain->Remainder = 0;
        }
        else
        {
            Chain->Remainder = Chain->Remainder - R->Quantity;
        }
    }
    else
    {
        u32 LoopCounter = 1;
        if(R)
        {
            if(Chain->Remainder < R->Quantity)
            {
                u32 Cof = (u32)(Ceiling((((f32)R->Quantity) / ((f32)Chain->Output.Quantity))));
                Chain->Remainder = (Cof * Chain->Output.Quantity) - R->Quantity;
                LoopCounter = Cof;
            }
            else if(Chain->Remainder == R->Quantity)
            {
                LoopCounter = 0;
                Chain->Remainder = 0;
            }
            else
            {
                Chain->Remainder = Chain->Remainder - R->Quantity;
                LoopCounter = 0;
            }
        }
        
        for(u32 InputIndex = 0;
            InputIndex < Chain->InputCount;
            ++InputIndex)
        {
            reaction *Reaction = Chain->Inputs + InputIndex;
            reaction_chain *ChainReaction = GetReactionChainFromReaction(Reaction);
            if(ChainReaction)
            {
                for(u32 Index = 0;
                    Index < LoopCounter;
                    ++Index)
                {
                    if(ChainReaction->InputCount == 1 &&
                       CStringEuqualToLengthString("ORE", ChainReaction->Inputs[0].Name, ChainReaction->Inputs[0].Length))
                    {
                        // TODO(fuzes): If we are doing this here why are we checking it above ?
                        Result += CalculateOre(ChainReaction, Reaction, Multiplier * Chain->Output.Quantity);
                    }
                    else
                    {
                        Result += CalculateOre(ChainReaction, Reaction, Multiplier * Reaction->Quantity);
                    }
                }
            }
        }
    }
    
    return(Result);
}

int main(int ArgCount, char **Args)
{
    if(ArgCount == 2)
    {
        loaded_file File = ReadEntireFileAndZeroTerminate(Args[1]);
        
        tokenizer Tokenizer;
        Tokenizer.At = File.Contents;
        Tokenizer.Exit = false;
        
        b32 Parsing = true;
        while(Parsing && (!Tokenizer.Exit))
        {
            token Token = GetToken(&Tokenizer);
            
            switch(Token.Type)
            {
                case '\0':
                {
                    Parsing = false;
                } break;
                
                case Token_Type_Number:
                {
                    reaction_chain *Chain = GlobalChain + GlobalChainCount++;
                    
                    token NextToken = Token;
                    do
                    {
                        Chain->Inputs[Chain->InputCount++] = ParseReaction(&Tokenizer, NextToken.Value);
                        NextToken = GetToken(&Tokenizer);
                        
                    } while(NextToken.Type != '>');
                    
                    token OutputNumberToken = GetToken(&Tokenizer);
                    Chain->Output = ParseReaction(&Tokenizer, OutputNumberToken.Value);
                    
                } break;
            }
        }
        
    }
    
#if 1    
    
    for(u32 ChainIndex = 0;
        ChainIndex < GlobalChainCount;
        ++ChainIndex)
    {
        reaction_chain *Chain = GlobalChain + ChainIndex;
        
        for(u32 InputIndex = 0;
            InputIndex < Chain->InputCount;
            ++InputIndex)
        {
            reaction *R = Chain->Inputs + InputIndex;
            printf("%d %.*s", R->Quantity, R->Length, R->Name);
            if(InputIndex != (Chain->InputCount - 1))
            {
                printf(", ");
            }
        }
        
        printf(" => %d %.*s\n", Chain->Output.Quantity, Chain->Output.Length, Chain->Output.Name);
    }
#endif
    
    reaction_chain *MyChain = GetReactionChainFromOutput("FUEL");
    u32 RequiredOre = CalculateOre(MyChain, 0, MyChain->Output.Quantity);
    
    printf("%d ORE for 1 FUEL\n", RequiredOre);
    
    return(0);
}
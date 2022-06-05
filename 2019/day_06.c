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

#define MAX_CHILDREN 8

typedef struct node node;
struct node
{
    u8 Name[3];
    
    u32 ChildCount;
    node *Children[MAX_CHILDREN];
    
    node *Parent;
};

typedef struct entry entry;
struct entry
{
    node *Node;
    entry *Next;
};

global entry NodeHashTable[2048];

internal b32
AreNodeNamesEqual(node *Node, u8 *Name, u32 NameLength)
{
    b32 AreEqual = true;
    for(u32 Index = 0;
        Index < NameLength;
        ++Index)
    {
        if(Node->Name[Index] != Name[Index])
        {
            AreEqual = false;
            break;
        }
    }
    
    return AreEqual;
}

internal node*
GetNodeFromHash(entry *HashTable, u32 Size, u8 *Name, u32 NameLength, memory_arena *Arena)
{
    u32 Hash = 0;
    for(u32 Index = 0;
        Index < NameLength;
        ++Index)
    {
        Hash += Name[Index];
    }
    
    u32 Index = Hash & (Size - 1);
    
    entry *Entry = HashTable + Index;
    node *Node = Entry->Node;
    if(Node == 0)
    {
        Node = PushStruct(Arena, node);
        for(u32 Index = 0;
            Index < NameLength;
            ++Index)
        {
            Node->Name[Index] = Name[Index];
        }
        
        Entry->Node = Node;
    }
    else
    {
        b32 FoundNode = false;
        for(entry *At = Entry;
            At;
            At = At->Next)
        {
            if(AreNodeNamesEqual(At->Node, Name, NameLength))
            {
                FoundNode = true;
                Node = At->Node;
                break;
            }
        }
        
        if(!FoundNode)
        {
            entry *NewEntry = PushStruct(Arena, entry);
            node *NewNode = PushStruct(Arena, node);
            NewEntry->Node = NewNode;
            
            for(u32 Index = 0;
                Index < NameLength;
                ++Index)
            {
                NewNode->Name[Index] = Name[Index];
            }
            
            if(Entry->Next)
            {
                entry *To = Entry->Next;
                Entry->Next = NewEntry;
                NewEntry->Next = To;
                Node = NewNode;
            }
            else
            {
                Entry->Next = NewEntry;
                Node = NewNode;
            }
        }
        
    }
    
    return Node;
}

internal b32
IsEndOfName(u8 Token)
{
    b32 Result;
    Result = (Token == '\0') ||
        (Token == '\n') ||
        (Token == ')');
    
    return Result;
}

internal u32
GetNodeNameLength(node *Node)
{
    u32 Length = 0;
    for(u32 Index = 0;
        Index < 3;
        ++Index)
    {
        if(Node->Name[Index] == '\0')
        {
            break;
        }
        ++Length;
    }
    
    return Length;
}

internal void
Traverse(node *Node, u32 *Counter, u32 Depth)
{
    *Counter += Depth;
    
    //printf("%.*s\n", GetNodeNameLength(Node), Node->Name);
    
    for(u32 ChildIndex = 0;
        ChildIndex < Node->ChildCount;
        ++ChildIndex)
    {
        node *Next = Node->Children[ChildIndex];
        Traverse(Next, Counter, Depth + 1);
    }
}

int main(int ArgCount, char **Args)
{
    if(ArgCount >= 2)
    {
        loaded_file File = ReadEntireFileAndZeroTerminate(Args[1]);
        memory_index Size = Megabytes(16);
        void *Memory = calloc(1, Size);
        memory_arena Arena = InitMemoryArena(Memory, Size);
        
        u8 *At = File.Contents;
        b32 Parsing = true;
        while(Parsing)
        {
            u8 Token = *At;
            
            if(Token == '\0')
            {
                Parsing = false;
            }
            else
            {
                u8 NameBuffer[3];
                u32 Index = 0;
                while(!IsEndOfName(*At))
                {
                    NameBuffer[Index++] = *At++;
                }
                
                ++At;
                
                node *Parent = GetNodeFromHash(NodeHashTable, ArrayCount(NodeHashTable),
                                               NameBuffer, Index, &Arena);
                Index = 0;
                while(!IsEndOfName(*At))
                {
                    NameBuffer[Index++] = *At++;
                }
                
                ++At;
                
                node *Child = GetNodeFromHash(NodeHashTable, ArrayCount(NodeHashTable),
                                              NameBuffer, Index, &Arena);
                
                Assert((Parent->ChildCount + 1) <= MAX_CHILDREN);
                Parent->Children[Parent->ChildCount++] = Child;
                Child->Parent = Parent;
            }
        }
        
        node *Root = GetNodeFromHash(NodeHashTable, ArrayCount(NodeHashTable),
                                     "COM", 3, &Arena);
        u32 CheckSum = 0;
        Traverse(Root, &CheckSum, 0);
        printf("Part1: %d\n", CheckSum);
        
        node *You = GetNodeFromHash(NodeHashTable, ArrayCount(NodeHashTable),
                                    "YOU", 3, &Arena);
        
        node *Santa = GetNodeFromHash(NodeHashTable, ArrayCount(NodeHashTable),
                                      "SAN", 3, &Arena);
        node *LCA = 0;
        u32 YouDistance = 0;
        u32 SantaDistance = 0;
        for(node *A = You->Parent;
            A;
            A = A->Parent)
        {
            SantaDistance = 0;
            
            for(node *B = Santa->Parent;
                B;
                B = B->Parent)
            {
                if(A == B)
                {
                    LCA = A;
                    break;
                }
                ++SantaDistance;
            }
            
            if(LCA)
            {
                printf("Part2: %d\n", YouDistance + SantaDistance);
                break;
            }
            
            ++YouDistance;
            
        }
    }
    
    return 0;
}
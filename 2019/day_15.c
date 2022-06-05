/*
====================================================================
$File: $
$Date: $
$Revision: $
$Createor: Fuzes Marcell $
$Notice: (C) Copyright 2019 by Fuzes Marcell, All Rights Reserved. $
====================================================================
*/

#include <windows.h>

#include "shared_advent.h"
#include "intcode.h"

typedef enum move_command_type move_command_type;
enum move_command_type
{
    Move_Command_North = 1,
    Move_Command_South,
    Move_Command_West,
    Move_Command_East,
    };

typedef enum robot_status_code_type robot_status_code_type;
enum robot_status_code_type
{
    Robot_Status_Code_HitWall,
    Robot_Status_Code_MovedSuccess,
    Robot_Status_Code_MoveSuccessOxygenSystem,
};

typedef enum area_type area_type;
enum area_type
{
    Area_Unexplored,
    Area_Empty,
    Area_Wall,
    Area_OxygenSystem,
    Area_Robot,
    };

#define TILE_MAP_WIDTH 7000
#define TILE_MAP_HEIGHT 7000
global area_type GlobalTileMap[TILE_MAP_HEIGHT * TILE_MAP_WIDTH];

global s64 GlobalIntCode[] = {3,1033,1008,1033,1,1032,1005,1032,31,1008,1033,2,1032,1005,1032,58,1008,1033,3,1032,1005,1032,81,1008,1033,4,1032,1005,1032,104,99,102,1,1034,1039,102,1,1036,1041,1001,1035,-1,1040,1008,1038,0,1043,102,-1,1043,1032,1,1037,1032,1042,1105,1,124,1002,1034,1,1039,1002,1036,1,1041,1001,1035,1,1040,1008,1038,0,1043,1,1037,1038,1042,1106,0,124,1001,1034,-1,1039,1008,1036,0,1041,1002,1035,1,1040,1001,1038,0,1043,102,1,1037,1042,1106,0,124,1001,1034,1,1039,1008,1036,0,1041,101,0,1035,1040,1002,1038,1,1043,1002,1037,1,1042,1006,1039,217,1006,1040,217,1008,1039,40,1032,1005,1032,217,1008,1040,40,1032,1005,1032,217,1008,1039,37,1032,1006,1032,165,1008,1040,33,1032,1006,1032,165,1102,1,2,1044,1105,1,224,2,1041,1043,1032,1006,1032,179,1102,1,1,1044,1106,0,224,1,1041,1043,1032,1006,1032,217,1,1042,1043,1032,1001,1032,-1,1032,1002,1032,39,1032,1,1032,1039,1032,101,-1,1032,1032,101,252,1032,211,1007,0,72,1044,1105,1,224,1101,0,0,1044,1105,1,224,1006,1044,247,101,0,1039,1034,1001,1040,0,1035,1001,1041,0,1036,102,1,1043,1038,1002,1042,1,1037,4,1044,1106,0,0,88,40,30,49,14,76,90,49,13,52,39,90,19,1,33,96,15,67,92,19,82,71,43,53,74,46,84,4,37,99,87,52,39,48,79,8,74,31,62,4,47,75,81,73,9,60,75,59,97,3,46,86,90,91,85,69,98,15,40,6,88,18,81,71,51,99,11,73,86,14,59,91,88,63,58,86,18,98,66,74,48,43,70,99,83,17,98,92,86,96,26,17,52,88,82,4,80,98,70,77,33,76,74,55,78,53,41,84,88,23,48,87,65,96,91,59,32,29,9,83,75,97,68,93,40,96,28,76,66,82,89,80,1,84,37,86,42,95,74,79,62,87,43,69,89,83,70,87,33,82,99,95,68,26,97,10,76,49,28,96,49,65,93,42,38,77,68,70,90,33,53,74,57,98,54,18,76,55,73,10,40,88,76,17,15,81,37,37,30,97,40,71,79,95,1,62,13,85,90,74,4,11,77,78,1,78,74,19,99,98,7,8,76,28,97,77,62,21,85,80,29,60,77,25,93,23,97,84,67,75,92,98,51,35,87,66,80,54,89,34,80,82,4,56,50,87,48,55,97,21,97,76,75,50,9,75,91,66,22,67,96,25,90,73,74,28,29,94,89,53,2,58,78,18,15,87,77,12,11,80,71,91,76,69,79,25,84,30,41,70,85,6,95,96,30,5,73,96,88,27,37,87,62,20,78,90,30,21,96,92,70,32,36,59,94,25,92,92,24,79,71,57,92,74,93,41,96,74,90,47,81,43,70,77,96,64,73,62,95,96,16,92,43,80,79,55,80,66,95,14,26,37,89,5,68,75,67,20,95,78,38,99,56,23,60,58,48,84,86,53,48,95,65,99,4,68,83,84,12,26,84,93,6,85,14,63,80,83,10,95,77,32,94,80,43,51,97,92,4,32,35,93,44,97,97,97,14,56,73,96,83,14,40,78,95,32,69,1,94,30,95,41,96,85,70,79,65,52,23,65,54,98,8,86,82,1,4,82,96,33,99,76,48,75,2,99,67,96,50,95,88,52,95,46,64,96,85,43,24,82,41,79,65,47,83,16,95,70,75,15,38,83,39,15,97,80,59,81,77,39,77,32,89,56,88,25,75,8,92,19,86,79,74,86,64,51,20,91,81,53,95,68,91,77,65,86,22,21,77,42,84,75,40,40,98,29,29,35,73,32,13,80,40,91,12,48,95,97,56,3,32,15,83,53,97,21,94,21,59,89,29,23,98,5,99,33,71,30,89,93,37,50,95,74,2,78,92,21,90,87,57,15,75,89,28,80,45,67,77,99,82,8,86,83,85,93,99,53,55,94,90,1,87,74,39,88,65,55,77,64,87,92,59,99,7,54,96,50,35,6,82,18,6,73,92,49,10,96,31,77,33,97,58,94,40,45,14,90,75,66,14,58,79,24,32,58,95,82,89,49,87,31,63,90,42,96,36,73,16,77,5,81,99,35,80,87,13,71,79,15,92,8,51,92,88,20,95,30,89,86,80,98,60,99,43,90,23,58,90,43,87,83,33,83,90,33,93,75,31,91,80,57,15,97,47,94,94,44,49,59,77,83,4,67,75,19,13,62,89,4,61,96,70,41,61,87,73,43,99,68,18,89,13,71,76,75,6,25,19,96,89,28,89,58,8,92,44,77,81,37,5,92,82,33,81,90,20,91,93,15,28,92,89,76,61,73,44,95,57,83,94,78,42,79,47,75,89,81,15,87,13,86,45,89,74,97,37,78,87,96,59,80,33,87,60,86,66,80,52,94,0,0,21,21,1,10,1,0,0,0,0,0,0};

internal move_command_type
GetNextMove(move_command_type Move)
{
    move_command_type Result = Move + 1;
    
    if(Move == Move_Command_East)
    {
        Result = Move_Command_North;
    }
    
    return(Result);
}

#define SetTile(X, Y, Value) (GlobalTileMap[(Y) * TILE_MAP_WIDTH + (X)] = (Value))
#define GetTile(X, Y) (GlobalTileMap[(Y) * TILE_MAP_WIDTH + (X)])

internal void
PrintTileMapArea(s32 MinX, s32 MinY, s32 MaxX, s32 MaxY, s32 RX, s32 RY)
{
    for(u32 Y = MinY;
        Y <= MaxY;
        ++Y)
    {
        for(u32 X = MinX;
            X <= MaxX;
            ++X)
        {
            if(X == RX && Y == RY)
            {
                printf("D");
            }
            else
            {
                area_type Type = GetTile(X, Y);
                switch(Type)
                {
                    case Area_Empty:
                    {
                        printf(".");
                    } break;
                    
                    case Area_OxygenSystem:
                    {
                        printf("O");
                    } break;
                    
                    case Area_Wall:
                    {
                        printf("#");
                    } break;
                    
                    case Area_Unexplored:
                    {
                        printf(" ");
                    } break;
                }
            }
            }
        
        printf("\n");
    }
}

internal area_type
GetAreaFromRobot(context *Context, move_command_type MoveCommand)
{
    area_type Result;
    s64 Input[1] = {MoveCommand};
    intcode_result IntCodeResult = RunIntCode(&Context, Input, ArrayCount(Input), 1);
    Result = IntCodeResult.Outputs[0];
    
    return(Result);
}

internal void
AOCFloodFill(context *Context, s32 X, s32 Y, 
          area_type NodeType, area_type TargetArea, area_type ReplacementArea)
{
    SetTile(X, Y, NodeType);
    
    if(TargetArea == ReplacementArea)
    {
        return;
    }
    else if(NodeType != TargetArea)
    {
        return;
    }
    else
{
        AOCFloodFill(Context, X, Y - 1, GetAreaFromRobot(Context, Move_Command_South), TargetArea, ReplacementArea);
        GetAreaFromRobot(Context, Move_Command_North);
        
        AOCFloodFill(Context, X, Y + 1, GetAreaFromRobot(Context, Move_Command_North), TargetArea, ReplacementArea);
        GetAreaFromRobot(Context, Move_Command_South);
        
        AOCFloodFill(Context, X - 1, Y, GetAreaFromRobot(Context, Move_Command_West), TargetArea, ReplacementArea);
        GetAreaFromRobot(Context, Move_Command_East);
        
        AOCFloodFill(Context, X + 1, Y, GetAreaFromRobot(Context, Move_Command_East), TargetArea, ReplacementArea);
        GetAreaFromRobot(Context, Move_Command_West);
}
}

int main(int ArgCount, char **Args)
{
    context Context = InitContext(GlobalIntCode, sizeof(GlobalIntCode), sizeof(s64) * 512);
    
    s32 RobotX = TILE_MAP_WIDTH / 2;
    s32 RobotY = TILE_MAP_HEIGHT / 2;
    
    s32 MinX = RobotX;
    s32 MinY = RobotY;
    
    s32 MaxX = RobotX;
    s32 MaxY = RobotY;
    
    GlobalTileMap[RobotY * TILE_MAP_WIDTH + RobotX] = Area_Empty;
    
    b32 Running = true;
    move_command_type MoveType = Move_Command_North;
    s64 Input[1] = {0};
    u32 Tick = 0;
    while(Running)
    {
        Input[0] = (s64)MoveType;
        
        s32 RequestedX = RobotX;
        s32 RequestedY = RobotY;
        
        switch(MoveType)
        {
            case Move_Command_North:
            {
                ++RequestedY;
            } break;
            
            case Move_Command_South:
            {
                --RequestedY;
            } break;
            
            case Move_Command_East:
            {
                ++RequestedX;
            } break;
            
            case Move_Command_West:
            {
                --RequestedX;
            } break;
            }
        
        if(RequestedY < MinY)
        {
            MinY = RequestedY;
        }
        
        if(RequestedY > MaxY)
        {
            MaxY = RequestedY;
        }
        
        if(RequestedX < MinX)
        {
            MinX = RequestedX;
        }
        
        if(RequestedX > MaxX)
        {
            MaxX = RequestedX;
        }
        
        intcode_result Result = RunIntCode(&Context, Input, ArrayCount(Input), 1);
        
        switch(Result.Event)
        {
            case Event_Type_Halt:
{
                Running = false;
            } break;
            
            case Event_Type_Output:
            {
                robot_status_code_type Status = Result.Outputs[0];
                switch(Status)
                {
                    case Robot_Status_Code_HitWall:
                    {
                        SetTile(RequestedX, RequestedY, Area_Wall);
                        
                        if(GetTile(RobotX + 1, RobotY) == Area_Unexplored)
                        {
                            MoveType = Move_Command_East;
                        }
                        else if(GetTile(RobotX - 1, RobotY) == Area_Unexplored)
                        {
                            MoveType = Move_Command_West;
                        }
                        else if(GetTile(RobotX, RobotY + 1) == Area_Unexplored)
                        {
                            MoveType = Move_Command_North;
                        }
                        else if(GetTile(RobotX, RobotY - 1) == Area_Unexplored)
                        {
                            MoveType = Move_Command_South;
                        }
                        else
                        {
                            // TODO(fuzes): We could still check in which direction
                            // the area is empty on go there not just cycyle trough
                            // the move commands.
                            MoveType = GetNextMove(MoveType);
                        }
                        
                    } break;
                    
                    case Robot_Status_Code_MovedSuccess:
                    {
                        SetTile(RequestedX, RequestedY, Area_Empty);
                        RobotX = RequestedX;
                        RobotY = RequestedY;
                        
                        if(GetTile(RobotX + 1, RobotY) == Area_Unexplored)
                        {
                            MoveType = Move_Command_East;
                        }
                        else if(GetTile(RobotX - 1, RobotY) == Area_Unexplored)
                        {
                            MoveType = Move_Command_West;
                        }
                        else if(GetTile(RobotX, RobotY + 1) == Area_Unexplored)
                        {
                            MoveType = Move_Command_North;
                        }
                        else if(GetTile(RobotX, RobotY - 1) == Area_Unexplored)
                        {
                            MoveType = Move_Command_South;
                        }
                        
                    } break;
                    
                    case Robot_Status_Code_MoveSuccessOxygenSystem:
                    {
                        SetTile(RequestedX, RequestedY, Area_OxygenSystem);
                        Running = false;
                    } break;
                }
            } break;
        }
        
        ++Tick;
        
        if(Tick % 250 == 0 && Tick != 0)
        {
            printf("--------------%d--------------\n", Tick);
            PrintTileMapArea(MinX, MinY, MaxX, MaxY, RobotX, RobotY);
            printf("--------------%d--------------\n", Tick);
            printf("\n\n");
            Sleep(750);
        }
        }
    
    return(0);
}
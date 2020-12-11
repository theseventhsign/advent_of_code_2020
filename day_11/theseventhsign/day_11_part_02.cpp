#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define Assert assert
#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

#define S32Min INT32_MIN

typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;

typedef uint32_t u32;
typedef uint64_t u64;

typedef uintptr_t umm;

enum seat_type
{
    SeatType_Null,

    SeatType_Floor,
    SeatType_Empty,
    SeatType_Occupied,
};
struct seat_layout
{
    int Width;
    int Height;
    seat_type *Seats;
};

struct v2i
{
    int x, y;
};

inline v2i
V2i(int X, int Y)
{
    v2i Result;
    Result.x = X;
    Result.y = Y;

    return(Result);
}

inline v2i
operator+(v2i A, v2i B)
{
    v2i Result;
    Result.x = (A.x + B.x);
    Result.y = (A.y + B.y);

    return(Result);
}

inline void
operator+=(v2i &A, v2i B)
{
    A = A + B;
}

static seat_layout *
MakeSeatLayout(void)
{
    seat_layout *Layout = (seat_layout *)malloc(sizeof(seat_layout));
    *Layout = {};
    Layout->Seats = (seat_type *)malloc(sizeof(seat_type)*16384);
    return(Layout);
}

static void
CopySeats(seat_layout *Source, seat_layout *Dest)
{
    // @Hardcoded
    memcpy(Dest->Seats, Source->Seats, sizeof(seat_type)*16384);
}

static seat_layout *
Duplicate(seat_layout *Layout)
{
    seat_layout *NewLayout = MakeSeatLayout();
    NewLayout->Width = Layout->Width;
    NewLayout->Height = Layout->Height;
    CopySeats(Layout, NewLayout);
    return(NewLayout);
}

static int
AddRow(seat_layout *Layout)
{
    int LatestRowIndex = Layout->Height++;

    return(LatestRowIndex);
}

static seat_type *
GetSeat(seat_layout *Layout, int X, int Y)
{
    Assert((0 <= X) && (X < Layout->Width));
    Assert((0 <= Y) && (Y < Layout->Height));
    seat_type *Seat = (Layout->Seats +
                       X +
                       Y*Layout->Width);
    return(Seat);
}

static void
SetSeat(seat_layout *Layout, int X, int Y, seat_type Type)
{
    seat_type *Seat = GetSeat(Layout, X, Y);
    *Seat = Type;
}

static void
SetFloor(seat_layout *Layout, int X, int Y)
{
    SetSeat(Layout, X, Y, SeatType_Floor);
}

static void
SetEmpty(seat_layout *Layout, int X, int Y)
{
    SetSeat(Layout, X, Y, SeatType_Empty);
}

static void
SetOccupied(seat_layout *Layout, int X, int Y)
{
    SetSeat(Layout, X, Y, SeatType_Occupied);
}

static b32
IsOccupied(seat_layout *Layout, int X, int Y)
{
    b32 Result = false;
    if(((0 <= X) && (X < Layout->Width)) &&
       ((0 <= Y) && (Y < Layout->Height)))
    {
        seat_type *Seat = GetSeat(Layout, X, Y);
        Result = (*Seat == SeatType_Occupied);
    }
    return(Result);
}

static int
GetOccupiedCount(seat_layout *Layout)
{
    int Result = 0;
    for(int Y = 0;
        Y < Layout->Height;
        ++Y)
    {
        for(int X = 0;
            X < Layout->Width;
            ++X)
        {
            seat_type *Seat = GetSeat(Layout, X, Y);
            if(*Seat == SeatType_Occupied)
            {
                Result++;
            }
        }
    }
    return(Result);
}

static void
PrintSeats(seat_layout *Layout)
{
    for(int Y = 0;
        Y < Layout->Height;
        ++Y)
    {
        for(int X = 0;
            X < Layout->Width;
            ++X)
        {
            seat_type *Seat = GetSeat(Layout, X, Y);
            switch(*Seat)
            {
                case SeatType_Floor:
                {
                    fprintf(stdout, ".");
                } break;

                case SeatType_Empty:
                {
                    fprintf(stdout, "L");
                } break;

                case SeatType_Occupied:
                {
                    fprintf(stdout, "#");
                } break;

                InvalidDefaultCase;
            }
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

inline b32
IsInBounds(seat_layout *Layout, v2i P)
{
    b32 Result = (((0 <= P.x) && (P.x < Layout->Width)) &&
                  ((0 <= P.y) && (P.y < Layout->Height)));
    return(Result);
}

static seat_type 
FindFirstSeat(seat_layout *Layout,
              int StartX, int StartY,
              v2i Step)
{
    seat_type Result = SeatType_Null;

    v2i P = V2i(StartX, StartY);
    for(;;)
    {
        P += Step;
        if(IsInBounds(Layout, P))
        {
            seat_type *Seat = GetSeat(Layout, P.x, P.y);
            if(*Seat != SeatType_Floor)
            {
                Assert(*Seat != SeatType_Null);
                Result = *Seat;
                break;
            }
        }
        else
        {
            break;
        }
    }

    return(Result);
}

int
main(int ArgCount, char **Args)
{
    FILE *File = fopen("../input/day_11.txt", "rb");
    if(File)
    {
        seat_layout *Layout = MakeSeatLayout();

        {
            char RowString[128];
            Assert(fscanf(File, "%s\n", &RowString) == 1);
            Layout->Width = strlen(RowString);
            fseek(File, 0, SEEK_SET);
        }
        {
            char RowString[128];
            while(fscanf(File, "%s\n", &RowString) == 1)
            {
                int LatestRowIndex = AddRow(Layout);

                for(int Index = 0;
                    Index < strlen(RowString);
                    ++Index)
                {
                    if(RowString[Index] == '.')
                    {
                        SetFloor(Layout, Index, LatestRowIndex);
                    }
                    else if(RowString[Index] == 'L')
                    {
                        SetEmpty(Layout, Index, LatestRowIndex);
                    }
                    else
                    {
                        InvalidCodePath;
                    }
                }
            }
        }

        for(;;)
        {
            b32 SeatsChangedType = false;

            seat_layout *NewSeatLayout = Duplicate(Layout);

            for(int Y = 0;
                Y < Layout->Height;
                ++Y)
            {
                for(int X = 0;
                    X < Layout->Width;
                    ++X)
                {
                    seat_type *Seat = GetSeat(Layout, X, Y);
                    switch(*Seat)
                    {
                        case SeatType_Floor:
                        {

                        } break;

                        case SeatType_Empty:
                        {
                            seat_type Up = FindFirstSeat(Layout, X, Y, V2i(0, -1));
                            seat_type Down = FindFirstSeat(Layout, X, Y, V2i(0, 1));
                            seat_type Left = FindFirstSeat(Layout, X, Y, V2i(-1, 0));
                            seat_type Right = FindFirstSeat(Layout, X, Y, V2i(1, 0));
                            seat_type UpLeft = FindFirstSeat(Layout, X, Y, V2i(-1, -1));
                            seat_type UpRight = FindFirstSeat(Layout, X, Y, V2i(1, -1));
                            seat_type DownLeft = FindFirstSeat(Layout, X, Y, V2i(-1, 1));
                            seat_type DownRight = FindFirstSeat(Layout, X, Y, V2i(1, 1));
                            if((Up != SeatType_Occupied) &&
                               (Down != SeatType_Occupied) &&
                               (Left != SeatType_Occupied) &&
                               (Right != SeatType_Occupied) &&
                               (UpLeft != SeatType_Occupied) &&
                               (UpRight != SeatType_Occupied) &&
                               (DownLeft != SeatType_Occupied) &&
                               (DownRight != SeatType_Occupied))
                            {
                                SetOccupied(NewSeatLayout, X, Y);
                                SeatsChangedType = true;
                            }
                        } break;

                        case SeatType_Occupied:
                        {
                            int OccupiedCount = 0;
                            if(FindFirstSeat(Layout, X, Y, V2i(0, -1)) == SeatType_Occupied) {OccupiedCount++;}
                            if(FindFirstSeat(Layout, X, Y, V2i(0, 1)) == SeatType_Occupied) {OccupiedCount++;}
                            if(FindFirstSeat(Layout, X, Y, V2i(-1, 0)) == SeatType_Occupied) {OccupiedCount++;}
                            if(FindFirstSeat(Layout, X, Y, V2i(1, 0)) == SeatType_Occupied) {OccupiedCount++;}
                            if(FindFirstSeat(Layout, X, Y, V2i(-1, -1)) == SeatType_Occupied) {OccupiedCount++;}
                            if(FindFirstSeat(Layout, X, Y, V2i(1, -1)) == SeatType_Occupied) {OccupiedCount++;}
                            if(FindFirstSeat(Layout, X, Y, V2i(-1, 1)) == SeatType_Occupied) {OccupiedCount++;}
                            if(FindFirstSeat(Layout, X, Y, V2i(1, 1)) == SeatType_Occupied) {OccupiedCount++;}
                            if(OccupiedCount >= 5)
                            {
                                SetEmpty(NewSeatLayout, X, Y);
                                SeatsChangedType = true;
                            }
                        } break;

                        InvalidDefaultCase;
                    }
                }
            }

            CopySeats(NewSeatLayout, Layout);
//          PrintSeats(Layout);
            if(!SeatsChangedType)
            {
                break;
            }
        }

        int OccupiedCount = GetOccupiedCount(Layout);
        fprintf(stdout, "%d\n", OccupiedCount);

        fclose(File);
    }
    else
    {
        fprintf(stderr, "fopen failed\n");
    }

    return(0);
}

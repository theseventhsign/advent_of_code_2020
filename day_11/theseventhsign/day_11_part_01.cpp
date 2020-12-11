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
							b32 Up = IsOccupied(Layout, X, Y - 1);
							b32 Down = IsOccupied(Layout, X, Y + 1);
							b32 Left = IsOccupied(Layout, X - 1, Y);
							b32 Right = IsOccupied(Layout, X + 1, Y);
							b32 UpLeft = IsOccupied(Layout, X - 1, Y - 1);
							b32 UpRight = IsOccupied(Layout, X + 1, Y - 1);
							b32 DownLeft = IsOccupied(Layout, X - 1, Y + 1);
							b32 DownRight = IsOccupied(Layout, X + 1, Y + 1);
							if(!Up &&
							   !Down &&
							   !Left &&
							   !Right &&
							   !UpLeft &&
							   !UpRight &&
							   !DownLeft &&
							   !DownRight)
							{
								SetOccupied(NewSeatLayout, X, Y);
								SeatsChangedType = true;
							}
						} break;

						case SeatType_Occupied:
						{
							int OccupiedCount = 0;
							if(IsOccupied(Layout, X, Y - 1)) {OccupiedCount++;}     // Up
							if(IsOccupied(Layout, X, Y + 1)) {OccupiedCount++;}     // Down
							if(IsOccupied(Layout, X - 1, Y)) {OccupiedCount++;}     // Left
							if(IsOccupied(Layout, X + 1, Y)) {OccupiedCount++;}     // Right
							if(IsOccupied(Layout, X - 1, Y - 1)) {OccupiedCount++;} // Up left
							if(IsOccupied(Layout, X + 1, Y - 1)) {OccupiedCount++;} // Up right
							if(IsOccupied(Layout, X - 1, Y + 1)) {OccupiedCount++;} // Down left
							if(IsOccupied(Layout, X + 1, Y + 1)) {OccupiedCount++;} // Down right
							if(OccupiedCount >= 4)
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
			//PrintSeats(Layout);
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

#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
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

typedef float f32;

typedef uintptr_t umm;

struct navigation_computer
{
	int ShipX;
	int ShipY;

	int WaypointX;
	int WaypointY;
};

inline f32
Abs(f32 S)
{
	f32 Result = fabsf(S);

	return(Result);
}

inline int
Absi(int I)
{
	int Result = (int)Abs((int)I);

	return(Result);
}

inline void
MoveWaypointNorth(navigation_computer *Computer, int Units)
{
	Computer->WaypointY += Units;
}

inline void
MoveWaypointSouth(navigation_computer *Computer, int Units)
{
	Computer->WaypointY -= Units;
}

inline void
MoveWaypointWest(navigation_computer *Computer, int Units)
{
	Computer->WaypointX -= Units;
}

inline void
MoveWaypointEast(navigation_computer *Computer, int Units)
{
	Computer->WaypointX += Units;
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_12.txt", "rb");
	if(File)
	{
		navigation_computer Computer_ = {};
		navigation_computer *Computer = &Computer_;
		Computer->WaypointX = 10;
		Computer->WaypointY = 1;

		char Action;
		int Value;
		while(fscanf(File, "%c%d\n", &Action, &Value) == 2)
		{
			switch(Action)
			{
				case 'N':
				{
					MoveWaypointNorth(Computer, Value);
				} break;

				case 'S':
				{
					MoveWaypointSouth(Computer, Value);
				} break;

				case 'W':
				{
					MoveWaypointWest(Computer, Value);
				} break;

				case 'E':
				{
					MoveWaypointEast(Computer, Value);
				} break;

				case 'L':
				{
					int Degrees = Value;
					while(Degrees)
					{
						int OldWaypointX = Computer->WaypointX;
						int OldWaypointY = Computer->WaypointY;

						Computer->WaypointX = -OldWaypointY;
						Computer->WaypointY = OldWaypointX;

						Degrees -= 90;
					}
				} break;

				case 'R':
				{
					int Degrees = Value;
					while(Degrees)
					{
						int OldWaypointX = Computer->WaypointX;
						int OldWaypointY = Computer->WaypointY;

						Computer->WaypointX = OldWaypointY;
						Computer->WaypointY = -OldWaypointX;

						Degrees -= 90;
					}
				} break;

				case 'F':
				{
					int MoveCount = Value;
					while(MoveCount)
					{
						Computer->ShipX += Computer->WaypointX;
						Computer->ShipY += Computer->WaypointY;

						MoveCount--;
					}
				} break;

				InvalidDefaultCase;
			}
		}

		int Result = Absi(Computer->ShipX) + Absi(Computer->ShipY);
		fprintf(stdout, "%d\n", Result);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

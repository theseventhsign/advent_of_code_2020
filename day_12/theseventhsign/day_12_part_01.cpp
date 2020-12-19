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

	int Degrees;
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
MoveNorth(navigation_computer *Computer, int Units)
{
	Computer->ShipY += Units;
}

inline void
MoveSouth(navigation_computer *Computer, int Units)
{
	Computer->ShipY -= Units;
}

inline void
MoveWest(navigation_computer *Computer, int Units)
{
	Computer->ShipX -= Units;
}

inline void
MoveEast(navigation_computer *Computer, int Units)
{
	Computer->ShipX += Units;
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_12.txt", "rb");
	if(File)
	{
		navigation_computer Computer_ = {};
		navigation_computer *Computer = &Computer_;

		char Action;
		int Value;
		while(fscanf(File, "%c%d\n", &Action, &Value) == 2)
		{
			switch(Action)
			{
				case 'N':
				{
					MoveNorth(Computer, Value);
				} break;

				case 'S':
				{
					MoveSouth(Computer, Value);
				} break;

				case 'W':
				{
					MoveWest(Computer, Value);
				} break;

				case 'E':
				{
					MoveEast(Computer, Value);
				} break;

				case 'L':
				{
					Computer->Degrees += Value;
					if(Computer->Degrees >= 360)
					{
						Computer->Degrees -= 360;
					}
				} break;

				case 'R':
				{
					Computer->Degrees -= Value;
					if(Computer->Degrees < 0)
					{
						Computer->Degrees += 360;
					}
				} break;

				case 'F':
				{
					if(Computer->Degrees == 0)
					{
						MoveEast(Computer, Value);
					}
					else if(Computer->Degrees == 90)
					{
						MoveNorth(Computer, Value);
					}
					else if(Computer->Degrees == 180)
					{
						MoveWest(Computer, Value);
					}
					else if(Computer->Degrees == 270)
					{
						MoveSouth(Computer, Value);
					}
					else
					{
						InvalidCodePath;
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

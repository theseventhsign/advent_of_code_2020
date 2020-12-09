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

#define S32Max INT32_MAX
#define S32Min INT32_MIN

typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;

typedef uintptr_t umm;

static s64
FindInvalidNumber(s64 *Numbers, int NumberCount,
				  s64 *InvalidNumber)
{
	b32 Result = false;

	int WindowSize = 25;
	s64 *At = Numbers + WindowSize;
	while(At < (Numbers + NumberCount))
	{
		s64 Target = *At;
		b32 Found = false;
		
		int WindowStartIndex = (At - WindowSize) - Numbers;
		int WindowEndIndex = (At - 1) - Numbers;
		for(int Outer = WindowStartIndex;
			Outer < WindowEndIndex;
			++Outer)
		{
			s64 EntryA = Numbers[Outer];
			for(int Inner = Outer + 1;
				Inner <= WindowEndIndex;
				++Inner)
			{
				s64 EntryB = Numbers[Inner];
				if((EntryA + EntryB) == Target)
				{
					Found = true;
					break;
				}
			}

			if(Found)
			{
				break;
			}
		}

		if(!Found)
		{
			*InvalidNumber = Target;
			Result = true;
			break;
		}

		At++;
	}

	return(Result);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_09.txt", "rb");
	if(File)
	{
		int NumberCount = 0;
		s64 Numbers[1 << 11];

		for(;;)
		{
			Assert(NumberCount < ArrayCount(Numbers));
			int ScanCount = fscanf(File, "%I64d\n", Numbers + NumberCount);
			if(ScanCount == 1)
			{
				NumberCount++;
			}
			else
			{
				break;
			}
		}

		s64 InvalidNumber;
		if(FindInvalidNumber(Numbers, NumberCount, &InvalidNumber))
		{
			s64 *SetStart = 0;
			s64 *SetEnd = 0;

			for(int Outer = 0;
				Outer < NumberCount - 1;
				++Outer)
			{
				s64 Sum = Numbers[Outer];
				for(int Inner = Outer + 1;
					Inner < NumberCount;
					++Inner)
				{
					Sum += Numbers[Inner];
					if(Sum == InvalidNumber)
					{
						SetStart = Numbers + Outer;
						SetEnd = Numbers + Inner;
						break;
					}
				}
			}

			if(SetStart &&
			   SetEnd)
			{
				s64 SetMin = S32Max;
				s64 SetMax = S32Min;
				s64 *At = SetStart;
				while(At <= SetEnd)
				{
					if(*At < SetMin)
					{
						SetMin = *At;
					}
					if(*At > SetMax)
					{
						SetMax = *At;
					}

					At++;
				}

				int Result = SetMin + SetMax;
				fprintf(stdout, "Encryption weakness: %d\n", Result);
			}
			else
			{
				fprintf(stderr, "No set sums to the invalid number!\n");
			}
		}
		else
		{
			fprintf(stderr, "No invalid entries found!\n");
		}

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

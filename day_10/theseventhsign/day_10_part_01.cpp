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

typedef uintptr_t umm;

struct adapter
{
	b32 Claimed;
	int Rating;
};

static b32
ClaimAdapter(adapter *Adapters, int AdapterCount,
			  int TargetRating)
{
	b32 Result = false;

	for(int AdapterIndex = 0;
		AdapterIndex < AdapterCount;
		++AdapterIndex)
	{
		adapter *Adapter = Adapters + AdapterIndex;
		if(!Adapter->Claimed &&
		   (Adapter->Rating == TargetRating))
		{
			Adapter->Claimed = true;
			Result = true;
			break;
		}
	}

	return(Result);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_10.txt", "rb");
	if(File)
	{
		int AdapterCount = 0;
		adapter Adapters[1 << 9] = {};
		
		for(;;)
		{
			Assert(AdapterCount < ArrayCount(Adapters));
			int ScanCount = fscanf(File, "%d\n", &((Adapters + AdapterCount)->Rating));
			if(ScanCount == 1)
			{
				AdapterCount++;
			}
			else
			{
				break;
			}
		}

		// Bubble sort
		for(int Outer = 0;
			Outer < AdapterCount;
			++Outer)
		{
			for(int Inner = 0;
				Inner < AdapterCount - 1;
				++Inner)
			{
				int RatingA = Adapters[Inner].Rating;
				int RatingB = Adapters[Inner + 1].Rating;
				if(RatingA > RatingB)
				{
					adapter Swap = Adapters[Inner];
					Adapters[Inner] = Adapters[Inner + 1];
					Adapters[Inner + 1] = Swap;
				}
			}
		}

		int OneDifferences = 0;
		int ThreeDifferences = 1;
		
		int EffectiveRating = 0;
		int AdaptersClaimed = 0;
		while(AdaptersClaimed < AdapterCount)
		{
			int TargetRatings[] =
			{
				EffectiveRating + 1,
				EffectiveRating + 2,
				EffectiveRating + 3,
			};

			b32 Claimed = false;

			for(int TargetRatingIndex = 0;
				TargetRatingIndex < ArrayCount(TargetRatings);
				++TargetRatingIndex)
			{
				int TargetRating = TargetRatings[TargetRatingIndex];
				if(ClaimAdapter(Adapters, AdapterCount, TargetRating))
				{
					Claimed = true;

					int Difference = TargetRating - EffectiveRating;
					if(Difference == 1)
					{
						OneDifferences++;
					}
					else if(Difference == 3)
					{
						ThreeDifferences++;
					}
					else
					{
						InvalidCodePath;
					}

					EffectiveRating = TargetRating;
					break;
				}
			}

			if(Claimed)
			{
				AdaptersClaimed++;
			}
			else
			{
				InvalidCodePath;
			}
		}

		int Result = OneDifferences*ThreeDifferences;
		fprintf(stdout, "%d\n", Result);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

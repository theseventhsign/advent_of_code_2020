#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define Assert assert
#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

#define Kilobytes(Value) ((Value)*1024ULL)
#define Megabytes(Value) (Kilobytes(Value)*1024ULL)
#define Gigabytes(Value) (Megabytes(Value)*1024ULL)
#define Terabytes(Value) (Gigabytes(Value)*1024ULL)

#define S32Min INT32_MIN

typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;

typedef uint32_t u32;
typedef uint64_t u64;

typedef uintptr_t umm;

struct adapter
{
	b32 Claimed;
	int Rating;
};

struct queue
{
	u64 ReadIndex;
	u64 WriteIndex;

	u64 EntryCount;
	int *Entries;
};

static queue *
MakeQueue(void)
{
	queue *Queue = (queue *)malloc(sizeof(queue));
	
	Queue->ReadIndex = 0;
	Queue->WriteIndex = 0;

	Queue->EntryCount = 1ULL << 40;
	u64 QueueEntriesSize = EntryCount*4ULL;
	Queue->Entries = (int *)VirtualAlloc(0, QueueEntriesSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

	DWORD Error = GetLastError();

	return(Queue);
}

static void
Push(queue *Queue, int Entry)
{
	*(Queue->Entries + Queue->WriteIndex++) = Entry;
	if(Queue->WriteIndex >= Queue->EntryCount)
	{
		Queue->WriteIndex &= (Queue->EntryCount - 1);
	}
	Assert(Queue->WriteIndex != Queue->ReadIndex);
}

static int
Pop(queue *Queue)
{
	int Result = *(Queue->Entries + Queue->ReadIndex++);
	if(Queue->ReadIndex >= Queue->EntryCount)
	{
		Queue->ReadIndex &= (Queue->EntryCount - 1);
	}
	return(Result);
}

static b32
IsEmpty(queue *Queue)
{
	b32 Result = (Queue->WriteIndex == Queue->ReadIndex);

	return(Result);
}

static void
FreeQueue(queue *Queue)
{
	free(Queue->Entries);
	free(Queue);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_10.txt", "rb");
	if(File)
	{
		int AdapterCount = 0;
		adapter Adapters[1 << 7] = {};
		
		//
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
		int MaximumRating = S32Min;

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
				if(RatingB > MaximumRating)
				{
					MaximumRating = RatingB;
				}
			}
		}

		//
		s64 CompletedBranchCount = 0;
		int PenultimateRating = MaximumRating;

		queue *EffectiveRatingQueue = MakeQueue();
		Push(EffectiveRatingQueue, 0);

		while(!IsEmpty(EffectiveRatingQueue))
		{
			int EffectiveRating = Pop(EffectiveRatingQueue);
			int TargetRatings[] =
			{
				EffectiveRating + 1,
				EffectiveRating + 2,
				EffectiveRating + 3,
			};
			for(int TargetRatingIndex = 0;
				TargetRatingIndex < ArrayCount(TargetRatings);
				++TargetRatingIndex)
			{
				int TargetRating = TargetRatings[TargetRatingIndex];

				for(int AdapterIndex = 0;
					AdapterIndex < ArrayCount(Adapters);
					++AdapterIndex)
				{
					int AdapterRating = (Adapters + AdapterIndex)->Rating;
					if(AdapterRating == TargetRating)
					{
						Push(EffectiveRatingQueue, AdapterRating);
						if(AdapterRating == PenultimateRating)
						{
							CompletedBranchCount++;
						}
						break;
					}
				}
			}
		}

		fprintf(stdout, "%I64d\n", CompletedBranchCount);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

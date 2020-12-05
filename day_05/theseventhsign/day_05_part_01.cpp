#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define Assert assert
#define InvalidCodePath Assert(!"InvalidCodePath")

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

#define S32Min INT32_MIN

typedef int32_t s32;
typedef s32 b32;

static int
BinarySearchSpec(char *Spec,
	             int PartitionLeftInit, int PartitionRightInit,
	             char LeftChar, char RightChar)
{
	int PartitionLeft = PartitionLeftInit;
	int PartitionRight = PartitionRightInit;

	char *At = Spec;
	while(*At)
	{
		char Half = *At;
		Assert((Half == LeftChar) || (Half == RightChar));
		int NewPartitionSize = ((PartitionRight - PartitionLeft) + 1) / 2;
		if(Half == LeftChar)
		{
			PartitionRight = PartitionLeft + (NewPartitionSize - 1);
		}
		else if(Half == RightChar)
		{
			PartitionLeft += NewPartitionSize;
		}

		if(NewPartitionSize == 1)
		{
			break;
		}

		At++;
	}

	Assert(PartitionLeft == PartitionRight);
	int Result = PartitionLeft;

	return(Result);
}

static int
ComputeRow(char *Spec)
{
	int Result = BinarySearchSpec(Spec, 0, 127, 'F', 'B');

	return(Result);
}

static int
ComputeColumn(char *Spec)
{
	int Result = BinarySearchSpec(Spec, 0, 7, 'L', 'R');

	return(Result);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_05.txt", "rb");
	if(File)
	{
		int HighestSeatID = S32Min;

		char RowSpec[8];
		char ColumnSpec[4];
		while(fscanf(File, "%7c%3c\n", &RowSpec, &ColumnSpec) == 2)
		{
			// Boring null termination
			RowSpec[7] = '\0';
			ColumnSpec[3] = '\0';

			int Row = ComputeRow(RowSpec);
			int Column = ComputeColumn(ColumnSpec);
			int SeatID = Row*8 + Column;
			if(SeatID > HighestSeatID)
			{
				HighestSeatID = SeatID;
			}
		}

		fprintf(stdout, "Highest seat ID: %d\n", HighestSeatID);
		
		fclose(File);
	}
	else
	{
		fprintf(stderr, "File failed to fopen\n");
	}

	return(0);
}

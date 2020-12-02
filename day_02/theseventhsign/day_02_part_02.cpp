#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>

#define Assert assert

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

typedef int32_t s32;
typedef s32 b32;

struct password_with_policy
{
	int MinAppear;
	int MaxAppear;
	char Char;
	char Password[64];
};

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_02.txt", "rb");
	if(File)
	{
		int EntryCount = 0;
		password_with_policy *Entries = (password_with_policy *)malloc(sizeof(password_with_policy)*1024);

		for(;;)
		{
			Assert(EntryCount < 1024);
			password_with_policy *Entry = Entries + EntryCount;
			if(fscanf(File, "%d-%d %c: %s\n", &Entry->MinAppear, &Entry->MaxAppear, &Entry->Char, &Entry->Password) == 4)
			{
				EntryCount++;
			}
			else
			{
				break;
			}
		}

		int ValidCount = 0;

		for(int EntryIndex = 0;
			EntryIndex < EntryCount;
			++EntryIndex)
		{
			password_with_policy *Entry = Entries + EntryIndex;

			int FirstIndex = Entry->MinAppear - 1;
			int SecondIndex = Entry->MaxAppear - 1;
			if(((Entry->Password[FirstIndex] == Entry->Char) && (Entry->Password[SecondIndex] != Entry->Char)) ||
			   ((Entry->Password[FirstIndex] != Entry->Char) && (Entry->Password[SecondIndex] == Entry->Char)))
			{
				ValidCount++;
			}
		}

		printf("There are %d valid passwords\n", ValidCount);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "File failed to fopen\n");
	}

	return(0);
}

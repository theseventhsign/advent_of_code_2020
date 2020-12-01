#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>

#define Assert assert

typedef int32_t s32;
typedef s32 b32;

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_01.txt", "rb");
	if(File)
	{
		int EntryCount = 0;
		int Entries[512] = {};

		int Entry;
		while(fscanf(File, "%d\n", &Entry) == 1)
		{
			Entries[EntryCount++] = Entry;
		}

		int Result = 0;
		b32 ResultSet = false;

		for(int Outer = 0;
			Outer < EntryCount - 1;
			++Outer)
		{
			int EntryA = Entries[Outer];
			for(int Inner = Outer + 1;
				Inner < EntryCount;
				++Inner)
			{
				int EntryB = Entries[Inner];
				if((EntryA + EntryB) == 2020)
				{
					Result = EntryA*EntryB;
					ResultSet = true;
					break;
				}
			}

			if(ResultSet)
			{
				break;
			}
		}

		if(ResultSet)
		{
			fprintf(stdout, "Result is %d\n", Result);
		}
		else
		{
			fprintf(stderr, "Result not found!\n");
		}

		fclose(File);
	}
	else
	{
		fprintf(stderr, "File failed to fopen\n");
	}

	return(0);
}

#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>

#define Assert assert

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

typedef int32_t s32;
typedef s32 b32;

struct v2i
{
	int x;
	int y;
};

struct map
{
	int Width;
	int Height;
	char *Memory;
};

static b32
IsInBounds(map *Map, v2i P)
{
	b32 Result = P.y < Map->Height;

	return(Result);
}

static b32
TreeExistsAtP(map *Map, v2i P)
{
	int X = P.x % Map->Width;
	int Y = P.y;

	Assert((0 <= X) && (X < Map->Width));
	Assert((0 <= Y) && (X < Map->Height));

	char Value = *(Map->Memory + X + Y*Map->Width);
	b32 Result = Value == '#';
	return(Result);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_03.txt", "rb");
	if(File)
	{
		fseek(File, 0, SEEK_END);
		long FileSize = ftell(File);
		fseek(File, 0, SEEK_SET);

		char *FileMemory = (char *)malloc(FileSize + 1);
		fread(FileMemory, FileSize, 1, File);
		FileMemory[FileSize] = '\0';

		map *Map = (map *)malloc(sizeof(map));
		*Map = {};
		{
			char *At = FileMemory;
			while(*At++ != '\n')
			{
				Map->Width++;
			}
		}
		{
			char *At = FileMemory;
			while(*At++ != '\0')
			{
				if(*At == '\n')
				{
					Map->Height++;
				}
			}
		}
		int MapSize = 1024*1024*1024;
		Map->Memory = (char *)malloc(MapSize);
		{
			char *Source = FileMemory;
			char *Dest = Map->Memory;
			int BytesWritten = 0;
			while(*Source != '\0')
			{
				if(*Source != '\n')
				{
					*Dest++ = *Source;
					Assert((Source - FileMemory) < MapSize);
				}

				Source++;
			}
			*Dest++ = '\0';
		}

		int Result = 0;
		
		v2i Slopes[] =
		{
			{1, 1},
			{3, 1},
			{5, 1},
			{7, 1},
			{1, 2},
		};
		for(int SlopeIndex = 0;
			SlopeIndex < ArrayCount(Slopes);
			++SlopeIndex)
		{
			int TreeCount = 0;
			
			v2i P = {};
			v2i Slope = Slopes[SlopeIndex];
			while(IsInBounds(Map, P))
			{
				P.x += Slope.x;
				P.y += Slope.y;

				b32 TreeFound = TreeExistsAtP(Map, P);
				if(TreeFound)
				{
					TreeCount++;
				}
			}

			if(Result == 0)
			{
				Result = TreeCount;
			}
			else
			{
				Result *= TreeCount;
			}
		}

		printf("Result is equal to %d\n", Result);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "File failed to fopen\n");
	}

	return(0);
}

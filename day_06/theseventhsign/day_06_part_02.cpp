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

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_06.txt", "rb");
	if(File)
	{
		int EveryoneAnsweredSum = 0;

		b32 Parsing = true;
		while(Parsing)
		{
			int GroupPeopleCount = 0;
			int GroupQuestionCounts[256] = {};

			for(;;)
			{
				char Questions[64];
				char Newline;
				if(fscanf(File, "%s%c", &Questions, &Newline) == 2)
				{
					GroupPeopleCount++;

					for(char *At = Questions;
						*At;
						++At)
					{
						GroupQuestionCounts[*At]++;
					}

					char NextChar = fgetc(File);
					if(NextChar == '\n')
					{
						break;
					}
					else
					{
						ungetc(NextChar, File);
					}
				}
				else
				{
					Parsing = false;
					break;
				}
			}

			int EveryoneAnsweredCount = 0;
			for(int GroupQuestionCountIndex = 0;
				GroupQuestionCountIndex < ArrayCount(GroupQuestionCounts);
				++GroupQuestionCountIndex)
			{
				if(GroupQuestionCounts[GroupQuestionCountIndex] == GroupPeopleCount)
				{
					EveryoneAnsweredCount++;
				}
			}
			EveryoneAnsweredSum += EveryoneAnsweredCount;
		}

		fprintf(stdout, "Result: %d\n", EveryoneAnsweredSum);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

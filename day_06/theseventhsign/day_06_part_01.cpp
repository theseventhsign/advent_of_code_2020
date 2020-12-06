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
		int QuestionCountSum = 0;

		b32 Parsing = true;
		while(Parsing)
		{
			int QuestionCount = 0;
			b32 QuestionEntries[256] = {};

			for(;;)
			{
				char Questions[64];
				char Newline;
				if(fscanf(File, "%s%c", &Questions, &Newline) == 2)
				{
					for(char *At = Questions;
						*At;
						++At)
					{
						QuestionEntries[*At] = true;						
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

			for(int QuestionEntryIndex = 0;
				QuestionEntryIndex < ArrayCount(QuestionEntries);
				++QuestionEntryIndex)
			{
				if(QuestionEntries[QuestionEntryIndex])
				{
					QuestionCount++;
				}
			}
			
			QuestionCountSum += QuestionCount;
		}

		fprintf(stdout, "Result: %d\n", QuestionCountSum);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

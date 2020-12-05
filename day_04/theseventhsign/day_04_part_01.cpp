#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define Assert assert

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

typedef int32_t s32;
typedef s32 b32;

struct kvp
{
	char *Key;
	char *Value;
};
 
struct passport
{
	int FieldCount;
	kvp Fields[16];
};

struct aps
{
	int PassportCount;
	passport Passports[512];
};

static passport *
NewPassport(aps *APS)
{
	Assert(APS->PassportCount < ArrayCount(APS->Passports));
	passport *Passport = APS->Passports + APS->PassportCount++;
	*Passport = {};
	return(Passport);
}

static void
AddField(passport *Passport, char *Key, char *Value)
{
	Assert(Passport->FieldCount < ArrayCount(Passport->Fields));
	kvp *Field = Passport->Fields + Passport->FieldCount++;
	Field->Key = _strdup(Key);
	Field->Value = _strdup(Value);
}

static b32
IsValid(passport *Passport)
{
	b32 Result = true;

	int FieldCounts[] = 
	{
		0, // byr
		0, // iyr
		0, // eyr
		0, // hgt
		0, // hcl
		0, // ecl
		0, // pid
		0, // cid
	};

	for(int FieldIndex = 0;
		FieldIndex < Passport->FieldCount;
		++FieldIndex)
	{
		kvp *Field = Passport->Fields + FieldIndex;
		if(strcmp(Field->Key, "byr") == 0)
		{
			FieldCounts[0]++;
		}
		else if(strcmp(Field->Key, "iyr") == 0)
		{
			FieldCounts[1]++;
		}
		else if(strcmp(Field->Key, "eyr") == 0)
		{
			FieldCounts[2]++;
		}
		else if(strcmp(Field->Key, "hgt") == 0)
		{
			FieldCounts[3]++;
		}
		else if(strcmp(Field->Key, "hcl") == 0)
		{
			FieldCounts[4]++;
		}
		else if(strcmp(Field->Key, "ecl") == 0)
		{
			FieldCounts[5]++;
		}
		else if(strcmp(Field->Key, "pid") == 0)
		{
			FieldCounts[6]++;
		}
		else if(strcmp(Field->Key, "cid") == 0)
		{
			FieldCounts[7]++;
		}
	}

	for(int FieldCountIndex = 0;
		FieldCountIndex < ArrayCount(FieldCounts);
		++FieldCountIndex)
	{
		int Count = FieldCounts[FieldCountIndex];
		if((Count == 0) &&
			(FieldCountIndex != 7))
		{
			Result = false;
			break;
		}
	}

	return(Result);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_04.txt", "rb");
	if(File)
	{
#if 0
		fseek(File, 0, SEEK_END);
		long FileSize = ftell(File);
		fseek(File, 0, SEEK_SET);

		char *FileMemory = (char *)malloc(FileSize + 1);
		fread(FileMemory, FileSize, 1, File);
		FileMemory[FileSize] = '\0';
#endif

		aps *APS = (aps *)malloc(sizeof(aps));
		*APS = {};

		b32 Parsing = true;
		while(Parsing)
		{
			passport *Passport = NewPassport(APS);
			for(;;)
			{
				char Key[64];
				char Value[64];
				if(fscanf(File, "%3c:%s", &Key, &Value) == 2)
				{
					// 3c format specifier does not null terminate
					Key[3] = '\0';
					AddField(Passport, Key, Value);
					char Char = fgetc(File);
					if(Char == ' ')
					{

					}
					else if(Char == '\n')
					{
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
				}
				else
				{
					Parsing = false;
					break;
				}
			}
		}

		int Result = 0;

		for(int PassportIndex = 0;
			PassportIndex < APS->PassportCount;
			++PassportIndex)
		{
			passport *Passport = APS->Passports + PassportIndex;
			if(IsValid(Passport))
			{
				Result++;
			}
		}

		fprintf(stdout, "There are %d valid passports\n", Result);
		
		fclose(File);
	}
	else
	{
		fprintf(stderr, "File failed to fopen\n");
	}

	return(0);
}

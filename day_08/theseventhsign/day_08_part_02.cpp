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
typedef s32 b32;

typedef uintptr_t umm;

enum instruction_type
{
	InstrType_Acc,
	InstrType_Jump,
	InstrType_NOP,
};
struct instruction
{
	instruction_type Type;
	int Offset;
};

struct program_state
{
	int Accumulator;

	int JumpCount;
	instruction *Jumps[2048];

	int NOPCount;
	instruction *NOPs[2048];

	int InstructionCount;
	instruction Instructions[2048];
};

static b32
RunProgram(program_state *ProgramState)
{
	b32 Result = false;

	// Program "reset"
	ProgramState->Accumulator = 0;
	int InstructionIndices[ArrayCount(ProgramState->Instructions)] = {};

	instruction *InstructionPtr = ProgramState->Instructions;
	while(InstructionPtr < (ProgramState->Instructions + ProgramState->InstructionCount))
	{
		int InstructionIndex = InstructionPtr - ProgramState->Instructions;
		if(InstructionIndices[InstructionIndex] != 0)
		{
			Result = true;
			break;
		}

		switch(InstructionPtr->Type)
		{
			case InstrType_Acc:
			{
				ProgramState->Accumulator += InstructionPtr->Offset;
			} break;

			case InstrType_Jump:
			{
				InstructionPtr += InstructionPtr->Offset;
				continue;
			} break;

			case InstrType_NOP:
			{

			} break;

			InvalidDefaultCase;
		}

		InstructionIndices[InstructionIndex]++;
		InstructionPtr++;
	}

	return(Result);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_08.txt", "rb");
	if(File)
	{
		program_state *ProgramState = (program_state *)malloc(sizeof(program_state));
		*ProgramState = {};

		char Instruction[32];
		char Sign;
		int Offset;
		while(fscanf(File, "%s %c%d\n", &Instruction, &Sign, &Offset) == 3)
		{
			Assert(ProgramState->InstructionCount < ArrayCount(ProgramState->Instructions));
			instruction *NewInstruction = ProgramState->Instructions + ProgramState->InstructionCount++;
			if(strcmp(Instruction, "acc") == 0)
			{
				NewInstruction->Type = InstrType_Acc;
			}
			else if(strcmp(Instruction, "jmp") == 0)
			{
				NewInstruction->Type = InstrType_Jump;
				Assert(ProgramState->JumpCount < ArrayCount(ProgramState->Jumps));
				ProgramState->Jumps[ProgramState->JumpCount++] = NewInstruction;
			}
			else if(strcmp(Instruction, "nop") == 0)
			{
				NewInstruction->Type = InstrType_NOP;
				Assert(ProgramState->NOPCount < ArrayCount(ProgramState->NOPs));
				ProgramState->NOPs[ProgramState->NOPCount++] = NewInstruction;
			}
			else
			{
				InvalidCodePath;
			}
			Assert((Sign == '+') || (Sign == '-'));
			NewInstruction->Offset = (Sign == '+') ? +Offset : -Offset;
		}

		b32 ResultSet = false;
		int Result = 0;

		// I didn't even have to try to toggle NOPs!
		for(int JumpIndex = 0;
			JumpIndex < ProgramState->JumpCount;
			++JumpIndex)
		{
			instruction *Jump = ProgramState->Jumps[JumpIndex];
			Jump->Type = InstrType_NOP;
			{
				b32 Looped = RunProgram(ProgramState);
				if(!Looped)
				{
					Result = ProgramState->Accumulator;
					ResultSet = true;
					break;
				}
			}
			Jump->Type = InstrType_Jump;
		}

		if(ResultSet)
		{
			fprintf(stdout, "Accumulator: %d\n", Result);
		}
		else
		{
			fprintf(stderr, "Program never completed!\n");
		}

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

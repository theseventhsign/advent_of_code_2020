/*
awful auburn bags contain 1 bright blue bag, 2 crass coral bags.
bright blue bags contain 3 crass coral bags.
crass coral bags contain 4 dark dantrine bags.
dark dantrine bags contain no other bags.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <ctype.h>
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

typedef uintptr_t umm;

struct vertex
{
	char *Name;
	b32 Visited; // Used in search
};

struct edge
{
	// DAGs can treat this as *from* A *to* B
	vertex *A;
	vertex *B;
	int Weight;
};

struct graph
{
	int EdgeCount;
	edge Edges[2048];

	int VertexCount;
	vertex Vertices[2048];
};

struct queue
{
	int ReadIndex;
	int WriteIndex;
	umm Entries[2048];
};

static graph *
MakeGraph(void)
{
	graph *Graph = (graph *)malloc(sizeof(graph));
	*Graph = {};

	return(Graph);
}

inline char *
ConcatWithWhitespace(char *A, char *B)
{
	char WhitespaceString[] =
	{
		' ',
		'\0',
	};
	char *Result = _strdup(strcat(strcat(A, WhitespaceString), B));
	return(Result);
}

static vertex *
AddVertex(graph *Graph, char *Name)
{
	Assert(Graph->VertexCount < ArrayCount(Graph->Vertices));
	vertex *Vertex = Graph->Vertices + Graph->VertexCount++;
	Vertex->Name = Name;

	return(Vertex);
}

static vertex *
GetVertex(graph *Graph, char *Name, b32 AddNew = true)
{
	vertex *Result = 0;

	for(int VertexIndex = 0;
		VertexIndex < Graph->VertexCount;
		++VertexIndex)
	{
		vertex *Vertex = Graph->Vertices + VertexIndex;
		if(strcmp(Vertex->Name, Name) == 0)
		{
			Result = Vertex;
			break;
		}
	}

	if(!Result &&
	   AddNew)
	{
		Result = AddVertex(Graph, Name);
	}

	return(Result);
}

static void
AddWeightedEdge(graph *Graph, vertex *A, vertex *B, int Weight)
{
	Assert(Graph->EdgeCount < ArrayCount(Graph->Edges));
	edge *Edge = Graph->Edges + Graph->EdgeCount++;
	Edge->A = A;
	Edge->B = B;
	Edge->Weight = Weight;
}

static void
ResetVertices(graph *Graph)
{
	for(int VertexIndex = 0;
		VertexIndex < Graph->VertexCount;
		++VertexIndex)
	{
		vertex *Vertex = Graph->Vertices + VertexIndex;
		Vertex->Visited = false;
	}
}

static queue *
MakeQueue(void)
{
	queue *Queue = (queue *)malloc(sizeof(queue));
	*Queue = {};

	return(Queue);
}

static void
Push(queue *Queue, umm Entry)
{
	Queue->Entries[Queue->WriteIndex++] = Entry;
	if(Queue->WriteIndex >= ArrayCount(Queue->Entries))
	{
		Queue->WriteIndex &= (ArrayCount(Queue->Entries) - 1);
	}
	Assert(Queue->WriteIndex != Queue->ReadIndex);
}

static umm
Pop(queue *Queue)
{
	umm Result = Queue->Entries[Queue->ReadIndex++];
	if(Queue->ReadIndex >= ArrayCount(Queue->Entries))
	{
		Queue->ReadIndex &= (ArrayCount(Queue->Entries) - 1);
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
	free(Queue);
}

int
main(int ArgCount, char **Args)
{
	FILE *File = fopen("../input/day_07.txt", "rb");
	if(File)
	{
		graph *Bags = MakeGraph();

		for(;;)
		{
			char BagColor0[32];
			char BagColor1[32];
			if(fscanf(File, "%s %s bags contain ", &BagColor0, &BagColor1) == 2)
			{
				vertex *Bag = GetVertex(Bags, ConcatWithWhitespace(BagColor0, BagColor1));
				for(;;)
				{
					char NOrNumber = fgetc(File);
					if(NOrNumber == 'n')
					{
						ungetc(NOrNumber, File);
						if(fscanf(File, "no other bags.\n") == 0)
						{
							break;
						}
						else
						{
							InvalidCodePath;
						}
					}
					else if(isdigit(NOrNumber))
					{
						ungetc(NOrNumber, File);

						int Count;
						char InnerBagColor0[32];
						char InnerBagColor1[32];
						if(fscanf(File, "%d %s %s bag", &Count, &InnerBagColor0, &InnerBagColor1) == 3)
						{
							vertex *OtherBag = GetVertex(Bags, ConcatWithWhitespace(InnerBagColor0, InnerBagColor1));
							AddWeightedEdge(Bags, Bag, OtherBag, Count);
							
							char NextChar = fgetc(File);
							// Skip the 's' at the end of "bags" if present
							if(NextChar == 's')
							{
								NextChar = fgetc(File);
							}
							if(NextChar == ',')
							{
								// Skip whitespace
								fgetc(File);
							}
							else if(NextChar == '.')
							{
								break;
							}
						}
					}
					else
					{
						InvalidCodePath;
					}
				}
			}
			else
			{
				break;
			}
		}

		int Result = 0;
		
		queue *VertexQueue = MakeQueue();
		queue *WeightQueue = MakeQueue();
		
		vertex *StartVertex = GetVertex(Bags, (char *)"shiny gold");
		
		Push(VertexQueue, (umm)(void *)StartVertex);
		Push(WeightQueue, 1);
		
		while(!IsEmpty(VertexQueue) &&
			  !IsEmpty(WeightQueue))
		{
			vertex *Vertex = (vertex *)Pop(VertexQueue);
			umm IncomingWeight = Pop(WeightQueue);
			
			for(int EdgeIndex = 0;
				EdgeIndex < Bags->EdgeCount;
				++EdgeIndex)
			{
				edge *Edge = Bags->Edges + EdgeIndex;
				if(Edge->A == Vertex)
				{
					int NewBags = IncomingWeight*Edge->Weight;

					Result += NewBags;

					Push(VertexQueue, (umm)(void *)Edge->B);
					Push(WeightQueue, NewBags);
				}
			}
		}

		fprintf(stdout, "Result: %d\n", Result);

		fclose(File);
	}
	else
	{
		fprintf(stderr, "fopen failed\n");
	}

	return(0);
}

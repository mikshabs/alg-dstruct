#pragma once

#include<stdio.h>
#include<stdlib.h>

typedef struct List
{
	int field;
	struct List* next;
} list_t;

typedef struct Queue
{
	struct List* head, * tail;
} queue_t;

typedef struct Edge
{
	int one;
	int two;
} edge_t;

typedef struct Graph
{
	int size;
	edge_t* edge;
} graph_t;

queue_t* QueueCreate(int value);
void QueueDestroy(queue_t* queue);
void AddInQueue(queue_t* queue, int field);
void DeleteOutQueue(queue_t* queue);
graph_t GraphCreate();
void GraphBFS();
void StressTest();
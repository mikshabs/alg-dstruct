#include <stdio.h>
#include <stdlib.h>
//#include <Windows.h>
#pragma warning (disable:4996)
#pragma warning (disable:6031)


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


queue_t* QueueCreate(int value)
{
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	if (!queue)
	{
		printf("Memory error");
		return NULL;
	}
	queue->head = (list_t*)malloc(sizeof(list_t));
	if (!queue->head)
	{
		printf("Memory error");
		return NULL;
	}
	queue->head->field = value;
	queue->head->next = NULL;
	queue->tail = queue->head;
	return queue;
}

void AddInQueue(queue_t* queue, int field)
{
	if (!queue->tail)
	{
		QueueCreate(field);
		return;
	}
	queue->tail->next = (list_t*)malloc(sizeof(list_t));
	if (!queue->tail->next)
	{
		perror("Memory error");
		return;
	}
	queue->tail = queue->tail->next;
	queue->tail->field = field;
	queue->tail->next = NULL;
}

void DeleteOutQueue(queue_t* queue)
{
	if (queue)
	{
		list_t* temp;
		temp = queue->head;
		queue->head = queue->head->next;
		free(temp);
	}
}

graph_t GraphCreate()
{
	FILE* file = fopen("test.txt", "r");
	graph_t graph;
	graph_t error = { 0,NULL };
	fscanf(file, "%i", &graph.size);
	int numEdge = 0.5 * graph.size * graph.size - 0.5 * graph.size;
	graph.edge = malloc(numEdge * sizeof(edge_t));
	if (!graph.edge)
	{
		printf("Memory error");
		return error;
	}
	int i = 0;
	while ((fscanf(file, "%i", &graph.edge[i].one) != EOF) && (fscanf(file, "%i", &graph.edge[i].two) != EOF) && (i != numEdge))
	{
		i++;
	}
	graph.edge[i].one = 0;
	graph.edge[i].two = 0;
	fclose(file);
	return graph;
}

void ShirtArray(edge_t edge[], int number)
{
	int i = number;
	while ((edge[i].one != 0) || (edge[i].two != 0))
	{
		edge[i] = edge[i + 1];
		i++;
	}
}

int FoundNextVert(graph_t graph, int top)
{
	int i = 0;
	int nextVert = graph.size, count;
	while ((graph.edge[i].one != 0) || (graph.edge[i].two != 0))
	{
		if (graph.edge[i].one == top)
		{
			if (graph.edge[i].two < nextVert)
			{
				nextVert = graph.edge[i].two;
				count = i;
			}
		}
		if (graph.edge[i].two == top)
		{
			if (graph.edge[i].one < nextVert)
			{
				nextVert = graph.edge[i].one;
				count = i;
			}
		}
		i++;
	}
	if (nextVert != graph.size)
	{
		ShirtArray(graph.edge, count);
		return nextVert;
	}
	return 0;
}

int IsInQuene(queue_t* queue, int value)
{
	list_t* tmp = queue->head;
	while (tmp != NULL)
	{
		if (tmp->field == value)
		{
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

int IsInArr(int* arr, int size, int value)
{
	for (int i = 0; i < size; i++) {
		if (arr[i] == value) {
			return 1;
		}
	}
	return 0;
}

void QueueDestroy(queue_t* queue)
{
	list_t* tmp = NULL;
	while (queue->head != NULL) {
		tmp = queue->head;
		queue->head = queue->head->next;
		free(tmp);
	}
}

void Print(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%i ", arr[i]);
	}
}

void GraphBFS()
{
	graph_t graph = GraphCreate();
	queue_t* queue = QueueCreate(0);
	if (!queue)
	{
		return;
	}
	int* BFS = (int*)malloc(graph.size * sizeof(int));
	if (!BFS)
	{
		perror("Memory Error");
		return;
	}
	int i = 0;
	int countBFS = 0;
	while (countBFS < graph.size)
	{
		if (queue->head == NULL)
			break;
		int tmp = queue->head->field;
		int vert = FoundNextVert(graph, tmp);
		while (vert != 0)
		{
			if ((IsInQuene(queue, vert) == 0) && (IsInArr(BFS, countBFS + 1, vert) == 0))
			{
				AddInQueue(queue, vert);
			}
			vert = FoundNextVert(graph, tmp);
		}
		DeleteOutQueue(queue);
		BFS[countBFS] = tmp;
		countBFS++;
	}
	QueueDestroy(queue);
	Print(BFS, countBFS);
	free(BFS);
	free(graph.edge);
}

int Random(int left, int right)
{
	int k;
	double value;
	k = rand();
	value = ((double)k) / 10000;
	value = value * (right - left) + left;
	return (int)value;
}

void StressTest()
{
	//LARGE_INTEGER start, end, counter;
	int start, end, counter;
	double time;

	int numVert = 10000;

	int numEdge = numVert * 100;

	FILE* file = fopen("test.txt", "w");
	fprintf(file, "%i\n", numVert);

	for (int i = 0; i < numEdge; i++)
	{
		fprintf(file, "%i  %i\n", Random(0, 9999), Random(0, 9999));
	}

	fclose(file);

	//QueryPerformanceFrequency(&counter);
	//QueryPerformanceCounter(&start);

	GraphBFS();

	//QueryPerformanceCounter(&end);
	//time = (double)((double)(end.QuadPart - start.QuadPart) / (double)counter.QuadPart);

	printf("%lf", time);
}

int main()
{
	StressTest();
	return 0;
}
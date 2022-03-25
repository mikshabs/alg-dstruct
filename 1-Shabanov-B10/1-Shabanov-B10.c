#include "memallocator.h"
#include <stdio.h>
#include <assert.h>

#define NULL ((void*)0)   
#define MY_NULL ((memblock_t*)0)   

#define FREE 1   
#define	BUSY 0   
#define BUSY_SPECIAL(x) ((-1)*x)   
#define SIZE_FULL_BLOCK memgetblocksize()   
#define SIZE_PART_BLOCK (memgetblocksize() - sizeof(int))   
#define FREE_BLOCK_MARKER -20


typedef struct memblock_t {
	int size;
	int status;
	struct memblock_t* next;
	struct memblock_t* prev;
} memblock_t;
typedef struct {
	memblock_t* head;
	int freeSize;
	int startSize;
} memcontrol_t;



memcontrol_t* MemoryStart = (memcontrol_t*)0;
memblock_t* lastReturnedPointer;


int memgetblocksize(void) {
	return sizeof(memblock_t) + sizeof(int);
}
int memgetminimumsize(void) {
	return memgetblocksize() + sizeof(memcontrol_t);
}



static void SetControlSize(memblock_t* dataptr, int status) {
	assert(dataptr->size >= 0);

	int* p = (int*)((char*)dataptr + SIZE_PART_BLOCK + dataptr->size);

	switch (status) {
	case FREE:
		*p = dataptr->size;
		break;
	case BUSY:
		*p = (int)dataptr->size * (-1);
		break;
	default:
		break;
	}
}
static int SeeControlSize(memblock_t* dataptr) {
	return *((int*)((char*)dataptr + SIZE_PART_BLOCK + dataptr->size));
}
static void SetFreeBytes(void* start, void* end) {
	if (start == end)
		return;

	char* p = (char*)start;
	do {
		*p = FREE_BLOCK_MARKER;
		p++;
	} while (p != (char*)end);
}
static int IsCorrupted(void) {
	memblock_t* ptr = MemoryStart->head;
	do {
		if (ptr->status == FREE) {
			if (SeeControlSize(ptr) < 0)
				return 1;
		}
		else {
			if (SeeControlSize(ptr) > 0)
				return 1;
		}
		ptr = ptr->next;
	} while (ptr != MY_NULL);
	return 0;
}
static memblock_t* FindFirstSuitBlock(int size, memblock_t* lastPtr) {
	memblock_t* best = MY_NULL;
	memblock_t* ptrPrev = lastPtr, * ptrNext = lastPtr;
	memblock_t* temp;

	if (lastPtr == MY_NULL)
		lastPtr = MemoryStart->head;


	while (best == MY_NULL) {
		if (lastPtr->size >= size && lastPtr->status == FREE) {
			best = lastPtr;
			break;
		}
		if (ptrNext->next == MY_NULL && ptrPrev->prev == MY_NULL) {
			if (ptrNext->size >= size && ptrNext->status == FREE) {
				best = ptrNext;
				break;
			}
			else
				return MY_NULL;
		}

		if (ptrPrev->prev != MY_NULL) {
			if (ptrPrev->prev->size >= size && ptrPrev->prev->status == FREE) {
				best = ptrPrev->prev;
				break;
			}
			else
				ptrPrev = ptrPrev->prev;
		}
		if (ptrNext->next != MY_NULL) {
			if (ptrNext->next->size >= size && ptrNext->next->status == FREE) {
				best = ptrNext->next;
				break;
			}
			else
				ptrNext = ptrNext->next;
		}

	}


	temp = best->next;
	if (best->size - (size + SIZE_FULL_BLOCK) > 0) {
		best->next = (memblock_t*)((char*)best + SIZE_FULL_BLOCK + size);
		best->next->prev = best;
		if (best->next == MY_NULL)
			best->next->next = MY_NULL;
		else {
			best->next->next = temp;
			if (best->next->next != MY_NULL)
				best->next->next->prev = best->next;
		}
		best->next->status = FREE;
		best->next->size = best->size - (size + SIZE_FULL_BLOCK);
		SetControlSize(best->next, FREE);
		best->status = BUSY;
	}
	else {
		best->status = BUSY_SPECIAL((best->size - size));
		SetControlSize(best, BUSY);
	}

	return best;
}



int meminit(void* pMemory, int size) {

	assert(pMemory != 0);
	assert(size >= 0);
	if (size < memgetminimumsize())
		return -1;

	memblock_t* firstBlock = (memblock_t*)(sizeof(memcontrol_t) + (char*)pMemory);
	MemoryStart = (memcontrol_t*)pMemory;


	MemoryStart->head = firstBlock;
	MemoryStart->freeSize = size - memgetminimumsize();
	MemoryStart->startSize = size;


	lastReturnedPointer = MemoryStart->head;


	firstBlock->size = size - memgetminimumsize();
	firstBlock->status = FREE;
	firstBlock->next = MY_NULL;
	firstBlock->prev = MY_NULL;
	SetControlSize(firstBlock, FREE);
	return 0;
}
void* memalloc(int size) {

	assert(size >= 0);
	if (IsCorrupted())
		assert(-1 > 0);
	if (MemoryStart == (memcontrol_t*)0 || MemoryStart->freeSize < size)
		return NULL;


	memblock_t* ptr = FindFirstSuitBlock(size, lastReturnedPointer);
	if (ptr == MY_NULL)
		return NULL;


	if (ptr->size < (size + SIZE_FULL_BLOCK)) {
		MemoryStart->freeSize -= ptr->size;
	}
	else
		MemoryStart->freeSize -= (size + SIZE_FULL_BLOCK);

	if (MemoryStart->freeSize < 0)
		MemoryStart->freeSize = 0;


	ptr->size = size;
	SetControlSize(ptr, BUSY);


	lastReturnedPointer = (memblock_t*)(ptr);
	return (void*)((char*)ptr + SIZE_PART_BLOCK);
}
void memfree(void* p) {
	assert(p != 0);

	memblock_t* ptr = (memblock_t*)((char*)p - SIZE_PART_BLOCK);
	int controlSize = *((int*)((char*)ptr + SIZE_PART_BLOCK + ptr->size));
	assert(controlSize <= 0);
	int sizeLeft = -1;
	int freeSize = 0;


	if (ptr->status != BUSY)
		ptr->size += ptr->status * (-1);

	freeSize = ptr->size;


	if (ptr != MemoryStart->head) {
		sizeLeft = *(int*)((char*)ptr - sizeof(int));
		if (sizeLeft > 0) {
			if (lastReturnedPointer == ptr)
				lastReturnedPointer = ptr->prev;

			ptr->prev->size += ptr->size + SIZE_FULL_BLOCK;
			ptr->prev->next = ptr->next;

			if (ptr->next != MY_NULL) {
				ptr->next->prev = ptr->prev;
			}
			ptr = ptr->prev;

			freeSize += SIZE_FULL_BLOCK;
		}
	}

	if (ptr->next != NULL && ptr->next->status == FREE) {
		if (lastReturnedPointer == ptr->next)
			lastReturnedPointer = ptr;

		ptr->size += ptr->next->size + SIZE_FULL_BLOCK;
		ptr->next = ptr->next->next;
		if (ptr->next != MY_NULL) {
			ptr->next->prev = ptr;
		}

		freeSize += SIZE_FULL_BLOCK;
	}

	SetControlSize(ptr, FREE);
	SetFreeBytes((char*)ptr + SIZE_PART_BLOCK, (char*)ptr + SIZE_PART_BLOCK + ptr->size);
	ptr->status = FREE;

	MemoryStart->freeSize += freeSize;
}
void memdone(void) {
	SetFreeBytes((void*)((char*)MemoryStart->head - sizeof(memcontrol_t)), (void*)((char*)MemoryStart->head - sizeof(memcontrol_t) + MemoryStart->startSize));
}
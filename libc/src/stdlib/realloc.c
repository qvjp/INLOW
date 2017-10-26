#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include "malloc.h"

static void changeChunkSize(Chunk* chunk, ssize_t sizeDiff)
{
	Chunk* next = chunk->next;

	Chunk* newNextChunk = (Chunk*) ((uintptr_t) next + sizeDiff);
	memmove(newNextChunk, next, sizeof(Chunk));
	chunk->next = newNextChunk;

	chunk->size += sizeDiff;
	newNextChunk->size -= sizeDiff;
}

void* realloc(void* addr, size_t size)
{
	if (addr == NULL)
			return malloc(size);

	__lockHeap();

	Chunk* chunk = (Chunk*) addr - 1;
	assert(chunk->magic == MAGIC_USED_CHUNK);

	if (size == 0)
			size = 1;
	size = alignUp(size, alignof(max_align_t));

	ssize_t sizeDiff = size - chunk->size;

	Chunk* next = chunk->next;

	if (sizeDiff == 0)
	{
		__unlockHeap();
		return addr;
	}

	if (next->magic == MAGIC_FREE_CHUNK)
	{
		if ((sizeDiff > 0 && next->size > sizeDiff + sizeof(Chunk)) || sizeDiff < 0)
		{
			changeChunkSize(chunk, sizeDiff);
			__unlockHeap();
			return addr;
		}
	}

	__unlockHeap();

	// Create a new chunk and copy the contents to it
	void* newAddress = malloc(size);
	if (!newAddress)
			return NULL;
	size_t copySize = sizeDiff < 0 ? size : chunk->size;
	memcpy(newAddress, addr, copySize);
	free(addr);
	return newAddress;
}

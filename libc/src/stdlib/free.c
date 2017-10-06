#include <assert.h>
#include "malloc.h"

void free(void* addr)
{
	if (addr == NULL)
			return;
	__lockHeap();

	Chunk* chunk = (Chunk*) addr - 1;

	assert(chunk->magic == MAGIC_USED_CHUNK);

	chunk->magic = MAGIC_FREE_CHUNK;

	if (chunk->prev && chunk->prev->magic == MAGIC_FREE_CHUNK)
	{
		chunk = __unifyChunks(chunk->prev, chunk);
	}

	if (chunk->next->magic == MAGIC_FREE_CHUNK)
	{
		chunk = __unifyChunks(chunk, chunk->next);
	}

	if (chunk->prev == NULL && chunk->next->magic == MAGIC_END_CHUNK)
	{
		Chunk* bigChunk = chunk - 1;
		assert(bigChunk->magic == MAGIC_BIG_CHUNK);
		if (bigChunk->prev)
		{
			bigChunk->prev->next = bigChunk->next;
		}
		if (bigChunk->next)
		{
			bigChunk->next->prev = bigChunk->prev;
		}
		unmapPages(bigChunk, bigChunk->size / PAGESIZE);
	}

	__unlockHeap();
}

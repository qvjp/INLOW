#include <stdalign.h>
#include <stddef.h>
#include "malloc.h"

void* malloc(size_t size)
{
	if (size == 0)
			size = 1;
	size_t chunksSize = sizeof(Chunk);
	size = alignUp(size, alignof(max_align_t));
	size_t totalSize = chunksSize + size;

	__lockHeap();

	Chunk* currentBigChunk = firstBigChunk;
	Chunk* currentChunk = currentBigChunk + 1;

	while (1)
	{
		switch (currentChunk->magic)
		{
			case MAGIC_FREE_CHUNK:
					if (currentChunk->size >= totalSize)
					{
						if(currentChunk->size > totalSize + chunksSize)
						{
							__splitChunk(currentChunk, size);
						}
						currentChunk->magic = MAGIC_USED_CHUNK;
						__unlockHeap();
						return (void*) (currentChunk + 1);
					}
					else
					{
						currentChunk = currentChunk->next;	
					}
					break;
			case MAGIC_USED_CHUNK:
					currentChunk = currentChunk->next;
					break;
			case MAGIC_END_CHUNK:
					if (currentBigChunk->next)
					{
						currentBigChunk = currentBigChunk->next;
						currentChunk = currentBigChunk + 1;
					}
					else
					{
						currentBigChunk = __allocateBigChunk(currentBigChunk, totalSize);
						currentChunk = currentBigChunk + 1;
						if(! currentBigChunk)
						{
							__unlockHeap();
							return NULL;
						}
					}
					break;
			default:
					__unlockHeap();
					return NULL;
		}
	}
}

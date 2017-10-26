#include <errno.h>
#include <stdalign.h>
#include <stddef.h>
#include "malloc.h"

void* malloc(size_t size)
{
	if (size == 0)
			size = 1;
	size = alignUp(size, alignof(max_align_t));

	__lockHeap();

	Chunk* currentBigChunk = __firstBigChunk;
	Chunk* currentChunk = currentBigChunk + 1;

	while (1)
	{
		switch (currentChunk->magic)
		{
			case MAGIC_FREE_CHUNK:
					if (currentChunk->size >= size)
					{
						if(currentChunk->size > size + sizeof(Chunk))
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
						currentBigChunk = __allocateBigChunk(currentBigChunk, size);
						currentChunk = currentBigChunk + 1;
						if(! currentBigChunk)
						{
							__unlockHeap();
							errno = ENOMEM;
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

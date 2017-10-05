#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>

#if 1 //kernel implementation
extern void* __mapPages(size_t);
extern void __unmapPages(void*, size_t);

# define mapPages(nPages) __mapPages(nPages)
# define unmapPages(addr, nPages) __unmapPages(addr, nPages)
#endif

typedef struct Chunk
{
	size_t magic;
	size_t size;
	struct Chunk* prev;
	struct Chunk* next;
} Chunk;

#define MAGIC_BIG_CHUNK 0xC001C0DE
#define MAGIC_FREE_CHUNK 0xBEEFBEEF
#define MAGIC_USED_CHUNK 0xDEADBEEF
#define MAGIC_END_CHUNK 0xDEADDEAD

#define PAGESIZE 0x1000

#define alignUp(val,alignment) ((((val) - 1) & ~((alignment) - 1)) + (alignment))

extern Chunk* firstBigChunk;

Chunk* __allocateBigChunk(Chunk* lastBigChunk, size_t size);
void __splitChunk(Chunk* chunk, size_t size);
Chunk* __unifyChunks(Chunk* first, Chunk* second);

void __lockHeap(void);
void __unlockHeap(void);

#endif

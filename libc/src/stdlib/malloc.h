#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>

#if __is_inlow_libc //kernel implementation
#include <sys/mman.h>
#define mapMemory(size) mmap(NULL, size, PROT_READ | PROT_WRITE, \
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
#define unmapMemory(addr, size) munmap(addr, size)
#else/* if __is_inlow_libk */

extern void* __mapMemory(size_t);
extern void __unmapMemory(void*, size_t);

#define mapMemory(size) __mapMemory(size)
#define unmapMemory(addr, size) __unmapMemory(addr, size)
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

extern Chunk* __firstBigChunk;

Chunk* __allocateBigChunk(Chunk* lastBigChunk, size_t size);
void __splitChunk(Chunk* chunk, size_t size);
Chunk* __unifyChunks(Chunk* first, Chunk* second);

void __lockHeap(void);
void __unlockHeap(void);

#endif

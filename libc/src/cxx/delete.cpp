#include <stdlib.h>

void operator delete(void* addr)
{
	free(addr);
}
void operator delete[](void* addr)
{
	free(addr);
}
void operator delete(void* addr,size_t)
{
	free(addr);
}
void operator delete[](void* addr, size_t)
{
	free(addr);
}

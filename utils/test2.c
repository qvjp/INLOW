#include <stdio.h>

int main(int argc, char* argv[])
{
	puts("Hello ! I am the other process.");
	printf("argc = %u, argv = 0x%p\n", argc, argv);
	for (int i = 0; i < argc; i++)
			printf("argv[%u] = %s\n", i, argv[i]);
	return argc;
}

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
	printf("Hello %s from userspace!\n", "World");

	FILE* file = fopen("hello", "r");
	char* buffer = malloc(10);
	while (fgets(buffer, 7, file))
			printf("Read from file: %s\n", buffer);
	fgets(buffer, 10, stdin);
	printf("\nYou wrote: %s\n", buffer);
	free(buffer);
	return 40;
}

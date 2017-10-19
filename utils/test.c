#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
	printf("Hello %s from userspace!\n", "World");

	char* buffer = malloc(10);
	fgets(buffer, 10, stdin);
	printf("\nYou wrote: %s\n", buffer);
	free(buffer);
	return 40;
}

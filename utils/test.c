#include <stdio.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	printf("Hello %s from userspace!\n", "World");
	char buffer[10];
	fgets(buffer, sizeof(buffer), stdin);
	printf("\nYou wrote: %s\n", buffer);
	return 40;
}

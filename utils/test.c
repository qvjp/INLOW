#include <stdio.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	printf("Hello %s from userspace!\n", "World");
	char buffer[10];
	buffer[9] = '\0';
	read(0, buffer, 9);
	printf("\nYou wrote: %s\n", buffer);
	return 40;
}

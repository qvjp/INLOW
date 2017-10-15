#include <stdio.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	printf("Hello %s from userspace!\n", "World");
	printf("%u = 0x%x = 0%o\n", 42, 42, 42);
	return 40;
}

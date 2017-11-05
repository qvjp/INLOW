#include <stdio.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	printf("\e[H\e[2J");
}

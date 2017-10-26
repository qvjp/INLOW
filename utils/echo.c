#include <stdio.h>

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		fputs(argv[i], stdout);
		if (i + 1 < argc)
		{
			fputc(' ', stdout);
		}
	}
	fputc('\n', stdout);
}

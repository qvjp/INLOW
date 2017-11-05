#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		errx(1, "Missing operand");
	}
	unsigned long time = strtoul(argv[1], NULL, 10);
	if (errno)
		err(1, "strtoul: '%s'", argv[1]);
	sleep(time);
}

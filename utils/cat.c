#include "utils.h"
#include <err.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static bool failed = false;

static void cat(const char* path)
{
	int fd;
	if (strcmp(path, "-") == 0)
	{
		fd = 0;
	}
	else
	{
		fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			warn("'%s'", path);
			failed = true;
			return;
		}
	}
	
	while (true)
	{
		char buffer[1];
		ssize_t readSize = read(fd, buffer, sizeof(buffer));
		if (readSize < 0)
		{
			warn("'%s'", path);
			break;
		}
		else if (readSize == 0)
		{
			break;
		}

		ssize_t writtenSize = write(1, buffer, readSize);
		if (writtenSize < 0)
		{
			err(1, "write");
		}
	}
	if (fd != 0)
			close(fd);
}

int main(int argc, char* argv[])
{
	struct option longopts[] = 
	{
		{ "help", no_argument, 0, '?'},
		{ "version", no_argument, 0, 1},
		{ 0, 0, 0, 0}
	};
	int c;
	while ((c = getopt_long(argc, argv, "u?", longopts, NULL)) != -1)
	{
		switch (c)
		{
			case 1:
					return version(argv[0]);
			case 'u':
					break;
			case '?':
					return help(argv[0], "[OPTION] [FILE...]\n"
									" -u,            (ignored)\n"
									" -?, --help     display this help\n"
									"     --version  display veresion info");
		}
	}
	if (optind < argc)
	{
		for (int i = optind; i < argc; i++)
		{
			cat(argv[i]);
		}
	}
	else
	{
		cat("-");
	}
	return failed ? 1 : 0;
}

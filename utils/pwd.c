#include "utils.h"
#include <err.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static bool isAbsolutePath(const char* path);

int main(int argc, char* argv[])
{
	struct option longopts[] = 
	{
		{ "help", no_argument, 0, '?' },
		{ "version", no_argument, 0, 1},
		{ 0, 0, 0, 0 }
	};

	bool logical = true;
	int c;
	while ((c = getopt_long(argc, argv, "LP?", longopts, NULL)) != -1)
	{
		switch (c)
		{
			case 1:
					return version(argv[0]);
			case 'L':
					logical = true;
					break;
			case 'p':
					logical = false;
					break;
			case '?':
					return help(argv[0], "[OPTIONS]\n"
									" -L                print logical path\n"
									" -P                print physical path\n"
									" -?, --help        display this help\n"
									"     --version     display version info");
		}
	}

	if (optind < argc)
	{
		errx(1, "extra operand '%s'", argv[optind]);
	}
	const char* pwd = getenv("PWD");
	if (logical && pwd && isAbsolutePath(pwd))
	{
		puts(pwd);
	}
	else
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			err(1, "getcwd");
		puts(pwd);
	}
}

static bool isAbsolutePath(const char* path)
{
	if (*path != '/')
		return false;
	while (*path)
	{
		if (path[0] == '.' && (path[1] == '/' || path[1] == '\0' ||
			(path[1] == '.' && (path [2] == '/' || path[2] == '\0'))))
		{
			return false;
		}
		while (*path && *path++ != '/');
	}
	return true;
}

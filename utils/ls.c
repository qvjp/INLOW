#include "utils.h"
#include <dirent.h>
#include <err.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static void listDirectory(const char* path);

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
			case '?':
					return help(argv[0], "[OPTION] [FILE...]\n"
									" -?, --help     display this help\n"
									"     --version  display veresion info");
		}
	}
	if (optind >= argc)
	{
		listDirectory(".");
	}

	for (int i = optind; i < argc; i++)
	{
		struct stat st;
		if (stat(argv[i], &st) < 0)
		{
			warn("stat: '%s'", argv[i]);
			continue;
		}

		if (S_ISDIR(st.st_mode))
				listDirectory(argv[i]);
		else
				puts(argv[i]);
	}
}

static void listDirectory(const char* path)
{
	DIR* dir = opendir(path);
	if (!dir)
	{
		warn("opendir: '%s'", path);
		return;
	}
	struct dirent* entry = readdir(dir);

	while (entry)
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				// puts("hello\0");
				puts(entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
}

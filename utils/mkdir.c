#include "utils.h"
#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


static void createDirectory(const char* path, bool parents);

int main(int argc, char* argv[])
{
    struct option longopts[] =
	{
        { "parents", no_argument, 0, 'p' },
        { "help", no_argument, 0, '?' },
        { "version", no_argument, 0, 1 },
        { 0, 0, 0, 0 }
    };

    bool parents = false;

    int c;
    while ((c = getopt_long(argc, argv, "p?", longopts, NULL)) != -1)
	{
        switch (c)
		{
        case 1:
            return version(argv[0]);
        case 'p':
            parents = true;
            break;
        case '?':
            return help(argv[0], "[OPTIONS] DIR...\n"
                    "  -p, --parents                create intermediate directories as required\n"
                    "  -?, --help                   display this help\n"
                    "      --version                display version info");
        }
    }

    if (optind >= argc) errx(1, "missing operand");

    for (int i = optind; i < argc; i++)
	{
        createDirectory(argv[i], parents);
    }
}

static void createDirectory(const char* path, bool parents)
{
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0)
	{
        if (parents && errno == EEXIST)
		{
            struct stat st;
            if (stat(path, &st) < 0) err(1, "stat");
            if (!S_ISDIR(st.st_mode))
			{
                errno = EEXIST;
                err(1, "'%s'", path);
            }
        } else if (parents && errno == ENOENT)
		{
            char* parentName = strdup(path);
            if (!path) err(1, "strdup");
            parentName = dirname(parentName);
            createDirectory(parentName, parents);
            free(parentName);
            createDirectory(path, parents);
        }
		else
		{
            err(1, "'%s'", path);
        }
    }
}

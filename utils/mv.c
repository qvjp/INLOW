#include "utils.h"
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static bool getConfirmation(void);
static bool move(int sourceFd, const char* sourceName, const char* sourcePath,
        int destFd, const char* destName, const char* destPath, bool prompt);

int main(int argc, char* argv[])
{
    struct option longopts[] =
	{
        { "force", no_argument, 0, 'f' },
        { "interactive", no_argument, 0, 'i' },
        { "help", no_argument, 0, '?' },
        { "version", no_argument, 0, 1 },
        { 0, 0, 0, 0 }
    };

    bool prompt = false;
    int c;
    while ((c = getopt_long(argc, argv, "fi?", longopts, NULL)) != -1)
	{
        switch (c)
		{
        case 1:
            return version(argv[0]);
        case 'f':
            prompt = false;
            break;
        case 'i':
            prompt = true;
            break;
        case '?':
            return help(argv[0], "[OPTIONS] SOURCE... DESTINATION\n"
                    "  -f, --force              do not prompt\n"
                    "  -i, --interactive        prompt before overwrite\n"
                    "  -?, --help               display this help\n"
                    "      --version            display version info");
        }
    }

    if (optind >= argc) errx(1, "missing source operand");
    if (optind == argc - 1) errx(1, "missing destination operand");

    const char* destination = argv[argc - 1];
    if (optind == argc - 2)
	{
        struct stat destSt;
        int statResult = stat(destination, &destSt);
        if (statResult < 0 && errno != ENOENT)
		{
            err(1, "stat: '%s'", destination);
        }
		else if (statResult < 0 || !S_ISDIR(destSt.st_mode))
		{
            bool success = move(AT_FDCWD, argv[optind], argv[optind], AT_FDCWD,
                    destination, destination, prompt);
            return success ? 0 : 1;
        }
    }

    bool success = true;

    int destFd = open(destination, O_SEARCH | O_DIRECTORY);
    if (destFd < 0) err(1, "open: '%s'", destination);
    for (int i = optind; i < argc - 1; i++)
	{
        const char* source = argv[i];
        char* sourceCopy = strdup(source);
        if (!sourceCopy) err(1, "strdup");
        char* destName = basename(sourceCopy);
        if (strcmp(destName, "/") == 0)
		{
            destName = ".";
        }
        char* destPath = malloc(strlen(destination) + strlen(destName) + 2);
        if (!destPath) err(1, "malloc");
        stpcpy(stpcpy(stpcpy(destPath, destination), "/"), destName);
        success &= move(AT_FDCWD, source, source, destFd, destName, destPath, prompt);
        free(sourceCopy);
        free(destPath);
    }
    return success ? 0 : 1;
}

static bool getConfirmation(void)
{
    char* buffer = NULL;
    size_t size = 0;
    if (getline(&buffer, &size, stdin) <= 0) return false;
    bool result = (*buffer == 'y' || *buffer == 'Y');
    free(buffer);
    return result;
}

static bool move(int sourceFd, const char* sourceName, const char* sourcePath,
        int destFd, const char* destName, const char* destPath, bool prompt)
{
    struct stat sourceSt, destSt;
    if (fstatat(sourceFd, sourceName, &sourceSt, 0) < 0)
	{
        warn("stat: '%s'", sourcePath);
        return false;
    }
    bool destExists = true;
    if (fstatat(destFd, destName, &destSt, 0) < 0)
	{
        if (errno != ENOENT)
		{
            warn("stat: '%s'", destPath);
            return false;
        }
        destExists = false;
    }

    if (destExists && prompt)
	{
        fprintf(stderr, "%s: overwrite '%s'? ", program_invocation_short_name, destPath);
        if (!getConfirmation())
			return true;
    }

    if (destExists && sourceSt.st_dev == destSt.st_dev &&
            sourceSt.st_ino == destSt.st_ino)
	{
        warnx("'%s' and '%s' are the same file", sourcePath, destPath);
        return false;
    }

    if (renameat(sourceFd, sourceName, destFd, destName) == 0)
	{
        return true;
    }
	else if (errno != EXDEV)
	{
        warn("cannot move '%s' to '%s'", sourcePath, destPath);
        return false;
    }

    warnx("cannot move '%s' to '%s': moving between file systems is not yet"
            " implemented");
    return false;
}

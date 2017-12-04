#include "utils.h"
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

static int exitStatus = 0;

static bool getConfirmation(void);
static bool removeFile(const char* filename, bool force, bool prompt, bool recursive);
static bool removeRecursively(const char* dirname, bool force, bool prompt);

int main(int argc, char* argv[])
{
	struct option longopts[] =
	{
		{ "force", no_argument, 0, 'f' },
		{ "recursive", no_argument, 0, 'r' },
		{ "help", no_argument, 0, '?' },
		{ "version", no_argument, 0, 1 },
		{ 0, 0, 0, 0 }
	};

	bool force = false;
	bool prompt = false;
	bool recursive = false;
	int c;
	while ((c = getopt_long(argc, argv, "fiRr?", longopts, NULL)) != -1)
	{
		switch (c)
		{
			case 1:
				return version(argv[0]);
			case 'f':
				force = true;
				prompt = false;
				break;
			case 'i':
				force = false;
				prompt = true;
				break;
			case 'R':
			case 'r':
				recursive = true;
				break;
			case '?':
				return help(argv[0], "[OPTIONS] FILE...\n"
								" -f, --force            ignore nonexistent files\n"
								" -i                     prompt for confirmation\n"
								" -r, -R, --recursive    recursively remove direntories\n"
								" -?, --hekp             display this help\n"
								"     --version          display version info");
		}
	}

	if (optind >= argc)
		errx(1, "missing operand");

	for (int i = optind; i < argc; i++)
	{
		char* nameCopy = strdup(argv[i]);
		char* base = basename(nameCopy);
		if (strcmp(base, "/") == 0)
		{
			warnx("cannot remove root directory");
			free(nameCopy);
			exitStatus = 1;
			continue;
		}

		if (strcmp(base, ".") == 0 || strcmp(base, "..") == 0)
		{
			warnx("cannot remove '%s'", argv[i]);
			free(nameCopy);
			exitStatus = 1;
			continue;
		}

		free(nameCopy);
		removeFile(argv[i], force, prompt, recursive);
	}
	return exitStatus;
}

static bool getConfirmation(void)
{
	char* buffer = NULL;
	size_t size = 0;
	if (getline(&buffer, &size, stdin) <= 0)
		return false;
	bool result = (*buffer == 'y' || *buffer == 'Y');
	free(buffer);
	return result;
}

static bool removeFile(const char* filename, bool force, bool prompt, bool recursive)
{
	struct stat st;
	if (stat(filename, &st) < 0)
	{
		if (force && errno == ENOENT)
			return true;
		warn("cannot remove '%s'", filename);
		exitStatus = 1;
		return false;
	}
	if (S_ISDIR(st.st_mode))
	{
		if (!recursive)
		{
			errno = EISDIR;
			warn("'%s'", filename);
			exitStatus = 1;
			return false;
		}
		if (prompt)
		{
			fprintf(stderr, "%s: descend into directory '%s'?", program_invocation_short_name, filename);
			if (!getConfirmation())
				return false;
		}
		bool result = removeRecursively(filename, force, prompt);

		if (prompt)
		{
			fprintf(stderr, "%s: remove directory '%s'?", program_invocation_short_name, filename);
			if (!getConfirmation())
				return false;
		}
		if (rmdir(filename) < 0)
		{
			warn("cannot remove '%s'", filename);
			exitStatus = 1;
			return false;
		}
		return result;
	}
	else
	{
		if (prompt)
		{
			fprintf(stderr, "%s: remove file '%s'? ", program_invocation_short_name, filename);
			if (!getConfirmation())
				return false;
		}
		if (unlink(filename) < 0)
		{
			warn("cannot remove '%s'", filename);
			exitStatus = 1;
			return false;
		}
		return true;
	}
}

static bool removeRecursively(const char* dirname, bool force, bool prompt)
{
	DIR* dir = opendir(dirname);
	if (!dir)
	{
		warn("fopendir: '%s'", dirname);
		exitStatus = 1;
		return false;
	}
	size_t dirnameLength = strlen(dirname);
	errno = 0;
	struct dirent* dirent = readdir(dir);
	while (dirent)
	{
		if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0)
		{
			dirent = readdir(dir);
			continue;
		}
		char* name = malloc(dirnameLength + strlen(dirent->d_name) + 2);
		if (!name)
			err(1, "malloc");
		stpcpy(stpcpy(stpcpy(name, dirname), "/"), dirent->d_name);

		bool result = removeFile(name, force, prompt, true);
		free(name);

		if (!result)
		{
			closedir(dir);
			return false;
		}
		rewinddir(dir);
		errno = 0;
		dirent = readdir(dir);
	}
	if (errno)
	{
		warn("readdir: '%s'", dirname);
		closedir(dir);
		exitStatus = 1;
		return false;
	}
	closedir(dir);
	return true;
}

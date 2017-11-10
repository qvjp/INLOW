#include "utils.h"
#include <err.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static int executeCommand(int argc, char* arguments[]);
static const char* getExecutablePath(const char* command);

static int cd(int argc, char* argv[]);

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

	char* buffer = NULL;
	size_t bufferSize = 0;

	while (true)
	{
		fputs("$>", stderr);
		ssize_t length = getline(&buffer, &bufferSize, stdin);
		if (buffer[length - 1] == '\n')
				buffer[length - 1] = '\0';

		size_t argumentCount = 1;
		for (size_t i = 0; buffer[i]; i++)
		{
			if (buffer[i] == ' ')
			{
				argumentCount++;
			}
		}

		char** arguments = malloc((argumentCount + 1) * sizeof(char*));
		if (!arguments)
				err(1, "malloc");
		char* token = strtok(buffer, " ");
		size_t argCount = 0;

		while (token)
		{
			arguments[argCount++] = token;
			token = strtok(NULL, " ");
		}
		arguments[argCount] = NULL;

		if (arguments[0])
			executeCommand(argCount, arguments);
		free(arguments);
	}
}
static int executeCommand(int argc, char* arguments[])
{
	const char* command = arguments[0];
	char argumentexit[] = "exit";
	if (strcmp(command,argumentexit) == 0)
	{
		exit(0);
	}

	if (strcmp(command, "cd") == 0)
	{
		return cd (argc, arguments);
	}

	pid_t pid = fork();

	if (pid < 0)
	{
		warn("fork");
		return -1;
	}
	else if (pid == 0)
	{
		if (!strchr(command, '/'))
		{
			command = getExecutablePath(command);
		}
		if (command)
		{
			execv(command, arguments);
			warn("execv: '%s'", command);
		}
		else
		{
			warnx("'%s': Command not fount", arguments[0]);
		}
		_Exit(127);
	}
	else
	{
		int status;
		if (waitpid(pid, &status, 0) == -1)
		{
			err(1, "waitpid");
		}
		return WEXITSTATUS(status);
	}
}

static const char* getExecutablePath(const char* command)
{
	size_t commandLength = strlen(command);
	const char* path = getenv("PATH");
	while (*path)
	{
		size_t length = strcspn(path, ":");
		char* buffer = malloc(commandLength + length + 2);
		if (!buffer)
			err(1, "malloc");
		memcpy(buffer, path, length);
		buffer[length] = '/';
		memcpy(buffer + length + 1, command, commandLength);
		buffer[commandLength + length + 1] = '\0';

		if (access(buffer, X_OK) == 0)
		{
			return buffer;
		}
		free(buffer);
		path += length + 1;
	}
	return NULL;
}

static int cd(int argc, char* argv[])
{
	const char* newCwd;
	if (argc >= 2)
	{
		newCwd = argv[1];
	}
	else
	{
		newCwd = getenv("HOME");
		if (!newCwd)
		{
			warnx("HOME not set");
			return 1;
		}
	}

	if (chdir(newCwd) == -1)
	{
		warn("cd: '%s'", newCwd);
		return 1;
	}

	return 0;
}

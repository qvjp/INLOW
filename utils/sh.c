#include "utils.h"
#include <err.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static char* pwd;
static size_t pwdSize;

static int executeCommand(int argc, char* arguments[]);
static const char* getExecutablePath(const char* command);
static void updateLogicalPwd(const char* path);

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

	pwd = getenv("PWD");
	if (pwd)
	{
		pwd = strdup(pwd);
	}
	else
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			setenv("PWD", pwd, 1);
		}
	}
	pwdSize = pwd ? strlen(pwd) : 0;

	char* buffer = NULL;
	size_t bufferSize = 0;

	const char* username = getlogin();
	if (!username)
	{
		username = "?";
	}
	char hostname[HOST_NAME_MAX + 1];
	if (gethostname(hostname, sizeof(hostname)) < 0)
	{
		strcpy(hostname, "?");
	}

	while (true)
	{
		//fputs("$>", stderr);
		fprintf(stderr, "\e[33m%s@%s \e[1;36m%s $>\e[22;39m", username, hostname, pwd ? pwd : ".");
		ssize_t length = getline(&buffer, &bufferSize, stdin);
		if (length < 0)
		{
			putchar('\n');
			if (feof(stdin))
					exit(0);
			err(1, NULL);
		}
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

		if (WIFSIGNALED(status))
		{
			int signum = WTERMSIG(status);
			if (signum == SIGINT)
			{
				fputc('\n', stderr);
			}
			else
			{
				fprintf(stderr, "%s\n", strsignal(signum));
			}
			return 128 + signum;
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

static void updateLogicalPwd(const char* path)
{
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		pwdSize = pwd ? strlen(pwd) : 0;
		return;
	}

	if (*path == '/')
	{
		strcpy(pwd, "/");
	}

	size_t newSize = strlen(pwd) + strlen(path) + 2;
	if (newSize > pwdSize)
	{
		char* newPwd = realloc(pwd, newSize);
		if (!newPwd)
		{
			free(pwd);
			pwd = NULL;
			return;
		}
		pwd = newPwd;
		pwdSize = newSize;
	}
	char* pwdEnd = pwd + strlen(pwd);
	const char* component = path;
	size_t componentLength = strcspn(component, "/");

	while (*component)
	{
		if (componentLength == 0 || (componentLength == 1 && strncmp(component, ".", 1) == 0))
		{
			
		}
		else if (componentLength == 2 && strncmp(component, "..", 2) == 0)
		{
			char* lastSlash = strrchr(pwd, '/');
			if (lastSlash == pwd)
			{
				pwdEnd = pwd + 1;
			}
			else if (lastSlash)
			{
				pwdEnd = lastSlash;
			}
		}
		else
		{
			if (pwdEnd != pwd + 1)
			{
				*pwdEnd++ = '/';
			}
			memcpy(pwdEnd, component, componentLength);
			pwdEnd += componentLength;
		}
		component += componentLength + 1;
		componentLength = strcspn(component, "/");
	}
	*pwdEnd = '\0';
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

	updateLogicalPwd(newCwd);
	if (!pwd || setenv("PWD", pwd, 1) < 0)
	{
		unsetenv("PWD");
	}

	return 0;
}

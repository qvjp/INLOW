#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
	while (true)
	{
		fputs("$ ", stderr);
		char buffer[81];
		fgets(buffer, sizeof(buffer), stdin);
		size_t length = strlen(buffer);

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
		char* str = strtok(buffer, " ");
		for (size_t i = 0; i < argumentCount; i++)
		{
			arguments[i] = str;
			str = strtok(NULL, " ");
		}
		arguments[argumentCount] = NULL;

		char argumentexit[] = "exit";
		if (strcmp(arguments[0], argumentexit) == 0)
		{
			exit(0);
		}
		pid_t pid = fork();

		if (pid < 0)
				fputs("fork() failed\n", stderr);
		else if (pid == 0)
		{
			execv(arguments[0], arguments);
			fputs("Coummand not found\n", stderr);
			_Exit(127);
		}
		else
		{
			int status;
			waitpid(pid, &status, 0);
		}
		free(arguments);
	}
}

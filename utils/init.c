#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	if (getpid() != 1)
	{
		errx(1, "PID is not 1");
	}
	if (setenv("PATH", "/bin:/sbin", 1) < 0)
	{
		err(1, "setenv");
	}
	pid_t childPid = fork();
	if (childPid < 0)
	{
		err(1, "fork");
	}
	if (childPid == 0)
	{
		const char* args[] = { "sh", NULL };
		execv("/bin/sh", (char**) args);
		err(1, "execv: '/bin/sh'");
	}

	while (true)
	{
		int status;
		wait(&status);
	}
}

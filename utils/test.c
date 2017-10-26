#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
	puts("Now forking the process.");
	pid_t pid = fork();
	if (pid == -1)
	{
		puts("fork failed");
	}
	else if (pid == 0)
	{
		puts("Executing new process");
		execl("/bin/test2", "test2", "Hello", "I'm Test2", NULL);
		puts("execl failed!\n");
	}
	else
	{
		printf("New child process has pid %u. Waiting...\n", pid);
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			printf("Child process exited with status %u\n", WEXITSTATUS(status));
		}
	}
	return 40;
}

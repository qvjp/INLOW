#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
	printf("Hello %s from userspace!\n", "World");

	pid_t pid = fork();
	
	if (pid == -1)
	{
		printf("fork() failed\n");
	}
	else if (pid == 0)
	{
		printf("Hello from child process!\n");
	}
	else
	{
		printf("Hello from parent process. The new process has pid %u.\n", pid);
	}
	return 40;
}

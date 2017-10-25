#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	

	puts("Start the other program?");
	
	char yes[] = "yes\n";
	char buffer[81];
	fgets(buffer, sizeof(buffer), stdin);

	if (strcmp(buffer, yes) == 0)
	{
		char* const args[] = { NULL };
		execv("/bin/test2", args);
		puts("execv failed!\n");
	}
	return 40;
}

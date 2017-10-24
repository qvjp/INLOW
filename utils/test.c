#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
	printf("Hello %s from userspace!\n", "World");

	char buffer[81];
	char exit[] = "exit";
	
	while (1)
	{
		fgets(buffer, sizeof(buffer), stdin);
 		size_t length = strlen(buffer);
		if (buffer[length - 1] == '\n')
		{
			buffer[length - 1] = '\0';
		}

		if (strcmp(buffer, exit) == 0)
		{
			puts("Exiting.");
			return 40;
		}

		
		FILE* file = fopen(buffer, "r");

		if (!file)
		{
			printf("Failed to open file '%s'\n", buffer);
			continue;
		}
		
		while (fgets(buffer, sizeof(buffer), file))
				fputs(buffer, stdout);
		fclose(file);
	}
}
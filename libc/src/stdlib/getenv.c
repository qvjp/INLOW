#include <stdlib.h>
#include <string.h>

extern char** environ;

char* getenv(const char* name)
{
	size_t nameLength = strlen(name);
	char** envp = environ;
	while (*envp)
	{
		size_t length = strcspn(*envp, "=");
		if (length == nameLength && strncmp(name, *envp, length) == 0)
		{
			return *envp + length + 1;
		}
		envp++;
	}

	return NULL;
}

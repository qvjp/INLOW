#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char** environ;
extern char** __mallocedEnviron;
extern size_t __environLength;

int unsetenv(const char* name)
{
	if (!*name || strchr(name, '='))
	{
		errno = EINVAL;
		return -1;
	}

	if (!environ)
		return 0;

	size_t nameLength = strlen(name);

	for (size_t i = 0; environ[i]; i++)
	{
		if (nameLength == strcspn(environ[i], "=") && strncmp(environ[i], name, nameLength) == 0)
		{
			if (environ == __mallocedEnviron)
			{
				free(environ[i]);
				environ[i] = environ[--__environLength];
				environ[__environLength] = NULL;
			}
			else
			{
				for (size_t j = i; environ[j]; j++)
				{
					environ[j] = environ[j + 1];
				}
			}
			i--;
		}
	}
	return 0;
}

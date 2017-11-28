#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

extern char** environ;
extern char** __mallocedEnviron;
extern size_t __environLength;
extern size_t __environSize;

static bool resetEnviron(void)
{
	if (__mallocedEnviron)
	{
		while (*__mallocedEnviron)
			free(*__mallocedEnviron++);
		free(__mallocedEnviron);
	}
	size_t length = 0;
	if (environ)
	{
		char** envp = environ;
		while (*envp++)
		{
			length++;
		}
	}
	size_t allocationSize = length > 15 ? length + 1 : 16;

	__mallocedEnviron = malloc(allocationSize * sizeof(char*));
	if (!__mallocedEnviron)
		return false;
	__environLength = length;
	__environSize = allocationSize;
	for (size_t i = 0; i < length; i++)
		__mallocedEnviron[i] = strdup(environ[i]);

	__mallocedEnviron[length] = NULL;
	environ = __mallocedEnviron;

	return true;
}

int setenv(const char* name, const char* value, int overwrite)
{
	if (!*name || strchr(name, '='))
	{
		errno = EINVAL;
		return -1;
	}
	if (!environ || environ != __mallocedEnviron)
	{
		if (!resetEnviron())
			return -1;
	}
	size_t nameLength = strlen(name);

	size_t i = 0;
	for (; environ[i]; i++)
	{
		if (nameLength == strcspn(environ[i], "=") && strncmp(environ[i], name, nameLength) == 0)
		{
			if (!overwrite)
				return 0;

			size_t entryLength = nameLength + strlen(value) + 2;
			char* newEntry = malloc(entryLength);
			if (!newEntry)
				return -1;
			stpcpy(stpcpy(stpcpy(newEntry, name), "="), value);
			free(environ[i]);
			environ[i] = newEntry;
			return 0;
		}
	}

	if (__environLength + 1 == __environSize)
	{
		char** newEnviron = reallocarray(environ, __environSize, 2 * sizeof(char*));
		if (!newEnviron)
			return -1;
		environ = __mallocedEnviron = newEnviron;
		__environSize *= 2;	
	}

	size_t entryLength = nameLength + strlen(value) + 2;
	environ[i] = malloc(entryLength);
	if (!environ[i])
		return -1;
	stpcpy(stpcpy(stpcpy(environ[i], name), "="), value);
	environ[i + 1] = NULL;
	__environLength++;
	return 0;
}

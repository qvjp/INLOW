#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

int execl(const char* path, const char* argv0, ...)
{
	int argc = 1;
	va_list ap;
	va_start(ap, argv0);
	while (va_arg(ap, char*))
	{
		argc++;
	}
	va_end(ap);

	// Create an array containing all argument
	
	char** argv = malloc((argc + 1) * sizeof(char*));

	va_start(ap, argv0);
	argv[0] = (char*) argv0;

	for (int i = 1; i < argc; i++)
	{
		argv[i] = va_arg(ap, char*);
	}
	argv[argc] = NULL;
	va_end(ap);
	execv(path, argv);

	free(argv);
	return -1;
}

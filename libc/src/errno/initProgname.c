#include <errno.h>

char* program_invocation_name;
char* program_invocation_short_name;

void __initProgname(char* argv[])
{
	program_invocation_name = argv[0] ? argv[0] : "";
	program_invocation_short_name = program_invocation_name;

	char* s = program_invocation_name;
	while (*s)
	{
		if (*s++ == '/')
				program_invocation_short_name = s;
	}
}

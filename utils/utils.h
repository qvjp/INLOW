#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#ifndef INLOW_VERSION
#define INLOW_VERSION ""
#endif

#define UNUSED __attribute__((unused))

static UNUSED int help(const char* argv0, const char* helpstr)
{
	printf("Usage: %s ", argv0);
	puts(helpstr);
	return 0;
}

static UNUSED int version(const char* argv0)
{
	printf("%s (INLOW) %s\n", argv0, INLOW_VERSION);
	return 0;
}

#endif

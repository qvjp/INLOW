#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

int uname(struct utsname* result)
{
	confstr(_CS_UNAME_SYSNAME, result->sysname, sizeof(result->sysname));
	strcpy(result->nodename, "inlow");
	confstr(_CS_UNAME_RELEASE, result->release, sizeof(result->release));
	confstr(_CS_UNAME_VERSION, result->version, sizeof(result->version));
	confstr(_CS_UNAME_MACHINE, result->machine, sizeof(result->machine));
	return 0;
}

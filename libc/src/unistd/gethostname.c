#include <string.h>
#include <unistd.h>

static const char* hostname = "inlow";
int gethostname(char* result, size_t size)
{
	strlcpy(result, hostname, size);
	return 0;
}

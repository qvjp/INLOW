#include <getopt.h>
#include <unistd.h>

static struct option emptyOptions[] = 
{
	{0, 0, 0, 0}
};

int getopt(int argc, char* const argv[], const char* optstring)
{
	return getopt_long(argc, argv, optstring, emptyOptions, NULL);
}

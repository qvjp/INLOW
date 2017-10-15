#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	const char* hello = "Hello World, from userspcae!\n";
	write(1, hello, strlen(hello));
	return 40;
}

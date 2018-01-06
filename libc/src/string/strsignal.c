#include <signal.h>
#include <string.h>

char* strsignal(int signum)
{
	switch (signum)
	{
		case SIGABRT:
			return "Aborted";
		case SIGALRM:
			return "Alarm clock";
		case SIGBUS:
			return "Bus error";
		case SIGCHLD:
			return "Child exited";
		case SIGCONT:
			return "Continued";
		case SIGFPE:
			return "Arithmetic exception";
		case SIGHUP:
			return "Hangup";
		case SIGILL:
			return "Illegal instruction";
		case SIGINT:
			return "Interrupt";
		case SIGKILL:
			return "Killed";
		case SIGPIPE:
			return "Broke pipe";
		case SIGQUIT:
			return "Quit";
		case SIGSEGV:
			return "Segmentation fault";
		case SIGSTOP:
			return "Stopped (signal)";
		case SIGSYS:
			return "Bad system call";
		case SIGTERM:
			return "Terminated";
		case SIGTRAP:
			return "Trace/breakpoint trap";
		case SIGTSTP:
			return "Stopped";
		case SIGTTIN:
			return "Stopped (tty input)";
		case SIGTTOU:
			return "Stopped (tty output)";
		case SIGURG:
			return "Urgent I/O condition";
		case SIGUSR1:
			return "User defined signal 1";
		case SIGUSR2:
			return "User defined signal 2";
		default:
			return "Unknown signal";
	}
}

#ifndef _INLOW_SIGNAL_H
#define _INLOW_SIGNAL_H

#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGABRT 6
#define SIGKILL 9
#define SIGALRM 14
#define SIGTERM 15

#define SIGBUS 4
#define SIGCHLD 5
#define SIGCONT 7
#define SIGFPE 8
#define SIGILL 10
#define SIGPIPE 11
#define SIGSEGV 12
#define SIGSTOP 13
#define SIGSYS 16
#define SIGTRAP 17
#define SIGTSTP 18
#define SIGTTIN 19
#define SIGTTOU 20
#define SIGURG 21
#define SIGUSR1 22
#define SIGUSR2 23

#define _NSIG 24

#define SIG_DFL ((void (*)(int)) 0)
#define SIG_IGN ((void (*)(int)) 1)
#define SIG_ERR ((void (*)(int)) -1)


#endif

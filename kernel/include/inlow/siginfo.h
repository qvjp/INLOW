#ifndef _INLOW_SIGINFO_H
#define _INLOW_SIGINFO_H

#include <inlow/types.h>

union sigval
{
	int sival_int;
	void* sival_ptr;
};

typedef struct {
	int si_signo;
	int si_code;
	__pid_t si_pid;
	__uid_t si_uid;
	void* si_addr;
	int si_status;
	union sigval si_value;
} siginfo_t;

#endif

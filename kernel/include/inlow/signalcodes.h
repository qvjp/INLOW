#ifndef _INLOW_SIGNALCODES_H
#define _INLOW_SIGNALCODES_H

#define SI_USER 1
#define SI_QUEUE 2
#define SI_TIMER 3
#define SI_ASYNCIO 4
#define SI_MESGQ 5
#define SI_KERNEL 6

#define ILL_ILLOPC 7
#define ILL_ILLOPN 8
#define ILL_ILLADR 9
#define ILL_ILLTRP 10
#define ILL_PRVOPC 11
#define ILL_PRVREG 12
#define ILL_COPEOC 13
#define ILL_BADSTK 14

#define FPE_INTDIV 7
#define FPE_INTOVF 8
#define FPE_FLTDIV 9
#define FPE_FLTOVF 10
#define FPE_FLTUND 11
#define FPE_FLTRES 12
#define FPE_FLTINV 13
#define FPE_FLTSUB 14

#define SEGV_MAPERR 7
#define SEGV_ACCERR 8

#define BUS_ADRALN 7
#define BUS_ADRERR 8
#define BUS_OBJERR 9

#define TRAP_BRKPT 7
#define TRAP_TRACE 8

#define CLD_EXITED 7
#define CLD_KILLED 8
#define CLD_DUMPED 9
#define CLD_TRAPPED 10
#define CLD_STOPPED 11
#define CLD_CONTINUED 12

#endif

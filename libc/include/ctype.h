#ifndef CTYPE_H_
#define CTYPE_H_

#include <sys/cdefs.h>
#define __need_locale_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C" {
#endif

int isspace(int);

#ifdef __cplusplus
}
#endif

#endif
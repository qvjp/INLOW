#ifndef _ERR_H
#define _ERR_H

#include <sys/cdefs.h>
#define __need__va_list
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

		__noreturn void err(int, const char*, ...);
		__noreturn void errc(int, int, const char*, ...);
		__noreturn void errx(int, const char*, ...);
		void warn(const char*, ...);
		void warnc(int, const char*, ...);
		void warnx(const char*, ...);

		__noreturn void verr(int, const char*, __gnuc_va_list);
		__noreturn void verrc(int, int, const char*, __gnuc_va_list);
		__noreturn void verrx(int, const char*, __gnuc_va_list);
		void vwarn(const char*, __gnuc_va_list);
		void vwarnc(int, const char*, __gnuc_va_list);
		void vwarnx(const char*, __gnuc_va_list);

#ifdef __cplusplus
}
#endif

#endif

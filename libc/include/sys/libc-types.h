#include <inlow/types.h>

#if defined(__need_FILE) && !defined(__FILE_defined)
typedef struct __FILE FILE;
#  define __FILE_defined
#endif

#if defined(__need_pid_t) && !defined(__pid_t_defined)
typedef __pid_t pid_t;
#  define __pid_t_defined
#endif

#if defined(__need_size_t) || defined(__need_NULL)
#  include <stddef.h>
#endif

#undef __need_FILE
#undef __need_pid_t
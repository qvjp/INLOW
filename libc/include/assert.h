#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C"
{
#endif

#undef assert

#ifdef NDEBUG
# define assert(ignore) ((void) 0)
#else
		__noreturn void __assertionFailure(const char*, const char*, unsigned int, const char*);
#define assert(assertion) ((assertion) ? (void) 0 : \
				__assertionFailure(#assertion, __FILE__, __LINE__, __func__))
#endif

#ifdef __cplusplus
}
#endif

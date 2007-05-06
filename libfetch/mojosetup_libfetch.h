#ifndef _INCL_MOJOSETUP_LIBFETCH_H_
#define _INCL_MOJOSETUP_LIBFETCH_H_

#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__APPLE__)
#ifndef FREEBSD
#define FREEBSD 1
#endif
#endif

// Things FreeBSD defines elsewhere...

#ifndef __FBSDID
#define __FBSDID(x)
#endif

#ifndef __DECONST
#define __DECONST(type, var) ((type) var)
#endif

#ifndef __unused
#define __unused
#endif

// apparently this is 17 in FreeBSD.
#ifndef MAXLOGNAME
#define MAXLOGNAME (17)
#endif



#include "../universal.h"
#include "../fileio.h"

#include <stdarg.h>
int MOJOSETUP_vasprintf(char **strp, const char *fmt, va_list ap);
#define vasprintf MOJOSETUP_vasprintf

#undef calloc
#define calloc(x, y) xmalloc(x * y)

#undef malloc
#define malloc(x) xmalloc(x)

#undef realloc
#define realloc(x, y) xrealloc(x, y)

#undef strdup
#define strdup(x) xstrdup(x)

#undef strncpy
#define strncpy(x, y, z) xstrncpy(x, y, z)

#endif

// end of mojosetup_libfetch.h ...


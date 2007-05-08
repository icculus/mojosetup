#ifndef _INCL_MOJOSETUP_LIBFETCH_H_
#define _INCL_MOJOSETUP_LIBFETCH_H_

#include "../universal.h"
#include "../platform.h"
#include "../fileio.h"

#include <stdarg.h>
#include <time.h>

int MOJOSETUP_vasprintf(char **strp, const char *fmt, va_list ap);
#define vasprintf MOJOSETUP_vasprintf
int MOJOSETUP_asprintf(char **strp, const char *fmt, ...) ISPRINTF(2,3);
#define asprintf MOJOSETUP_asprintf

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

#if !FREEBSD
#ifndef TCP_NOPUSH
#define TCP_NOPUSH TCP_CORK
#endif
#define EAUTH EPERM
boolean ishexnumber(char ch);
// Linux has had this since glibc 4.6.8, but doesn't expose it in the headers
//  without _XOPEN_SOURCE or _GNU_SOURCE, which breaks other things.
//  ...just force a declaration here, then.
#ifdef __linux__
char *strptime(const char *s, const char *format, struct tm *tm);
#endif
#endif

#endif

// end of mojosetup_libfetch.h ...


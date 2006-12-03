#ifndef _INCL_UNIVERSAL_H_
#define _INCL_UNIVERSAL_H_

// Include this file from everywhere...it provides basic type sanity, etc.

// Include the Holy Trinity...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// and some others...
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

// Just a define for when we have to glue to external code.
#define __MOJOSETUP__ 1

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef int boolean;
// !!! FIXME: this is begging for trouble.
#define true 1
#define false 0

// Command line access outside of main().
extern int GArgc;
extern char **GArgv;

// Static, non-stack memory for scratch work...not thread safe!
// !!! FIXME: maybe lose this.
extern uint8 scratchbuf_128k[128 * 1024];

// Call this for fatal errors that require immediate app termination.
//  Does not clean up, or translate the error string. Try to avoid this.
//  These are for crucial lowlevel issues that preclude any meaningful
//  recovery (GUI didn't initialize, etc)
// Doesn't return, but if it did, you can assume it returns zero, so you can
//  do:  'return panic("out of memory");' or whatnot.
int panic(const char *err);

// Call this for a fatal problem that attempts an orderly shutdown (system
//  is fully working, but config file is hosed, etc). (str) will be localized
//  if possible.
//  do:  'return fatal("scripting error");' or whatnot.
int fatal(const char *str);

// Call this to pop up a warning dialog box and block until user hits OK.
void warn(const char *str);

// Malloc replacements that blow up on allocation failure.
void *xmalloc(size_t bytes);
void *xrealloc(void *ptr, size_t bytes);
char *xstrdup(const char *str);

// !!! FIXME: just use strlcpy/strlcat instead...
// strncpy() that promises to null-terminate the string, even on overflow.
char *xstrncpy(char *dst, const char *src, size_t len);

// External plugins won't link against misc.c ...
#ifndef BUILDING_EXTERNAL_PLUGIN
#define malloc(x) DO_NOT_CALL_MALLOC__USE_XMALLOC_INSTEAD
#define calloc(x,y) DO_NOT_CALL_CALLOC__USE_XMALLOC_INSTEAD
#define realloc(x,y) DO_NOT_CALL_REALLOC__USE_XREALLOC_INSTEAD
#define strdup(x) DO_NOT_CALL_STRDUP__USE_XSTRDUP_INSTEAD
#define strncpy(x) DO_NOT_CALL_STRNCPY__USE_XSTRNCPY_INSTEAD
#endif

// Localization support.
const char *translate(const char *str);
#ifdef _
#undef _
#endif
#define _(x) translate(x)

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
#if (defined _MSC_VER)
#define __EXPORT__ __declspec(dllexport)
#elif (__GNUC__ >= 3)
#define __EXPORT__ __attribute__((visibility("default")))
#else
#define __EXPORT__
#endif
#endif  // DOXYGEN_SHOULD_IGNORE_THIS

#if 1
#define STUBBED(x) \
{ \
    static boolean seen_this = false; \
    if (!seen_this) \
    { \
        seen_this = true; \
        fprintf(stderr, "STUBBED: %s at %s:%d\n", x, __FILE__, __LINE__); \
    } \
}
#else
#define STUBBED(x)
#endif

// !!! FIXME: make this a real function.
#define dbgprintf printf

#define STATICARRAYLEN(x) ( (sizeof ((x))) / (sizeof ((x)[0])) )

#define DEFINE_TO_STR2(x) #x
#define DEFINE_TO_STR(x) DEFINE_TO_STR2(x)

#ifdef __cplusplus
}
#endif

#endif

// end of universal.h ...


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

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef int boolean;
#define true 1
#define false 0

// Command line access outside of main().
extern int GArgc;
extern char **GArgv;

// Static, non-stack memory for scratch work...not thread safe!
extern uint8 scratchbuf_128k[128 * 1024];

// Malloc replacements that blow up on allocation failure.
void *xmalloc(size_t bytes);
void *xrealloc(void *ptr, size_t bytes);
char *xstrdup(const char *str);

// External plugins won't link against misc.c ...
#ifndef BUILDING_EXTERNAL_PLUGIN
#define malloc(x) DO_NOT_CALL_MALLOC__USE_XMALLOC_INSTEAD
#define calloc(x,y) DO_NOT_CALL_CALLOC__USE_XMALLOC_INSTEAD
#define realloc(x,y) DO_NOT_CALL_REALLOC__USE_XREALLOC_INSTEAD
#define strdup(x) DO_NOT_CALL_STRDUP__USE_XSTRDUP_INSTEAD
#endif

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

#ifdef __cplusplus
}
#endif

#endif

// end of universal.h ...


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

extern int GArgc;
extern char **GArgv;

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


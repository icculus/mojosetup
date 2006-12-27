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

// These are likely to get stolen by some overzealous library header...
#ifdef boolean
#error Something is defining boolean. Resolve this before including universal.h.
#endif
#ifdef true
#error Something is defining true. Resolve this before including universal.h.
#endif
#ifdef false
#error Something is defining false. Resolve this before including universal.h.
#endif

typedef int boolean;
#define true 1
#define false 0

// Compiler-enforced printf() safety helper.
// This is appended to function declarations that use printf-style semantics,
//  and will make sure your passed the right params to "..." for the
//  format you specified, so gcc can catch programmer bugs at compile time.
#ifdef __GNUC__
#define ISPRINTF(x,y) __attribute__((format (printf, x, y)))
#else
#define ISPRINTF(x,y)
#endif

// Command line access outside of main().
extern int GArgc;
extern const char **GArgv;

// Build-specific details.
extern const char *GBuildVer;

// Static, non-stack memory for scratch work...not thread safe!
extern uint8 scratchbuf_128k[128 * 1024];

// Call this for fatal errors that require immediate app termination.
//  Does not clean up, or translate the error string. Try to avoid this.
//  These are for crucial lowlevel issues that preclude any meaningful
//  recovery (GUI didn't initialize, etc)
// Doesn't return, but if it did, you can assume it returns zero, so you can
//  do:  'return panic("out of memory");' or whatnot.
int panic(const char *err);

// Call this for a fatal problem that attempts an orderly shutdown (system
//  is fully working, but config file is hosed, etc).
// This makes an attempt to clean up any files already created by the
//  current installer (successfully run Setup.Install blocks in the config
//  file are not cleaned up).
// If there's no GUI or Lua isn't initialized, this calls panic(). That's bad.
// Doesn't return, but if it did, you can assume it returns zero, so you can
//  do:  'return fatal("missing config file");' or whatnot.
int fatal(const char *fmt, ...) ISPRINTF(1,2);

// Call this to pop up a warning dialog box and block until user hits OK.
void warn(const char *fmt, ...) ISPRINTF(1,2);

// Malloc replacements that blow up on allocation failure.
void *xmalloc(size_t bytes);
void *xrealloc(void *ptr, size_t bytes);
char *xstrdup(const char *str);

// strncpy() that promises to null-terminate the string, even on overflow.
char *xstrncpy(char *dst, const char *src, size_t len);

#define malloc(x) DO_NOT_CALL_MALLOC__USE_XMALLOC_INSTEAD
#define calloc(x,y) DO_NOT_CALL_CALLOC__USE_XMALLOC_INSTEAD
#define realloc(x,y) DO_NOT_CALL_REALLOC__USE_XREALLOC_INSTEAD
#define strdup(x) DO_NOT_CALL_STRDUP__USE_XSTRDUP_INSTEAD
#define strncpy(x,y,z) DO_NOT_CALL_STRNCPY__USE_XSTRNCPY_INSTEAD
//#define strcasecmp(x,y) DO_NOT_CALL_STRCASECMP__USE_UTF8STRICMP_INSTEAD
//#define stricmp(x,y) DO_NOT_CALL_STRICMP__USE_UTF8STRICMP_INSTEAD
//#define strcmpi(x,y) DO_NOT_CALL_STRCMPI__USE_UTF8STRICMP_INSTEAD

// Localization support.
const char *translate(const char *str);
#ifdef _
#undef _
#endif
#define _(x) translate(x)

// Call this with what you are profiling and the start time to log it:
//   uint32 start = MojoPlatform_ticks();
//     ...do something...
//   profile("Something I did", start);
uint32 profile(const char *what, uint32 start_time);


// See if a given flag was on the command line
//
// cmdline("nosound") will return true if "-nosound", "--nosound",
//  etc was on the command line. The option must start with a '-' on the
//  command line to be noticed by this function.
//
//  \param arg the command line flag to find
// \return true if arg was on the command line, false otherwise.
boolean cmdline(const char *arg);


// Get robust command line options, with optional default for missing ones.
//
//  If the command line was ./myapp --a=b -c=d ----e f
//    - cmdlinestr("a") will return "b"
//    - cmdlinestr("c") will return "d"
//    - cmdlinestr("e") will return "f"
//    - cmdlinestr("g") will return the default string.
//
// Like cmdline(), the option must start with a '-'.
//
//  \param arg The command line option to find.
//  \param envr optional environment var to check if command line wasn't found.
//  \param deflt The return value if (arg) isn't on the command line.
// \return The command line option, or (deflt) if the command line wasn't
//         found. This return value is READ ONLY. Do not free it, either.
const char *cmdlinestr(const char *arg, const char *envr, const char *deflt);


// Logging functions.
typedef enum
{
    MOJOSETUP_LOG_NOTHING,
    MOJOSETUP_LOG_ERRORS,
    MOJOSETUP_LOG_WARNINGS,
    MOJOSETUP_LOG_INFO,
    MOJOSETUP_LOG_DEBUG,
    MOJOSETUP_LOG_EVERYTHING
} MojoSetupLogLevel;

void MojoLog_initLogging(void);
void MojoLog_deinitLogging(void);
void logWarning(const char *fmt, ...) ISPRINTF(1,2);
void logError(const char *fmt, ...) ISPRINTF(1,2);
void logInfo(const char *fmt, ...) ISPRINTF(1,2);
void logDebug(const char *fmt, ...) ISPRINTF(1,2);

boolean initEverything(void);
void deinitEverything(void);

// A pointer to this struct is passed to plugins, so they can access
//  functionality in the base application. (Add to this as you need to.)
typedef struct MojoSetupEntryPoints
{
    void *(*xmalloc)(size_t bytes);
    void *(*xrealloc)(void *ptr, size_t bytes);
    char *(*xstrdup)(const char *str);
    char *(*xstrncpy)(char *dst, const char *src, size_t len);
    const char *(*translate)(const char *str);
    void (*logWarning)(const char *fmt, ...) ISPRINTF(1,2);
    void (*logError)(const char *fmt, ...) ISPRINTF(1,2);
    void (*logInfo)(const char *fmt, ...) ISPRINTF(1,2);
    void (*logDebug)(const char *fmt, ...) ISPRINTF(1,2);
} MojoSetupEntryPoints;
extern MojoSetupEntryPoints GEntryPoints;


#ifndef DOXYGEN_SHOULD_IGNORE_THIS
#if ((defined _MSC_VER) || (PLATFORM_BEOS))
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

#define STATICARRAYLEN(x) ( (sizeof ((x))) / (sizeof ((x)[0])) )

#define DEFINE_TO_STR2(x) #x
#define DEFINE_TO_STR(x) DEFINE_TO_STR2(x)

#ifdef __cplusplus
}
#endif

#endif

// end of universal.h ...


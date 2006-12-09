#ifndef _INCL_PLATFORM_H_
#define _INCL_PLATFORM_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

// this is called by your mainline.
int MojoSetup_main(int argc, char **argv);

// Caller must free returned string!
const char *MojoPlatform_appBinaryPath(void);

uint32 MojoPlatform_ticks(void);

// Make current process kill itself immediately, without any sort of internal
//  cleanup, like atexit() handlers or static destructors...the OS will have
//  to sort out the freeing of any resources, and no more code in this
//  process than necessary should run. This function does not return. Try to
//  avoid calling this.
void MojoPlatform_die(void);

// Get the current locale, in the format "xx_YY" where "xx" is the language
//  (en, fr, de...) and "_YY" is the country. (_US, _CA, etc). The country
//  can be omitted. Don't include encoding, it's always UTF-8 at this time.
// Return true if locale is known, false otherwise.
boolean MojoPlatform_locale(char *buf, size_t len);

boolean MojoPlatform_osType(char *buf, size_t len);
boolean MojoPlatform_osVersion(char *buf, size_t len);

// Basic platform detection.
#if PLATFORM_WINDOWS
#define PLATFORM_NAME "windows"
#elif PLATFORM_MACOSX
#define PLATFORM_NAME "macosx"
#elif PLATFORM_UNIX
#define PLATFORM_NAME "unix"
#else
#error Unknown processor architecture.
#endif

// Basic architecture detection.

#if defined(__powerpc64__)
#define PLATFORM_ARCH "powerpc64"
#elif defined(__ppc__) || defined(__powerpc__) || defined(__POWERPC__)
#define PLATFORM_ARCH "powerpc"
#elif defined(__x86_64__) || defined(_M_X64)
#define PLATFORM_ARCH "x86-64"
#elif defined(__X86__) || defined(__i386__) || defined(i386) || defined (_M_IX86) || defined(__386__)
#define PLATFORM_ARCH "x86"
#else
#error Unknown processor architecture.
#endif

#ifdef __cplusplus
}
#endif

#endif

// end of platform.h ...


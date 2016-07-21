/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

/*
 * This is in a separate file so that we can recompile it every time
 *  without it forcing a recompile on something ccache would otherwise not
 *  have to rebuild...this file's checksum changes every time you build it
 *  due to the __DATE__ and __TIME__ macros.
 *
 * The makefile will rebuild this file every time it relinks an executable
 *  so that we'll always have a unique build string.
 *
 * APPNAME and APPREV need to be predefined in the build system.
 *  The rest are supposed to be supplied by the compiler.
 */

#ifndef APPID
#error Please define APPID in the build system.
#endif

#ifndef APPREV
#error Please define APPREV in the build system.
#endif

#if (defined __clang__)
#   define VERSTR2(x) #x
#   define VERSTR(x) VERSTR2(x)
#   define COMPILERVER " " VERSTR(__clang_major__) "." VERSTR(__clang_minor__) "." VERSTR(__clang_patchlevel__)
#elif (defined __GNUC__)
#   define VERSTR2(x) #x
#   define VERSTR(x) VERSTR2(x)
#   define COMPILERVER " " VERSTR(__GNUC__) "." VERSTR(__GNUC_MINOR__) "." VERSTR(__GNUC_PATCHLEVEL__)
#elif (defined __SUNPRO_C)
#   define VERSTR2(x) #x
#   define VERSTR(x) VERSTR2(x)
#   define COMPILERVER " " VERSTR(__SUNPRO_C)
#elif (defined __VERSION__)
#   define COMPILERVER " " __VERSION__
#else
#   define COMPILERVER ""
#endif

#ifndef __DATE__
#define __DATE__ "(Unknown build date)"
#endif

#ifndef __TIME__
#define __TIME__ "(Unknown build time)"
#endif

#ifndef COMPILER
  #if (defined __clang__) && defined(__apple_build_version__)  // Apple reports version differently than LLVM Clang, note difference here.
    #define COMPILER "Apple Clang"
  #elif (defined __clang__)
    #define COMPILER "Clang"
  #elif (defined __GNUC__)
    #define COMPILER "GCC"
  #elif (defined _MSC_VER)
    #define COMPILER "Visual Studio"
  #elif (defined __SUNPRO_C)
    #define COMPILER "Sun Studio"
  #else
    #error Please define your platform.
  #endif
#endif

// macro mess so we can turn APPID and APPREV into a string literal...
#define MAKEBUILDVERSTRINGLITERAL2(id, rev) \
    #id ", revision " #rev ", built " __DATE__ " " __TIME__ \
    ", by " COMPILER COMPILERVER

#define MAKEBUILDVERSTRINGLITERAL(id, rev) MAKEBUILDVERSTRINGLITERAL2(id, rev)

const char *GBuildVer = MAKEBUILDVERSTRINGLITERAL(APPID, APPREV);

// end of buildver.c ...


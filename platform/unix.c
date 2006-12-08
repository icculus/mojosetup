#if PLATFORM_MACOSX
#include <Carbon/Carbon.h>
#endif

#include "../platform.h"

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

static struct timeval startup_time;


int main(int argc, char **argv)
{
    gettimeofday(&startup_time, NULL);
    return MojoSetup_main(argc, argv);
} // main


// (Stolen from physicsfs: http://icculus.org/physfs/ ...)
/*
 * See where program (bin) resides in the $PATH. Returns a copy of the first
 *  element in $PATH that contains it, or NULL if it doesn't exist or there
 *  were other problems.
 *
 * You are expected to free() the return value when you're done with it.
 */
static char *findBinaryInPath(const char *bin)
{
    const char *_envr = getenv("PATH");
    size_t alloc_size = 0;
    char *envr = NULL;
    char *exe = NULL;
    char *start = NULL;
    char *ptr = NULL;

    if ((_envr == NULL) || (bin == NULL))
        return NULL;

    envr = (char *) alloca(strlen(_envr) + 1);
    strcpy(envr, _envr);
    start = envr;

    do
    {
        size_t size;
        ptr = strchr(start, ':');  // find next $PATH separator.
        if (ptr)
            *ptr = '\0';

        size = strlen(start) + strlen(bin) + 2;
        if (size > alloc_size)
        {
            char *x = (char *) xrealloc(exe, size);
            alloc_size = size;
            exe = x;
        } // if

        // build full binary path...
        strcpy(exe, start);
        if ((exe[0] == '\0') || (exe[strlen(exe) - 1] != '/'))
            strcat(exe, "/");
        strcat(exe, bin);

        if (access(exe, X_OK) == 0)  // Exists as executable? We're done.
        {
            strcpy(exe, start);  // i'm lazy. piss off.
            return(exe);
        } // if

        start = ptr + 1;  // start points to beginning of next element.
    } while (ptr != NULL);

    if (exe != NULL)
        free(exe);

    return(NULL);  // doesn't exist in path.
} // findBinaryInPath


const char *MojoPlatform_appBinaryPath(void)
{
    const char *argv0 = GArgv[0];
    char resolved[PATH_MAX];

    static char *retval = NULL;
    if (retval != NULL)
        return retval;

    if (realpath("/proc/self/exe", resolved) != NULL)
        retval = xstrdup(resolved);  // fast path for Linux.
    else if ( (strchr(argv0, '/')) && (realpath(argv0, resolved)) )
        retval = xstrdup(resolved);  // argv[0] contains a path

    else  // slow path...have to search the whole $PATH for this one...
    {
        char *found = findBinaryInPath(argv0);
        if ( (found) && (realpath(found, resolved)) )
            retval = xstrdup(resolved);  // argv[0] contains a path
        free(found);
    } // else

    return retval;
} // MojoPlatform_appBinaryPath


// This implementation is a bit naive.
boolean MojoPlatform_locale(char *buf, size_t len)
{
    boolean retval = false;
    const char *envr = getenv("LANG");
    if (envr != NULL)
    {
        char *ptr = NULL;
        xstrncpy(buf, envr, len);
        ptr = strchr(buf, '.');  // chop off encoding if explicitly listed.
        if (ptr != NULL)
            *ptr = '\0';
        retval = true;
    } // if

    return retval;
} // MojoPlatform_locale


boolean MojoPlatform_osType(char *buf, size_t len)
{
#if PLATFORM_MACOSX
    xstrncpy(buf, "macosx", len);  // !!! FIXME: better string?
#elif defined(linux) || defined(__linux) || defined(__linux__)
    xstrncpy(buf, "linux", len);
#elif defined(__FreeBSD__) || defined(__DragonFly__)
    xstrncpy(buf, "freebsd", len);
#elif defined(__NetBSD__)
    xstrncpy(buf, "netbsd", len);
#elif defined(__OpenBSD__)
    xstrncpy(buf, "openbsd", len);
#elif defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
    xstrncpy(buf, "bsdi", len);
#elif defined(_AIX)
    xstrncpy(buf, "aix", len);
#elif defined(hpux) || defined(__hpux) || defined(__hpux__)
    xstrncpy(buf, "hpux", len);
#elif defined(sgi) || defined(__sgi) || defined(__sgi__) || defined(_SGI_SOURCE)
    xstrncpy(buf, "irix", len);
#else
#   error Please define your platform.
#endif

    return true;
} // MojoPlatform_ostype


boolean MojoPlatform_osVersion(char *buf, size_t len)
{
#if PLATFORM_MACOSX
    long ver = 0x0000;
	if (Gestalt(gestaltSystemVersion, &ver) == noErr)
    {
        char str[16];
        snprintf(str, sizeof (str), "%X", (int) ver);
        snprintf(buf, len, "%c%c.%c.%c", str[0], str[1], str[2], str[3]);
        return true;
    } // if
#endif

    // At this time, there isn't any way to determine the correct version of
    //  the OS on Unix that works everywhere or necessarily means anything.
    return false;
} // MojoPlatform_osversion


uint32 MojoPlatform_ticks(void)
{
    uint64 then_ms, now_ms;
    struct timeval now;
    gettimeofday(&now, NULL);
    then_ms = (((uint64) startup_time.tv_sec) * 1000) +
              (((uint64) startup_time.tv_usec) / 1000);
    now_ms = (((uint64) now.tv_sec) * 1000) + (((uint64) now.tv_usec) / 1000);
    return ((uint32) (now_ms - then_ms));
} // MojoPlatform_ticks

// end of unix.c ...


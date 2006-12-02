#include "../platform.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

int main(int argc, char **argv)
{
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
    const char *envr = getenv("PATH");
    size_t alloc_size = 0;
    char *exe = NULL;
    char *start = envr;
    char *ptr = NULL;

    if ((envr == NULL) || (bin == NULL))
        return NULL;

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
        const char *found = findBinaryInPath(argv0);
        if ( (found) && (realpath(found, resolved)) )
            retval = xstrdup(resolved);  // argv[0] contains a path
        free(found);
    } // else

    return retval;
} // MojoPlatform_appBinaryPath

// end of unix.c ...


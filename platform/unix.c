#include "../platform.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

int main(int argc, char **argv)
{
    return MojoSetup_main(argc, argv);
} // main


const char *MojoPlatform_appBinaryPath(void)
{
    char resolved[PATH_MAX];

    static char *retval = NULL;
    if (retval != NULL)
        return retval;

    if (realpath("/proc/self/exe", resolved) != NULL)
        retval = xstrdup(resolved);  // fast path for Linux.
    else if ( (strchr(GArgv[0], '/')) && (realpath(GArgv[0], resolved)) )
        retval = xstrdup(resolved);  // argv[0] contains a path
    else
        STUBBED("search path");

    return retval;
} // MojoPlatform_appBinaryPath

// end of unix.c ...


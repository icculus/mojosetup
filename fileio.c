#include "fileio.h"

// !!! FIXME: don't have this here. (need unlink for now).
#include <unistd.h>

boolean mojoInputToPhysicalFile(MojoInput *in, const char *fname)
{
    FILE *out = NULL;
    boolean iofailure = false;
    char buf[1024];
    size_t br;

    STUBBED("mkdir first?");

    if (in == NULL)
        return false;

    STUBBED("fopen?");
    unlink(fname);
    out = fopen(fname, "wb");
    if (out == NULL)
        return false;

    while (!iofailure)
    {
        br = in->read(in, buf, sizeof (buf));
        STUBBED("how to detect read failures?");
        if (br == 0)  // we're done!
            break;
        else if (br < 0)
            iofailure = true;
        else
        {
            if (fwrite(buf, br, 1, out) != 1)
                iofailure = true;
        } // else
    } // while

    fclose(out);
    if (iofailure)
    {
        unlink(fname);
        return false;
    } // if

    return true;
} // mojoInputToPhysicalFile

// end of fileio.c ...


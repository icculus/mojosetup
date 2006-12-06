#include <unistd.h>  // !!! FIXME: for _exit().

#include "universal.h"
#include "gui.h"

uint8 scratchbuf_128k[128 * 1024];
int GArgc = 0;
char **GArgv = NULL;

uint32 profile(const char *what, uint32 start_time)
{
    uint32 retval = MojoPlatform_ticks() - *counter;
    STUBBED("logging");
    printf("PROFILE: %s took %lu ms.\n", (unsigned long) retval);
    return retval;
} // profile_start


int fatal(const char *err)
{
    return panic(err);   // !!! FIXME
} // fatal


int panic(const char *err)
{
    static int panic_runs = 0;

    panic_runs++;
    if (panic_runs == 1)
    {
        if ((GGui != NULL) && (GGui->msgbox != NULL))
            GGui->msgbox(GGui, "PANIC", err);
        else
            panic(err);  /* no GUI plugin...double-panic. */
    } // if

    else if (panic_runs == 2)  // no GUI or panic panicked...write to stderr...
        fprintf(stderr, "\n\n\nMOJOSETUP PANIC: %s\n\n\n", err);

    else  // panic is panicking in a loop, terminate without any cleanup...
        _exit(22);

    exit(22);
    return 0;  // shouldn't hit this.
} // panic


char *xstrncpy(char *dst, const char *src, size_t len)
{
    snprintf(dst, len, "%s", src);
    return dst;
} // xstrncpy


#undef malloc
#undef calloc
void *xmalloc(size_t bytes)
{
    void *retval = calloc(1, bytes);
    if (retval == NULL)
        panic("out of memory");
    return retval;
} // xmalloc

#undef realloc
void *xrealloc(void *ptr, size_t bytes)
{
    void *retval = realloc(ptr, bytes);
    if (retval == NULL)
        panic("out of memory");
    return retval;
} // xrealloc

#undef strdup
char *xstrdup(const char *str)
{
    char *retval = (char *) xmalloc(strlen(str) + 1);
    strcpy(retval, str);
    return retval;
} // xstrdup

// end of misc.c ...


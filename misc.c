#include "universal.h"
#include "platform.h"
#include "gui.h"

uint8 scratchbuf_128k[128 * 1024];
MojoSetupEntryPoints GEntryPoints =
{
    xmalloc,
    xrealloc,
    xstrdup,
    xstrncpy,
    translate,
};

int GArgc = 0;
const char **GArgv = NULL;

uint32 profile(const char *what, uint32 start_time)
{
    uint32 retval = MojoPlatform_ticks() - start_time;
    if (what != NULL)
    {
        STUBBED("logging");  // !!! FIXME: and localize!
        printf("PROFILE: %s took %lu ms.\n", what, (unsigned long) retval);
    } // if
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
            GGui->msgbox(_("PANIC"), err);
        else
            panic(err);  /* no GUI plugin...double-panic. */
    } // if

    else if (panic_runs == 2)  // no GUI or panic panicked...write to stderr...
        fprintf(stderr, "\n\n\n%s\n  %s\n\n\n", _("PANIC"), err);

    else  // panic is panicking in a loop, terminate without any cleanup...
        MojoPlatform_die();

    exit(22);
    return 0;  // shouldn't hit this.
} // panic


char *xstrncpy(char *dst, const char *src, size_t len)
{
    snprintf(dst, len, "%s", src);
    return dst;
} // xstrncpy


static void out_of_memory(void)
{
    // Try to translate "out of memory", but not if it causes recursion.
    static boolean already_panicked = false;
    const char *errstr = "out of memory";
    if (!already_panicked)
    {
        already_panicked = true;
        errstr = translate(errstr);
    } // if
    panic(errstr);
} // out_of_memory


#undef malloc
#undef calloc
void *xmalloc(size_t bytes)
{
    void *retval = calloc(1, bytes);
    if (retval == NULL)
        out_of_memory();
    return retval;
} // xmalloc

#undef realloc
void *xrealloc(void *ptr, size_t bytes)
{
    void *retval = realloc(ptr, bytes);
    if (retval == NULL)
        out_of_memory();
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


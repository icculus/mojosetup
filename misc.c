#include "universal.h"

uint8 scratchbuf_128k[128 * 1024];
int GArgc = 0;
char **GArgv = NULL;

static void panic(const char *err)
{
    fprintf(stderr, "\n\n\nPANIC: %s\n\n\n", err);
    exit(22);
} // panic

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


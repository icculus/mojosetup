#include "universal.h"

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

// end of misc.c ...


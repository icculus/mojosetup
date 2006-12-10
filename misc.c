#include <stdarg.h>

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
    logWarning,
    logError,
    logInfo,
    logDebug,
};

int GArgc = 0;
const char **GArgv = NULL;


boolean cmdline(const char *arg)
{
    int argc = GArgc;
    const char **argv = GArgv;
    int i;

    if ((arg == NULL) || (argv == NULL))
        return false;

    while (*arg == '-')  // Skip all '-' chars, so "--nosound" == "-nosound"
        arg++;

    for (i = 1; i < argc; i++)
    {
        const char *thisarg = argv[i];
        if (*thisarg != '-')
            continue;  // no dash in the string, skip it.
        while (*(++thisarg) == '-') { /* keep looping. */ }
        if (strcmp(arg, thisarg) == 0)
            return true;
    } // for

    return false;
} // cmdline


const char *cmdlinestr(const char *arg, const char *envr, const char *deflt)
{
    uint32 len = 0;
    int argc = GArgc;
    const char **argv = GArgv;
    int i;

    if (arg == NULL)
        return deflt;

    while (*arg == '-')  // Skip all '-' chars, so "--nosound" == "-nosound"
        arg++;

    len = strlen(arg);

    for (i = 1; i < argc; i++)
    {
        const char *thisarg = argv[i];
        if (*thisarg != '-')
            continue;  // no dash in the string, skip it.

        while (*(++thisarg) == '-') { /* keep looping. */ }
        if (strncmp(arg, thisarg, len) != 0)
            continue;   // not us.

        thisarg += len;  // skip ahead in string to end of match.

        if (*thisarg == '=')  // --a=b format.
            return (thisarg + 1);
        else if (*thisarg == '\0')  // --a b format.
            return ((argv[i+1] == NULL) ? deflt : argv[i+1]);
    } // for

    if (envr != NULL)
    {
        const char *val = getenv(envr);
        if (val != NULL)
            return val;
    } // if

    return deflt;
} // cmdlinestr



#define DEFLOGLEV "everything"
static MojoSetupLogLevel logLevel = MOJOSETUP_LOG_EVERYTHING;
static FILE *logFile = NULL;

void MojoLog_initLogging(void)
{
    const char *level = cmdlinestr("loglevel","MOJOSETUP_LOGLEVEL", DEFLOGLEV);
    const char *fname = cmdlinestr("logfile", "MOJOSETUP_LOGFILE", NULL);

    if (strcmp(level, "nothing") == 0)
        logLevel = MOJOSETUP_LOG_NOTHING;
    else if (strcmp(level, "errors") == 0)
        logLevel = MOJOSETUP_LOG_ERRORS;
    else if (strcmp(level, "warnings") == 0)
        logLevel = MOJOSETUP_LOG_WARNINGS;
    else if (strcmp(level, "info") == 0)
        logLevel = MOJOSETUP_LOG_INFO;
    else if (strcmp(level, "debug") == 0)
        logLevel = MOJOSETUP_LOG_DEBUG;
    else  // Unknown string gets everything...that'll teach you.
        logLevel = MOJOSETUP_LOG_EVERYTHING;

    if (fname != NULL)
        logFile = fopen(fname, "w");
} // MojoLog_initLogging


void MojoLog_deinitLogging(void)
{
    if (logFile != NULL)
        fclose(logFile);
    logFile = NULL;
} // MojoLog_deinitLogging


static inline void addLog(MojoSetupLogLevel level, char levelchar,
                          const char *fmt, va_list ap)
{
    if (level <= logLevel)
    {
        char buf[256];
        //int len = vsnprintf(buf + 2, sizeof (buf) - 2, fmt, ap) + 2;
        //buf[0] = levelchar;
        //buf[1] = ' ';
        int len = vsnprintf(buf, sizeof (buf), fmt, ap);
        while ( (--len >= 0) && ((buf[len] == '\n') || (buf[len] == '\r')) ) {}
        buf[len+1] = '\0';  // delete trailing newline crap.
        MojoPlatform_log(buf);
        if (logFile != NULL)
        {
            fputs(buf, logFile);
            fputs("\n", logFile);
        } // if
    } // if
} // addLog


void logWarning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    addLog(MOJOSETUP_LOG_WARNINGS, '-', fmt, ap);
    va_end(ap);
} // logWarning


void logError(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    addLog(MOJOSETUP_LOG_ERRORS, '!', fmt, ap);
    va_end(ap);
} // logError


void logInfo(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    addLog(MOJOSETUP_LOG_INFO, '*', fmt, ap);
    va_end(ap);
} // logInfo


void logDebug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    addLog(MOJOSETUP_LOG_DEBUG, '#', fmt, ap);
    va_end(ap);
} // logDebug


uint32 profile(const char *what, uint32 start_time)
{
    uint32 retval = MojoPlatform_ticks() - start_time;
    if (what != NULL)
    {
        logDebug("%s took %lu ms.", what, (unsigned long) retval);
    } // if
    return retval;
} // profile_start


int fatal(const char *err)
{
    logError("FATAL: %s", err);
    STUBBED("fatal is not a panic scenario...do an orderly cleanup and exit.");
    return panic(err);
} // fatal


int panic(const char *err)
{
    static int panic_runs = 0;

    panic_runs++;
    if (panic_runs == 1)
    {
        logError("PANIC: %s", err);
        panic(err);
    } // if

    else if (panic_runs == 2)
    {
        if ((GGui != NULL) && (GGui->msgbox != NULL))
            GGui->msgbox(_("PANIC"), err);
        else
            panic(err);  /* no GUI plugin...double-panic. */
    } // if

    else if (panic_runs == 3)  // no GUI or panic panicked...write to stderr...
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


static void outOfMemory(void)
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
} // outOfMemory


#undef malloc
#undef calloc
void *xmalloc(size_t bytes)
{
    void *retval = calloc(1, bytes);
    if (retval == NULL)
        outOfMemory();
    return retval;
} // xmalloc

#undef realloc
void *xrealloc(void *ptr, size_t bytes)
{
    void *retval = realloc(ptr, bytes);
    if (retval == NULL)
        outOfMemory();
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


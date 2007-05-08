#include <stdarg.h>

#include "universal.h"
#include "platform.h"
#include "gui.h"
#include "lua_glue.h"
#include "fileio.h"

#define TEST_ARCHIVE_CODE 0
int MojoSetup_testArchiveCode(int argc, char **argv);

#define TEST_NETWORK_CODE 1
int MojoSetup_testNetworkCode(int argc, char **argv);


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
    MojoPlatform_ticks,
};

int GArgc = 0;
const char **GArgv = NULL;

static char *crashedmsg = NULL;
static char *termedmsg = NULL;

void MojoSetup_crashed(void)
{
    if (crashedmsg == NULL)
        panic("BUG: crash at startup");
    else
        fatal(crashedmsg);
} // MojoSetup_crash


void MojoSetup_terminated(void)
{
    if (termedmsg == NULL)  // no translation yet.
        panic("The installer has been stopped by the system.");
    else
        fatal(termedmsg);
} // MojoSetup_crash


static boolean initEverything(void)
{
    MojoLog_initLogging();

    logInfo("MojoSetup starting up...");

    // We have to panic on errors until the GUI is ready. Try to make things
    //  "succeed" unless they are catastrophic, and report problems later.

    // Start with the base archive work, since it might have GUI plugins.
    //  None of these panic() calls are localized, since localization isn't
    //  functional until MojoLua_initLua() succeeds.
    if (!MojoArchive_initBaseArchive())
        panic("Initial setup failed. Cannot continue.");

    else if (!MojoGui_initGuiPlugin())
        panic("Initial GUI setup failed. Cannot continue.");

    else if (!MojoLua_initLua())
        panic("Initial Lua setup failed. Cannot continue.");

    crashedmsg = xstrdup(_("The installer has crashed due to a bug."));
    termedmsg = xstrdup(_("The installer has been stopped by the system."));

    return true;
} // initEverything


static void deinitEverything(void)
{
    char *tmp = NULL;

    logInfo("MojoSetup shutting down...");
    MojoLua_deinitLua();
    MojoGui_deinitGuiPlugin();
    MojoArchive_deinitBaseArchive();
    MojoLog_deinitLogging();

    tmp = crashedmsg;
    crashedmsg = NULL;
    free(tmp);
    tmp = termedmsg;
    termedmsg = NULL;
    free(tmp);
} // deinitEverything


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


boolean wildcardMatch(const char *str, const char *pattern)
{
    char sch = *(str++);
    while (true)
    {
        const char pch = *(pattern++);
        if (pch == '?')
        {
            if ((sch == '\0') || (sch == '/'))
                return false;
            sch = *(str++);
        } // else if

        else if (pch == '*')
        {
            char nextpch = *pattern;
            if ((nextpch != '?') && (nextpch != '*'))
            {
                while ((sch != '\0') && (sch != nextpch))
                    sch = *(str++);
            } // if
        } // else if

        else
        {
            if (pch != sch)
                return false;
            else if (pch == '\0')
                return true;
            sch = *(str++);
        } // else
    } // while

    return true;   // shouldn't hit this.
} // wildcardMatch


// !!! FIXME: change this later.
#define DEFLOGLEV "everything"
MojoSetupLogLevel MojoLog_logLevel = MOJOSETUP_LOG_EVERYTHING;
static FILE *logFile = NULL;

void MojoLog_initLogging(void)
{
    const char *level = cmdlinestr("loglevel","MOJOSETUP_LOGLEVEL", DEFLOGLEV);
    const char *fname = cmdlinestr("logfile", "MOJOSETUP_LOGFILE", NULL);

    if (strcmp(level, "nothing") == 0)
        MojoLog_logLevel = MOJOSETUP_LOG_NOTHING;
    else if (strcmp(level, "errors") == 0)
        MojoLog_logLevel = MOJOSETUP_LOG_ERRORS;
    else if (strcmp(level, "warnings") == 0)
        MojoLog_logLevel = MOJOSETUP_LOG_WARNINGS;
    else if (strcmp(level, "info") == 0)
        MojoLog_logLevel = MOJOSETUP_LOG_INFO;
    else if (strcmp(level, "debug") == 0)
        MojoLog_logLevel = MOJOSETUP_LOG_DEBUG;
    else  // Unknown string gets everything...that'll teach you.
        MojoLog_logLevel = MOJOSETUP_LOG_EVERYTHING;

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
    if (level <= MojoLog_logLevel)
    {
        char buf[1024];
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
            fflush(logFile);
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


int fatal(const char *fmt, ...)
{
    static boolean in_fatal = false;
    size_t len = 128;
    char *buf = NULL;
    int rc = 0;
    va_list ap;

    if (in_fatal)
        return panic("BUG: fatal() called more than once!");

    in_fatal = true;

    buf = xmalloc(len);
    va_start(ap, fmt);
    rc = vsnprintf(buf, len, fmt, ap);
    va_end(ap);
    if (rc >= len)
    {
        len = rc;
        buf = xrealloc(buf, len);
        va_start(ap, fmt);
        vsnprintf(buf, len, fmt, ap);
        va_end(ap);
    } // if

    logError("FATAL: %s", buf);
    if ( (GGui == NULL) || (!MojoLua_initialized()) )
    {
        logError("fatal() called before app is initialized! Panicking...");
        panic(buf);   // Shouldn't call fatal() before app is initialized!
    } // if

    GGui->msgbox(_("Fatal error"), buf);
    free(buf);

    //GGui->status(_("There were errors. Click 'OK' to clean up and exit."));
    MojoLua_callProcedure("revertinstall");

    deinitEverything();
    exit(23);
    return 0;
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


// This is called from main()/WinMain()/whatever.
int MojoSetup_main(int argc, char **argv)
{
    GArgc = argc;
    GArgv = (const char **) argv;

    if (cmdline("buildver"))
    {
        printf("%s\n", GBuildVer);
        return 0;
    } // if

    #if TEST_ARCHIVE_CODE
    return MojoSetup_testArchiveCode(argc, argv);
    #endif

    #if TEST_NETWORK_CODE
    return MojoSetup_testNetworkCode(argc, argv);
    #endif

    if (!initEverything())
        return 1;

    // Jump into Lua for the heavy lifting.
    MojoLua_runFile("mojosetup_mainline");

    deinitEverything();

    return 0;
} // MojoSetup_main





#if TEST_ARCHIVE_CODE
int MojoSetup_testArchiveCode(int argc, char **argv)
{
    int i;
    printf("Testing archiver code...\n\n");
    for (i = 1; i < argc; i++)
    {
        MojoArchive *archive = MojoArchive_newFromDirectory(argv[i]);
        if (archive != NULL)
            printf("directory '%s'...\n", argv[i]);
        else
        {
            MojoInput *io = MojoInput_newFromFile(argv[i]);
            if (io != NULL)
            {
                archive = MojoArchive_newFromInput(io, argv[i]);
                if (archive != NULL)
                    printf("archive '%s'...\n", argv[i]);
            } // if
        } // else

        if (archive == NULL)
            fprintf(stderr, "Couldn't handle '%s'\n", argv[i]);
        else
        {
            if (!archive->enumerate(archive))
                fprintf(stderr, "enumerate() failed.\n");
            else
            {
                const MojoArchiveEntry *ent;
                while ((ent = archive->enumNext(archive)) != NULL)
                {
                    printf("%s ", ent->filename);
                    if (ent->type == MOJOARCHIVE_ENTRY_FILE)
                    {
                        printf("(file, %d bytes, %o)\n",
                                (int) ent->filesize, (int) ent->perms);
                    } // if
                    else if (ent->type == MOJOARCHIVE_ENTRY_DIR)
                        printf("(dir, %o)\n", ent->perms);
                    else if (ent->type == MOJOARCHIVE_ENTRY_SYMLINK)
                        printf("(symlink -> '%s')\n", ent->linkdest);
                    else
                    {
                        printf("(UNKNOWN?!, %d bytes, -> '%s', %o)\n",
                                (int) ent->filesize, ent->linkdest,
                                (int) ent->perms);
                    } // else
                } // while
            } // else
            archive->close(archive);
            printf("\n\n");
        } // else
    } // for

    return 0;
} // MojoSetup_testArchiveCode
#endif


#if TEST_NETWORK_CODE
int MojoSetup_testNetworkCode(int argc, char **argv)
{
    int i;
    fprintf(stderr, "Testing networking code...\n\n");
    for (i = 1; i < argc; i++)
    {
        static char buf[64 * 1024];
        uint32 start = 0;
        const char *url = argv[i];
        int64 br = 0;
        printf("\n\nFetching '%s' ...\n", url);
        MojoInput *io = MojoInput_fromURL(url);
        if (io == NULL)
        {
            fprintf(stderr, "failed!\n");
            continue;
        } // if

        start = MojoPlatform_ticks();
        while (!io->ready(io))
            MojoPlatform_sleep(10);
        fprintf(stderr, "took about %d ticks to get started\n",
                (int) (MojoPlatform_ticks() - start));

        fprintf(stderr, "Ready to read (%lld) bytes.\n",
                (long long) io->length(io));

        do
        {
            start = MojoPlatform_ticks();
            if (!io->ready(io))
            {
                fprintf(stderr, "Not ready!\n");
                while (!io->ready(io))
                    MojoPlatform_sleep(10);
                fprintf(stderr, "took about %d ticks to get ready\n",
                        (int) (MojoPlatform_ticks() - start));
            } // if

            start = MojoPlatform_ticks();
            br = io->read(io, buf, sizeof (buf));
            fprintf(stderr, "read blocked for about %d ticks\n",
                    (int) (MojoPlatform_ticks() - start));
            if (br > 0)
            {
                fprintf(stderr, "read %lld bytes\n", (long long) br);
                fwrite(buf, br, 1, stdout);
            } // if
        } while (br > 0);

        if (br < 0)
            fprintf(stderr, "ERROR IN TRANSMISSION.\n\n");
        else
            fprintf(stderr, "TRANSMISSION COMPLETE!\n\n");

        io->close(io);
    } // for

    return 0;
} // MojoSetup_testNetworkCode
#endif

// end of mojosetup.c ...


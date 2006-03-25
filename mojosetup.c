
#include "universal.h"
#include "platform.h"
#include "fileio.h"
#include "gui.h"

/* !!! FIXME: None of these should be here. */
#include <dlfcn.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int GArgc = 0;
char **GArgv = NULL;
MojoGui *GGui = NULL;

static boolean mojoInputToPhysicalFile(MojoInput *in, const char *fname)
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
        if (br == 0)  /* we're done! */
            break;
        else if (br < 0)
            iofailure = true;
        else
        {
            if (fwrite(buf, br, 1, out) != 1)
                iofailure = true;
        } /* else */
    } /* while */

    fclose(out);
    if (iofailure)
    {
        unlink(fname);
        return false;
    } /* if */

    return true;
} /* mojoInputToPhysicalFile */


static void *loadGuiPlugin(MojoArchive *ar)
{
    char fname[256];
    void *retval = NULL;
    boolean rc;
    MojoInput *io = ar->openCurrentEntry(ar);
    if (io == NULL)
        return false;

    STUBBED("Don't copy if it's a physical file already?");

    STUBBED("Filename creation has to change");
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        snprintf(fname, sizeof (fname), "/tmp/mojosetup-gui-%ld.so", tv.tv_sec);
    }

    rc = mojoInputToPhysicalFile(io, fname);
    io->close(io);

    if (rc)
    {
        retval = dlopen(fname, RTLD_NOW | RTLD_GLOBAL);
        STUBBED("abstract out dlopen!");
        if (retval != NULL)
        {
            STUBBED("abstract out dlsym, too. :)");
            void *entry = dlsym(retval, MOJOGUI_ENTRY_POINT_STR);
            if (entry == NULL)  /* not a compatible plugin. */
            {
                STUBBED("aaaaaaand, dlclose")
                dlclose(retval);
                unlink(fname);
                retval = NULL;
            } /* if */
        } /* if */
    } /* if */

    return retval;
} /* loadGuiPlugin */


typedef struct S_DLLLIST { void *lib; struct S_DLLLIST *next; } DllList;
static boolean findGuiPlugin(void)
{
    const MojoArchiveEntryInfo *entinfo = NULL;
    DllList plugins = { NULL, NULL };
    DllList *i = NULL;

    if (GGui != NULL)
        return true;

    STUBBED("DllList should track filename, isstatic, and entrypoint, too");

    /*
     * !!! FIXME: Have a global MojoArchive that represents the install
     * !!! FIXME:  (either an archive, a physical dir, etc.)
     */
    STUBBED("use a global MojoArchive for basedir");
    MojoArchive *dir = MojoArchive_newFromDirectory(".");
    if (dir == NULL)
        return false;

    dir->restartEnumeration(dir);
    while ((entinfo = dir->enumEntry(dir)) != NULL)
    {
        void *lib;
        DllList *item;

        /* Not a file? */
        if (entinfo->type != MOJOARCHIVE_ENTRY_FILE)
            continue;

        /* not in the gui dir? */
        if (strncmp(entinfo->filename, "guiplugins/", 4) != 0)
            continue;

        lib = loadGuiPlugin(dir);
        if (lib == NULL)
            continue;

        item = malloc(sizeof (DllList));
        if (item == NULL)
            continue;

        item->lib = lib;
        item->next = plugins.next;
        plugins.next = item;
    } /* while */

    STUBBED("Add static plugins to the list");
    STUBBED("Choose plugin by priority");
    for (i = plugins.next; i != NULL; i = i->next)
    {
        MOJOGUI_STRUCT * (*entry)(void) = dlsym(i->lib, MOJOGUI_ENTRY_POINT_STR);
        MOJOGUI_STRUCT *gui = entry();
        if ( gui->priority(gui) != ((uint8) MOJOGUI_PRIORITY_NEVER_TRY) )
        {
            if (gui->init(gui))
            {
                GGui = gui;
                break;
            } /* if */
        } /* if */
    } /* for */

    STUBBED("close libs, delete tmp files, free list.");

    STUBBED("Don't close this when it's a global.");
    dir->close(dir);

    return (GGui != NULL);
} /* findGuiPlugin */

/*
 * This is called from main()/WinMain()/whatever.
 */
int MojoSetup_main(int argc, char **argv)
{
    GArgc = argc;
    GArgv = argv;

    if (!findGuiPlugin())
        return 1;

    GGui->msgbox(GGui, "Title", "text goes here.");
    GGui->deinit(GGui);
    STUBBED("cleanup gui lib, etc.\n");

    return 0;
} /* MojoSetup_main */

/* end of mojosetup.c ... */


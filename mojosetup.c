
#include "universal.h"
#include "platform.h"
#include "fileio.h"
#include "gui.h"

int GArgc = 0;
char **GArgv = NULL;


// This is called from main()/WinMain()/whatever.
int MojoSetup_main(int argc, char **argv)
{
    GArgc = argc;
    GArgv = argv;

    if (!MojoGui_initGuiPlugin())
        return 1;

    dbgprintf("Using GUI plugin '%s'\n", GGui->name(GGui));
    GGui->msgbox(GGui, "Title", "text goes here.");

    MojoGui_deinitGuiPlugin();
    return 0;
} // MojoSetup_main

// end of mojosetup.c ...


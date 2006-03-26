
#include "universal.h"
#include "platform.h"
#include "fileio.h"
#include "gui.h"

static boolean initEverything(void)
{
    STUBBED("Init logging functionality.");

    if (!MojoArchive_initBaseArchive())
        return false;

    if (!MojoGui_initGuiPlugin())
        return false;

    return true;
} // initEverything


static void deinitEverything(void)
{
    MojoGui_deinitGuiPlugin();
    STUBBED("Deinit logging functionality.");
} // deinitEverything


// This is called from main()/WinMain()/whatever.
int MojoSetup_main(int argc, char **argv)
{
    GArgc = argc;
    GArgv = argv;

    if (!initEverything())
        return 1;

    GGui->msgbox(GGui, "test", "testing");

    deinitEverything();
    return 0;
} // MojoSetup_main

// end of mojosetup.c ...


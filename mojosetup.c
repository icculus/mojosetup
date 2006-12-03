
#include "universal.h"
#include "platform.h"
#include "fileio.h"
#include "gui.h"
#include "lua_glue.h"

static boolean initEverything(void)
{
    STUBBED("Init logging functionality.");

    // We have to panic on errors until the GUI is ready. Try to make things
    //  "succeed" unless they are catastrophic, and report problems later.

    // Start with the base archive work, since it might have GUI plugins.
    if (!MojoArchive_initBaseArchive())
        panic("Initial setup failed. Cannot continue.");

    else if (!MojoGui_initGuiPlugin())
        panic("Initial GUI setup failed. Cannot continue.");

    else if (!MojoLua_initLua())
        panic("Initial Lua setup failed. Cannot continue.");

    // Set up localization table, if possible.
    MojoLua_runFile("translations");

    // loadConfigFile()

    return true;
} // initEverything


static void deinitEverything(void)
{
    MojoLua_deinitLua();
    MojoGui_deinitGuiPlugin();
    MojoArchive_deinitBaseArchive();
    STUBBED("Deinit logging functionality.");
} // deinitEverything


// This is called from main()/WinMain()/whatever.
int MojoSetup_main(int argc, char **argv)
{
    GArgc = argc;
    GArgv = argv;

    GLocale = xstrdup("es");   // !!! FIXME

    if (!initEverything())
        return 1;

    GGui->msgbox(GGui, "translating...", _("Required for play"));

    deinitEverything();

    free(GLocale);   // !!! FIXME

    return 0;
} // MojoSetup_main

// end of mojosetup.c ...


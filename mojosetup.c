
#include "universal.h"
#include "platform.h"
#include "fileio.h"
#include "gui.h"
#include "lua_glue.h"

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

    return true;
} // initEverything


static void deinitEverything(void)
{
    logInfo("MojoSetup shutting down...");
    MojoLua_deinitLua();
    MojoGui_deinitGuiPlugin();
    MojoArchive_deinitBaseArchive();
    MojoLog_deinitLogging();
} // deinitEverything


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

    if (!initEverything())
        return 1;

    // Jump into Lua for the heavy lifting.
    MojoLua_runFile("mojosetup_mainline");

    deinitEverything();

    return 0;
} // MojoSetup_main

// end of mojosetup.c ...


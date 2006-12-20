
#include "universal.h"
#include "lua_glue.h"

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


#define BUILDING_EXTERNAL_PLUGIN 1
#include "../gui.h"

// Probably want to support this always, unless explicitly overridden.
#ifndef SUPPORT_GUI_STDIO
#define SUPPORT_GUI_STDIO 1
#endif

// Probably want to statically link it, too.
#ifndef GUI_STDIO_STATIC_LINK
#define GUI_STDIO_STATIC_LINK 1
#endif

#if SUPPORT_GUI_STDIO

#include <ctype.h>

static uint8 MojoGui_stdio_priority(MojoGui_rev1 *gui)
{
    return MOJOGUI_PRIORITY_TRY_LAST;
}

static const char* MojoGui_stdio_name(MojoGui_rev1 *gui)
{
    return "stdio";
}

static boolean MojoGui_stdio_init(MojoGui_rev1 *gui)
{
    return true;
}

static void MojoGui_stdio_deinit(MojoGui_rev1 *gui)
{
    // no-op
}

static void MojoGui_stdio_msgbox(MojoGui_rev1 *gui, const char *title, const char *text)
{
    printf("NOTICE: %s\n[hit enter]", text);
    fflush(stdout);
    if (!feof(stdin))
        getchar();
}

static boolean MojoGui_stdio_promptyn(MojoGui_rev1 *gui, const char *title, const char *text)
{
    if (feof(stdin))
        return 0;

    while (1)
    {
        int c;
        printf("%s\n[y/n]", text);
        fflush(stdout);
        c = toupper(getchar());
        if (c == 'N')
            return 0;
        else if (c == 'Y')
            return 1;
    } // while

    return 0;
}

MOJOGUI_PLUGIN(stdio)

#if !GUI_STDIO_STATIC_LINK
CREATE_MOJOGUI_ENTRY_POINT(stdio)
#endif

#endif // SUPPORT_GUI_STDIO

// end of gui_stdio.c ...


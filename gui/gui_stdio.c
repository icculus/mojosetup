#define BUILDING_EXTERNAL_PLUGIN 1
#include "../gui.h"

#if SUPPORT_GUI_STDIO

#include <ctype.h>

static uint8 MojoGui_stdio_priority(MojoGui *gui)
{
    return MOJOGUI_PRIORITY_TRY_LAST;
}

static boolean MojoGui_stdio_init(MojoGui *gui)
{
    return true;
}

static void MojoGui_stdio_deinit(MojoGui *gui)
{
    // no-op
}

static void MojoGui_stdio_msgbox(MojoGui *gui, const char *title, const char *text)
{
    printf(_("NOTICE: %s\n[hit enter]"), text);
    fflush(stdout);
    if (!feof(stdin))
        getchar();
}

static boolean MojoGui_stdio_promptyn(MojoGui *gui, const char *title, const char *text)
{
    if (feof(stdin))
        return 0;

    while (1)
    {
        int c;
        printf(_("%s\n[y/n]"), text);
        fflush(stdout);
        c = toupper(getchar());
        if (c == 'N')  // !!! FIXME: localize?
            return 0;
        else if (c == 'Y')
            return 1;
    } // while

    return 0;
}

MOJOGUI_PLUGIN(stdio)

#if !GUI_STATIC_LINK_STDIO
CREATE_MOJOGUI_ENTRY_POINT(stdio)
#endif

#endif // SUPPORT_GUI_STDIO

// end of gui_stdio.c ...


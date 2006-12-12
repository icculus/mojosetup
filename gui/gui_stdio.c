#if !SUPPORT_GUI_STDIO
#error Something is wrong in the build system.
#endif

#define BUILDING_EXTERNAL_PLUGIN 1
#include "../gui.h"

MOJOGUI_PLUGIN(stdio)

#if !GUI_STATIC_LINK_STDIO
CREATE_MOJOGUI_ENTRY_POINT(stdio)
#endif

#include <ctype.h>

static uint8 MojoGui_stdio_priority(void)
{
    return MOJOGUI_PRIORITY_TRY_LAST;
}

static boolean MojoGui_stdio_init(void)
{
    return true;
}

static void MojoGui_stdio_deinit(void)
{
    // no-op
}

static void MojoGui_stdio_msgbox(const char *title, const char *text)
{
    printf(entry->_("NOTICE: %s\n[hit enter]"), text);
    fflush(stdout);
    if (!feof(stdin))
        getchar();
}

static boolean MojoGui_stdio_promptyn(const char *title, const char *text)
{
    if (feof(stdin))
        return 0;

    while (1)
    {
        int c;
        printf(entry->_("%s\n[y/n]"), text);
        fflush(stdout);
        c = toupper(getchar());
        if (c == 'N')  // !!! FIXME: localize?
            return 0;
        else if (c == 'Y')
            return 1;
    } // while

    return 0;
}

static boolean MojoGui_stdio_startgui(const char *title, const char *splash)
{
    printf("%s\n", title);
    return true;
} // MojoGui_stdio_startgui


static void MojoGui_stdio_endgui(void)
{
    // no-op.
} // MojoGui_stdio_endgui

// end of gui_stdio.c ...


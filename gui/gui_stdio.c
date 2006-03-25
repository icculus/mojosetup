#include "../gui.h"
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

CREATE_MOJOGUI_ENTRY_POINT(stdio)

// gui_stdio.c ...


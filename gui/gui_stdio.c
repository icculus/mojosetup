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
    getchar();
} // MojoGui_stdio_msgbox

static boolean MojoGui_stdio_promptyn(const char *title, const char *text)
{
    if (feof(stdin))
        return 0;
    else
    {
        const char *localized_no = entry->_("N");
        const char *localized_yes = entry->_("Y");
        char buf[128];
        size_t len = 0;
        while (1)
        {
            printf(entry->_("%s\n[y/n]: "), text);
            fflush(stdout);

            if (fgets(buf, sizeof (buf), stdin) == NULL)
                return 0;

            len = strlen(buf) - 1;
            while ( (len >= 0) && ((buf[len] == '\n') || (buf[len] == '\r')) )
                buf[len--] = '\0';

            if (strcasecmp(buf, localized_no) == 0)
                return 0;
            else if (strcasecmp(buf, localized_yes) == 0)
                return 1;
        } // while
    } // else

    return 0;
} // MojoGui_stdio_promptyn

static boolean MojoGui_stdio_start(const char *title, const char *splash)
{
    printf("%s\n", title);
    return true;
} // MojoGui_stdio_start


static void MojoGui_stdio_stop(void)
{
    // no-op.
} // MojoGui_stdio_stop


static boolean MojoGui_stdio_readme(const char *name, const uint8 *data,
                                    size_t len, boolean can_go_back,
                                    boolean can_go_forward)
{
    printf("%s\n%s\n", name, (const char *) data);
    return true;
} // MojoGui_stdio_readme

// end of gui_stdio.c ...


/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#if !SUPPORT_GUI_STDIO
#error Something is wrong in the build system.
#endif

#define BUILDING_EXTERNAL_PLUGIN 1
#include "gui.h"

MOJOGUI_PLUGIN(stdio)

#if !GUI_STATIC_LINK_STDIO
CREATE_MOJOGUI_ENTRY_POINT(stdio)
#endif

#include <ctype.h>

static char *lastProgressType = NULL;
static char *lastComponent = NULL;
static uint32 percentTicks = 0;

static int read_stdin(char *buf, int len)
{
    if (fgets(buf, len, stdin) == NULL)
        return -1;

    len = strlen(buf) - 1;
    while ( (len >= 0) && ((buf[len] == '\n') || (buf[len] == '\r')) )
        buf[len--] = '\0';

    return len+1;
} // read_stdin


static int readstr(const char *prompt, char *buf, int len,
                   boolean back, boolean fwd)
{
    int retval = 0;
    const char *backstr = NULL;

    if (prompt != NULL)
        printf("%s\n", prompt);

    if (back)
    {
        backstr = entry->xstrdup(entry->_("back"));
        printf(entry->_("Type '%s' to go back."), backstr);
        printf("\n");
    } // if

    if (fwd)
    {
        printf(entry->_("Press enter to continue."));
        printf("\n");
    } // if

    printf(entry->_("> "));
    fflush(stdout);

    if ((retval = read_stdin(buf, len)) >= 0)
    {
        if ((back) && (strcmp(buf, backstr) == 0))
            retval = -1;
    } // if

    free((void *) backstr);
    return retval;
} // print_prompt


static uint8 MojoGui_stdio_priority(boolean istty)
{
    // if not a tty and no other GUI plugins worked out, let the base
    //  application try to spawn a terminal and try again. If it can't do so,
    //  it will panic() and thus end the process, so we don't end up blocking
    //  on some prompt the user can't see.

    if (!istty)
        return MOJOGUI_PRIORITY_NEVER_TRY;

    return MOJOGUI_PRIORITY_TRY_ABSOLUTELY_LAST;  // always a last resort.
} // MojoGui_stdio_priority


static boolean MojoGui_stdio_init(void)
{
    percentTicks = 0;
    return true;  // always succeeds.
} // MojoGui_stdio_init


static void MojoGui_stdio_deinit(void)
{
    free(lastProgressType);
    free(lastComponent);
    lastProgressType = NULL;
    lastComponent = NULL;
} // MojoGui_stdio_deinit


static void MojoGui_stdio_msgbox(const char *title, const char *text)
{
    char buf[128];
    printf(entry->_("NOTICE: %s\n[hit enter]"), text);
    fflush(stdout);
    read_stdin(buf, sizeof (buf));
} // MojoGui_stdio_msgbox


static boolean MojoGui_stdio_promptyn(const char *title, const char *text,
                                      boolean defval)
{
    boolean retval = false;
    if (!feof(stdin))
    {
        const char *localized_no = entry->xstrdup(entry->_("N"));
        const char *localized_yes = entry->xstrdup(entry->_("Y"));
        boolean getout = false;
        char buf[128];
        while (!getout)
        {
            // !!! FIXME:
            // We currently ignore defval and make you type out your choice.
            printf(entry->_("%s\n[y/n]: "), text);
            fflush(stdout);
            if (read_stdin(buf, sizeof (buf)) < 0)
                getout = true;
            else if (strcasecmp(buf, localized_no) == 0)
                getout = true;
            else if (strcasecmp(buf, localized_yes) == 0)
                retval = getout = true;
        } // while
        free((void *) localized_no);
        free((void *) localized_yes);
    } // if

    return retval;
} // MojoGui_stdio_promptyn


static MojoGuiYNAN MojoGui_stdio_promptynan(const char *title, const char *txt,
                                            boolean defval)
{
    MojoGuiYNAN retval = MOJOGUI_NO;
    if (!feof(stdin))
    {
        const char *localized_no = entry->xstrdup(entry->_("N"));
        const char *localized_yes = entry->xstrdup(entry->_("Y"));
        const char *localized_always = entry->xstrdup(entry->_("Always"));
        const char *localized_never = entry->xstrdup(entry->_("Never"));
        boolean getout = false;
        char buf[128];
        while (!getout)
        {
            // !!! FIXME:
            // We currently ignore defval and make you type out your choice.
            printf(entry->_("%s\n[y/n/Always/Never]: "), txt);
            fflush(stdout);
            if (read_stdin(buf, sizeof (buf)) < 0)
                getout = true;
            else if (strcasecmp(buf, localized_no) == 0)
                getout = true;
            else if (strcasecmp(buf, localized_yes) == 0)
            {
                retval = MOJOGUI_YES;
                getout = true;
            } // else if
            else if (strcasecmp(buf, localized_always) == 0)
            {
                retval = MOJOGUI_ALWAYS;
                getout = true;
            } // else if
            else if (strcasecmp(buf, localized_never) == 0)
            {
                retval = MOJOGUI_NEVER;
                getout = true;
            } // else if
        } // while
        free((void *) localized_no);
        free((void *) localized_yes);
        free((void *) localized_always);
        free((void *) localized_never);
    } // if

    return retval;
} // MojoGui_stdio_promptynan


static boolean MojoGui_stdio_start(const char *title,
                                   const MojoGuiSplash *splash)
{
    printf("%s\n", title);
    return true;
} // MojoGui_stdio_start


static void MojoGui_stdio_stop(void)
{
    // no-op.
} // MojoGui_stdio_stop


static int MojoGui_stdio_readme(const char *name, const uint8 *data,
                                    size_t datalen, boolean can_back,
                                    boolean can_fwd)
{
    boolean getout = false;
    int retval = -1;
    int len = 0;
    char buf[128];

    while (!getout)
    {
        printf("%s\n%s\n", name, (const char *) data);
        if ((len = readstr(NULL, buf, sizeof (buf), can_back, true)) < 0)
            getout = true;
        else if (len == 0)
        {
            getout = true;
            retval = 1;
        } // else if
    } // while

    return retval;
} // MojoGui_stdio_readme


static void toggle_option(MojoGuiSetupOptions *parent,
                          MojoGuiSetupOptions *opts, int *line, int target)
{
    if ((opts != NULL) && (target > *line))
    {
        if (!opts->is_group_parent)
        {
            if (++(*line) == target)
            {
                const boolean toggled = ((opts->value) ? false : true);

                // "radio buttons" in a group?
                if ((parent) && (parent->is_group_parent))
                {
                    if (toggled)  // drop unless we weren't the current toggle.
                    {
                        // set all siblings to false...
                        MojoGuiSetupOptions *i = parent->child;
                        while (i != NULL)
                        {
                            i->value = false;
                            i = i->next_sibling;
                        } // while
                        opts->value = true;  // reset us to be true.
                    } // if
                } // if

                else  // individual "check box" was chosen.
                {
                    opts->value = toggled;
                } // else

                return;  // we found it, bail.
            } // if
        } // if

        if (opts->value) // if option is toggled on, descend to children.
            toggle_option(opts, opts->child, line, target);
        toggle_option(parent, opts->next_sibling, line, target);
    } // if
} // toggle_option


static void print_options(MojoGuiSetupOptions *opts, int *line, int level)
{
    if (opts != NULL)
    {
        int i;
        int spacing = 1;
        if (opts->is_group_parent)
            spacing += 6;
        else
        {
            (*line)++;
            printf("%2d  [%c]", *line, opts->value ? 'X' : ' ');
        } // else

        for (i = 0; i < (level + spacing); i++)
            putchar(' ');

        printf("%s%s\n", opts->description, opts->is_group_parent ? ":" : "");

        if ((opts->value) || (opts->is_group_parent))
            print_options(opts->child, line, level+1);
        print_options(opts->next_sibling, line, level);
    } // if
} // print_options


static int MojoGui_stdio_options(MojoGuiSetupOptions *opts,
                                 boolean can_back, boolean can_fwd)
{
    const char *prompt = entry->xstrdup(entry->_("Choose number to change."));
    const char *inst_opts_str = entry->xstrdup(entry->_("Install options:"));
    int retval = -1;
    boolean getout = false;
    char buf[128];
    int len = 0;

    while (!getout)
    {
        int line = 0;

        printf("\n\n");
        printf(inst_opts_str);
        printf("\n");
        print_options(opts, &line, 1);
        printf("\n");

        if ((len = readstr(prompt, buf, sizeof (buf), can_back, true)) < 0)
            getout = true;
        else if (len == 0)
        {
            getout = true;
            retval = 1;
        } // else if
        else
        {
            char *endptr = NULL;
            int target = (int) strtol(buf, &endptr, 10);
            if (*endptr == '\0')  // complete string was a valid number?
            {
                line = 0;
                toggle_option(NULL, opts, &line, target);
            } // if
        } // else
    } // while

    free((void *) inst_opts_str);
    free((void *) prompt);

    return retval;
} // MojoGui_stdio_options


static char *MojoGui_stdio_destination(const char **recommends, int recnum,
                                       int *command, boolean can_back,
                                       boolean can_fwd)
{
    const char *instdeststr = entry->xstrdup(entry->_("Destination"));
    const char *prompt = NULL;
    char *retval = NULL;
    boolean getout = false;
    char buf[128];
    int len = 0;
    int i = 0;

    *command = -1;

    if (recnum > 0)
        prompt = entry->xstrdup(entry->_("Choose install destination by number (hit enter for #1), or enter your own."));
    else
        prompt = entry->xstrdup(entry->_("Enter path where files will be installed."));

    while (!getout)
    {
        printf("\n\n%s\n", instdeststr);
        for (i = 0; i < recnum; i++)
            printf("  %2d  %s\n", i+1, recommends[i]);
        printf("\n");

        if ((len = readstr(prompt, buf, sizeof (buf), can_back, false)) < 0)
            getout = true;

        else if ((len == 0) && (recnum > 0))   // default to first in list.
        {
            retval = entry->xstrdup(recommends[0]);
            *command = 1;
            getout = true;
        } // else if

        else if (len > 0)
        {
            char *endptr = NULL;
            int target = (int) strtol(buf, &endptr, 10);
            // complete string was a valid number?
            if ((*endptr == '\0') && (target > 0) && (target <= recnum))
                retval = entry->xstrdup(recommends[target-1]);
            else
                retval = entry->xstrdup(buf);

            *command = 1;
            getout = true;
        } // else
    } // while

    free((void *) prompt);
    free((void *) instdeststr);

    return retval;
} // MojoGui_stdio_destination


static boolean MojoGui_stdio_insertmedia(const char *medianame)
{
    char buf[32];
    printf(entry->_("Please insert '%s'"), medianame);
    printf("\n");
    return (readstr(NULL, buf, sizeof (buf), false, true) >= 0);
} // MojoGui_stdio_insertmedia


static boolean MojoGui_stdio_progress(const char *type, const char *component,
                                      int percent, const char *item)
{
    const uint32 now = entry->ticks();

    if ( (lastComponent == NULL) ||
         (strcmp(lastComponent, component) != 0) ||
         (lastProgressType == NULL) ||
         (strcmp(lastProgressType, type) != 0) )
    {
        free(lastProgressType);
        free(lastComponent);
        lastProgressType = entry->xstrdup(type);
        lastComponent = entry->xstrdup(component);
        printf("%s\n%s\n", type, component);
    } // if

    // limit update spam... will only write every one second, tops.
    if (percentTicks <= now)
    {
        percentTicks = now + 1000;
        // !!! FIXME: localization.
        if (percent < 0)
            printf(entry->_("%s\n"), item);
        else
            printf(entry->_("%s (total progress: %d%%)\n"), item, percent);
    } // if

    return true;
} // MojoGui_stdio_progress


static void MojoGui_stdio_final(const char *msg)
{
    printf("%s\n\n", msg);
    fflush(stdout);
} // MojoGui_stdio_final

// end of gui_stdio.c ...


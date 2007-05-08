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

static char *lastType = NULL;
static char *lastComponent = NULL;
static int lastPercent = -1;
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


static uint8 MojoGui_stdio_priority(void)
{
    return MOJOGUI_PRIORITY_TRY_LAST;  // always a last resort.
} // MojoGui_stdio_priority


static boolean MojoGui_stdio_init(void)
{
    free(lastType);
    lastType = NULL;
    free(lastComponent);
    lastComponent = NULL;
    lastPercent = -1;
    percentTicks = 0;
    return true;   // always succeeds.
} // MojoGui_stdio_init


static void MojoGui_stdio_deinit(void)
{
    // no-op
} // MojoGui_stdio_deinit


static void MojoGui_stdio_msgbox(const char *title, const char *text)
{
    char buf[128];
    printf(entry->_("NOTICE: %s\n[hit enter]"), text);
    fflush(stdout);
    read_stdin(buf, sizeof (buf));
} // MojoGui_stdio_msgbox


static boolean MojoGui_stdio_promptyn(const char *title, const char *text)
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
                                    size_t datalen, boolean can_go_back,
                                    boolean can_go_forward)
{
    boolean getout = false;
    boolean retval = false;
    int len = 0;
    char buf[128];

    while (!getout)
    {
        printf("%s\n%s\n", name, (const char *) data);
        if ((len = readstr(NULL, buf, sizeof (buf), can_go_back, true)) < 0)
            getout = true;
        else if (len == 0)
            getout = retval = true;
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
            spacing += 7;
        else
        {
            (*line)++;
            printf("%2d  [%c]", *line, opts->value ? 'X' : ' ');
        } // else

        for (i = 0; i < ((level*2) + spacing); i++)
            putchar(' ');

        puts(opts->description);

        if (opts->value)
            print_options(opts->child, line, level+1);
        print_options(opts->next_sibling, line, level);
    } // if
} // print_options


static boolean MojoGui_stdio_options(MojoGuiSetupOptions *opts,
                       boolean can_go_back, boolean can_go_forward)
{
    const char *prompt = entry->xstrdup(entry->_("Choose number to change."));
    const char *inst_opts_str = entry->xstrdup(entry->_("Install options:"));
    boolean retval = false;
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

        if ((len = readstr(prompt, buf, sizeof (buf), can_go_back, true)) < 0)
            getout = true;
        else if (len == 0)
            retval = getout = true;
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


static char *MojoGui_stdio_destination(const char **recommends, int reccount,
                                       boolean can_go_back, boolean can_go_fwd)
{
    const char *instdeststr = entry->xstrdup(entry->_("Install destination:"));
    const char *prompt = NULL;
    char *retval = NULL;
    boolean getout = false;
    char buf[128];
    int len = 0;
    int i = 0;

    if (reccount > 0)
        prompt = entry->xstrdup(entry->_("Choose install destination by number, or enter your own."));
    else
        prompt = entry->xstrdup(entry->_("Enter path where files will be installed."));

    while (!getout)
    {
        printf("\n\n%s\n", instdeststr);
        for (i = 0; i < reccount; i++)
            printf("  %2d  %s\n", i+1, recommends[i]);
        printf("\n");

        if ((len = readstr(prompt, buf, sizeof (buf), can_go_back, false)) < 0)
            getout = true;
        else if (len > 0)
        {
            char *endptr = NULL;
            int target = (int) strtol(buf, &endptr, 10);
            // complete string was a valid number?
            if ((*endptr == '\0') && (target > 0) && (target <= reccount))
                retval = entry->xstrdup(recommends[target-1]);
            else
                retval = entry->xstrdup(buf);

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
    printf(entry->_("Please insert '%s'\n"), medianame);
    return (readstr(NULL, buf, sizeof (buf), false, true) >= 0);
} // MojoGui_stdio_insertmedia


static boolean MojoGui_stdio_progress(const char *type, const char *component,
                                      int percent, const char *item)
{
    const uint32 now = entry->ticks();
    if ((lastType == NULL) || (strcmp(lastType, type) != 0))
    {
        percentTicks = 0;
        lastPercent = -1;
        free(lastType);
        lastType = entry->xstrdup(type);
        printf("%s\n", type);
    } // if

    if ((lastComponent == NULL) || (strcmp(lastComponent, component) != 0))
    {
        percentTicks = 0;
        free(lastComponent);
        lastComponent = entry->xstrdup(component);
        printf("%s\n", component);
    } // if

    // limit update spam... will only write every two seconds, tops.
    if (percentTicks <= now)
    {
        percentTicks = now + 2000;
        if (percent != lastPercent)
        {
            lastPercent = percent;
            // !!! FIXME: localization.
            printf(entry->_("%s (total %d%%)\n"), item, percent);
        } // if
    } // if

    return true;
} // MojoGui_stdio_progress

// end of gui_stdio.c ...

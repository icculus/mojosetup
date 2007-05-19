/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#if !SUPPORT_GUI_NCURSES
#error Something is wrong in the build system.
#endif

#define BUILDING_EXTERNAL_PLUGIN 1
#include "gui.h"

MOJOGUI_PLUGIN(ncurses)

#if !GUI_STATIC_LINK_NCURSES
CREATE_MOJOGUI_ENTRY_POINT(ncurses)
#endif

#include <unistd.h>
#include <ctype.h>
#include <curses.h>

// This was built to look roughly like dialog(1), but it's not nearly as
//  robust. Also, I didn't use any of dialog's code, as it is GPL/LGPL,
//  depending on what version you start with. There _is_ a libdialog, but
//  it's never something installed on any systems, and I can't link it
//  statically due to the license.
//
// ncurses is almost always installed as a shared library, though, so we'll
//  just talk to it directly. Fortunately we don't need much of what dialog(1)
//  offers, so rolling our own isn't too painful.
//
// Pradeep Padala's ncurses HOWTO was very helpful in teaching me curses
//  quickly: http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html

typedef enum
{
    MOJOCOLOR_BACKGROUND=1,
    MOJOCOLOR_BORDERTOP,
    MOJOCOLOR_BORDERBOTTOM,
    MOJOCOLOR_BORDERSHADOW,
    MOJOCOLOR_TEXT,
    MOJOCOLOR_BUTTONHOVER,
    MOJOCOLOR_BUTTONNORMAL,
} MojoColor;


static int strcells(const char *str)
{
    // !!! FIXME: how to know how many _cells_ UTF-8 strings take in cursesw?
    return (int) strlen(str);
} // strcells


typedef struct
{
    WINDOW *mainwin;
    WINDOW *textwin;
    WINDOW **buttons;
    char *title;
    char *text;
    char **textlines;
    char **buttontext;
    int buttoncount;
    int textlinecount;
    int hoverover;
    int textpos;
    boolean hidecursor;
    boolean ndelay;
    int cursval;
} MojoBox;


// !!! FIXME: this is not really Unicode friendly...
static char **splitText(char *text, int *_count, int *_w)
{
    int i;
    int scrw, scrh;
    char **retval = NULL;
    getmaxyx(stdscr, scrh, scrw);
    int count = 0;
    int w = 0;

    *_count = *_w = 0;
    while (*text)
    {
        int furthest = 0;
        for (i = 0; (*text) && (i < (scrw-4)); i++)
        {
            const int ch = text[i];
            if ((ch == '\r') || (ch == '\n'))
            {
                text[i] = '\0';
                count++;
                retval = (char **) entry->xrealloc(retval,
                                                   count * sizeof (char *));
                retval[count-1] = entry->xstrdup(text);
                *text = ch;
                text += i;
                if ((ch == '\r') && (text[1] == '\n'))
                    text++;
                text++;

                if (i > w)
                    w = i;
                i = -1;  // will be zero on next iteration...
            } // if
            else if (isspace(ch))
            {
                furthest = i;
            } // else if
        } // for

        if (*text)  // line overflow, need to wrap...
        {
            int pos = furthest;
            char ch;
            if (furthest == 0)  // uhoh, no split at all...hack it.
            {
                // !!! FIXME: might be chopping in the middle of a UTF-8 seq.
                pos = strlen(text);
                if (pos > scrw-4)
                    pos = scrw-4;
            } // if

            ch = text[pos];
            text[pos] = '\0';
            count++;
            retval = (char **) entry->xrealloc(retval, count * sizeof (char*));
            retval[count-1] = entry->xstrdup(text);
            *text = ch;
            text += pos;
            if (pos > w)
                w = pos;
        } // if
    } // while

    *_count = count;
    *_w = w;
    return retval;
} // splitText


static void drawButton(MojoBox *mojobox, int button)
{
    const boolean hover = (mojobox->hoverover == button);
    WINDOW *win = mojobox->buttons[button];
    const char *str = mojobox->buttontext[button];
    int w, h;
    getmaxyx(win, h, w);

    if (hover)
        wbkgdset(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER));
    else
        wbkgdset(win, COLOR_PAIR(MOJOCOLOR_BUTTONNORMAL));

    wclear(win);
    wmove(win, 0, 0);
    waddch(win, '<');
    wmove(win, 0, w-1);
    waddch(win, '>');
    wmove(win, 0, 2);
    waddstr(win, str);
} // drawButton


static void drawText(MojoBox *mojobox)
{
    int i;
    wclear(mojobox->textwin);
    for (i = 0; i+mojobox->textpos < mojobox->textlinecount; i++)
        mvwaddstr(mojobox->textwin, i, 0, mojobox->textlines[i]);
} // drawText


static MojoBox *makeBox(const char *title, const char *text,
                        char **buttons, int bcount,
                        boolean ndelay, boolean hidecursor)
{
    MojoBox *retval = NULL;
    WINDOW *win = NULL;
    int scrw, scrh;
    int len = 0;
    int buttonsw = 0;
    int h = 0;
    int w = 0;
    int texth = 0;
    int i;

    getmaxyx(stdscr, scrh, scrw);
    if (scrw < 16)
        return NULL;

    retval = (MojoBox *) entry->xmalloc(sizeof (MojoBox));
    retval->hidecursor = hidecursor;
    retval->ndelay = ndelay;
    retval->cursval = ((hidecursor) ? curs_set(0) : ERR);
    retval->title = entry->xstrdup(title);
    retval->text = entry->xstrdup(text);
    retval->buttoncount = bcount;
    retval->buttons = (WINDOW **) entry->xmalloc(sizeof (WINDOW*) * bcount);
    retval->buttontext = (char **) entry->xmalloc(sizeof (char*) * bcount);

    for (i = 0; i < bcount; i++)
        retval->buttontext[i] = entry->xstrdup(buttons[i]);

    retval->textlines = splitText(retval->text, &retval->textlinecount, &w);

    len = strcells(title);
    if (len > scrw-4)
    {
        len = scrw-4;
        strcpy(&retval->title[len-3], "...");  // !!! FIXME: not Unicode safe!
    } // if

    if (len > w)
        w = len;

    if (bcount > 0)
    {
        for (i = 0; i < bcount; i++)
            buttonsw += strcells(buttons[i]) + 6;
        if (buttonsw > w)
            w = buttonsw;
        // !!! FIXME: what if these overflow the screen?
    } // if

    w += 4;
    h = retval->textlinecount + 2;
    if (bcount > 0)
        h += 2;

    if (h > scrh)
        h = scrh;

    win = retval->mainwin = newwin(h, w, (scrh - h) / 2, (scrw - w) / 2);
	keypad(win, TRUE);
    nodelay(win, ndelay);
    wbkgdset(win, COLOR_PAIR(MOJOCOLOR_TEXT));
    wclear(win);
    waddch(win, ACS_ULCORNER | A_BOLD | COLOR_PAIR(MOJOCOLOR_BORDERTOP));
    whline(win, ACS_HLINE | A_BOLD | COLOR_PAIR(MOJOCOLOR_BORDERTOP), w-2);
    wmove(win, 0, w-1);
    waddch(win, ACS_URCORNER | COLOR_PAIR(MOJOCOLOR_BORDERBOTTOM));
    wmove(win, 1, 0);
    wvline(win, ACS_VLINE | A_BOLD | COLOR_PAIR(MOJOCOLOR_BORDERTOP), h-2);
    wmove(win, 1, w-1);
    wvline(win, ACS_VLINE | COLOR_PAIR(MOJOCOLOR_BORDERBOTTOM), h-2);
    wmove(win, h-1, 0);
    waddch(win, ACS_LLCORNER | A_BOLD | COLOR_PAIR(MOJOCOLOR_BORDERTOP));
    whline(win, ACS_HLINE | COLOR_PAIR(MOJOCOLOR_BORDERBOTTOM), w-2);
    wmove(win, h-1, w-1);
    waddch(win, ACS_LRCORNER | COLOR_PAIR(MOJOCOLOR_BORDERBOTTOM));

    len = strcells(retval->title);
    wmove(win, 0, ((w-len)/2)-1);
    waddch(win, ' ' | COLOR_PAIR(MOJOCOLOR_TEXT));
    waddstr(win, retval->title);
    wmove(win, 0, ((w-len)/2)+len);
    waddch(win, ' ' | COLOR_PAIR(MOJOCOLOR_TEXT));

    if (bcount > 0)
    {
        int pos = (w - buttonsw) / 2;
        wmove(win, h-3, 1);
        whline(win, ACS_HLINE | A_BOLD | COLOR_PAIR(MOJOCOLOR_BORDERTOP), w-2);
        for (i = 0; i < bcount; i++)
        {
            len = strcells(buttons[i]) + 4;
            pos += len-2;
            win = retval->buttons[i] = newwin(1, len, (((scrh - h) / 2) + h)-2,
                                              (((scrw - w) / 2) + w)-pos);

	        keypad(win, TRUE);
            nodelay(win, ndelay);
        } // for
    } // if

    texth = h-2;
    if (bcount > 0)
        texth -= 2;
    win = retval->textwin = newwin(texth, w-4, ((scrh-h)/2)+1, ((scrw-w)/2)+2);
	keypad(win, TRUE);
    nodelay(win, ndelay);
    wbkgdset(win, COLOR_PAIR(MOJOCOLOR_TEXT));
    drawText(retval);

    wclear(stdscr);
    wrefresh(stdscr);
    wrefresh(retval->mainwin);
    wrefresh(retval->textwin);
    for (i = 0; i < bcount; i++)
    {
        drawButton(retval, i);
        wrefresh(retval->buttons[i]);
    } // for

    return retval;
} // makeBox


static void freeBox(MojoBox *mojobox, boolean clearscreen)
{
    if (mojobox != NULL)
    {
        int i;
        const int bcount = mojobox->buttoncount;
        const int tcount = mojobox->textlinecount;

        if (mojobox->cursval != ERR)
            curs_set(mojobox->cursval);

        for (i = 0; i < bcount; i++)
        {
            free(mojobox->buttontext[i]);
            delwin(mojobox->buttons[i]);
        } // for

        free(mojobox->buttontext);
        free(mojobox->buttons);

        delwin(mojobox->textwin);
        delwin(mojobox->mainwin);

        free(mojobox->title);
        free(mojobox->text);

        for (i = 0; i < tcount; i++)
            free(mojobox->textlines[i]);

        free(mojobox->textlines);
        free(mojobox);

        if (clearscreen)
        {
            wclear(stdscr);
            wrefresh(stdscr);
        } // if
    } // if
} // freeBox


static int upkeepBox(MojoBox **_mojobox, int ch)
{
    MojoBox *mojobox = *_mojobox;
    if (mojobox == NULL)
        return -2;

    switch (ch)
    {
        case ERR:
            return -2;

        case '\r':
        case '\n':
        case KEY_ENTER:
        case ' ':
            return mojobox->hoverover;

        case '\e':
            return mojobox->buttoncount-1;

        case KEY_RESIZE:
            mojobox = makeBox(mojobox->title, mojobox->text,
                              mojobox->buttontext, mojobox->buttoncount,
                              mojobox->ndelay, mojobox->hidecursor);
            freeBox(*_mojobox, false);
            *_mojobox = mojobox;
            return -1;
    } // switch

    return -1;
} // upkeepBox


static char *lastComponent = NULL;
static uint32 percentTicks = 0;

static uint8 MojoGui_ncurses_priority(void)
{
    if (getenv("DISPLAY") != NULL)
        return MOJOGUI_PRIORITY_TRY_LAST;  // let graphical stuff go first.
    return MOJOGUI_PRIORITY_TRY_NORMAL;
} // MojoGui_ncurses_priority


static boolean MojoGui_ncurses_init(void)
{
    const char *badtty = NULL;
    if (!isatty(0))
        badtty = "stdin";
    else if (!isatty(1))
        badtty = "stdout";

    if (badtty != NULL)
    {
        entry->logInfo("ncurses: %s is not a tty, use another UI.", badtty);
        return false;  // stdin or stdout redirected, or maybe no xterm...
    } // if

    if (initscr() == NULL)
    {
        entry->logInfo("ncurses: initscr() failed, use another UI.");
        return false;
    } // if

	cbreak();
	keypad(stdscr, TRUE);
	noecho();
    //nodelay(stdscr, TRUE);
    start_color();
    init_pair(MOJOCOLOR_BACKGROUND, COLOR_BLUE, COLOR_BLUE);
    init_pair(MOJOCOLOR_BORDERTOP, COLOR_WHITE, COLOR_WHITE);
    init_pair(MOJOCOLOR_BORDERBOTTOM, COLOR_BLACK, COLOR_WHITE);
    init_pair(MOJOCOLOR_BORDERSHADOW, COLOR_BLACK, COLOR_BLACK);
    init_pair(MOJOCOLOR_TEXT, COLOR_BLACK, COLOR_WHITE);
    init_pair(MOJOCOLOR_BUTTONHOVER, COLOR_WHITE, COLOR_BLUE);
    init_pair(MOJOCOLOR_BUTTONNORMAL, COLOR_BLACK, COLOR_WHITE);
    wbkgdset(stdscr, COLOR_PAIR(MOJOCOLOR_BACKGROUND));
    wclear(stdscr);
    wrefresh(stdscr);

    percentTicks = 0;
    return true;   // always succeeds.
} // MojoGui_ncurses_init


static void MojoGui_ncurses_deinit(void)
{
    endwin();
    delwin(stdscr);  // not sure if this is safe, but valgrind said it leaks.
    stdscr = NULL;
    free(lastComponent);
    lastComponent = NULL;
} // MojoGui_ncurses_deinit


static void MojoGui_ncurses_msgbox(const char *title, const char *text)
{
    char *localized_ok = entry->xstrdup(entry->_("OK"));
    MojoBox *mojobox = makeBox(title, text, &localized_ok, 1, false, true);
    while (upkeepBox(&mojobox, wgetch(mojobox->mainwin)) == -1) {}
    freeBox(mojobox, true);
    free(localized_ok);
} // MojoGui_ncurses_msgbox


static boolean MojoGui_ncurses_promptyn(const char *title, const char *text)
{
    char *localized_yes = entry->xstrdup(entry->_("Yes"));
    char *localized_no = entry->xstrdup(entry->_("No"));
    char *buttons[] = { localized_yes, localized_no };
    MojoBox *mojobox = makeBox(title, text, buttons, 2, false, true);
    int rc = 0;
    while ((rc = upkeepBox(&mojobox, wgetch(mojobox->mainwin))) == -1) {}
    freeBox(mojobox, true);
    free(localized_yes);
    free(localized_no);
    return (rc == 0);
} // MojoGui_ncurses_promptyn


static MojoGuiYNAN MojoGui_ncurses_promptynan(const char *title,
                                              const char *text)
{
    char *loc_yes = entry->xstrdup(entry->_("Yes"));
    char *loc_no = entry->xstrdup(entry->_("No"));
    char *loc_always = entry->xstrdup(entry->_("Always"));
    char *loc_never = entry->xstrdup(entry->_("Never"));
    char *buttons[] = { loc_yes, loc_always, loc_never, loc_no };
    MojoBox *mojobox = makeBox(title, text, buttons, 4, false, true);
    int rc = 0;
    while ((rc = upkeepBox(&mojobox, wgetch(mojobox->mainwin))) == -1) {}
    freeBox(mojobox, true);
    free(loc_yes);
    free(loc_no);
    free(loc_always);
    free(loc_never);

    switch (rc)
    {
        case 0: return MOJOGUI_YES;
        case 1: return MOJOGUI_ALWAYS;
        case 2: return MOJOGUI_NEVER;
        case 3: return MOJOGUI_NO;
    } // switch

    assert(false && "BUG: unhandled case in switch statement!");
    return MOJOGUI_NO;
} // MojoGui_ncurses_promptynan


static boolean MojoGui_ncurses_start(const char *title, const char *splash)
{
    wclear(stdscr);
    wrefresh(stdscr);
    return true;
} // MojoGui_ncurses_start


static void MojoGui_ncurses_stop(void)
{
    wclear(stdscr);
    wrefresh(stdscr);
} // MojoGui_ncurses_stop


static int MojoGui_ncurses_readme(const char *name, const uint8 *data,
                                    size_t datalen, boolean can_back,
                                    boolean can_fwd)
{
    MojoBox *mojobox = NULL;
    char *buttons[3] = { NULL, NULL, NULL };
    int bcount = 0;
    int backbutton = -99;
    int fwdbutton = -99;
    int rc = 0;
    int i = 0;

    if (can_fwd)
    {
        fwdbutton = bcount++;
        buttons[fwdbutton] = entry->xstrdup(entry->_("Next"));
    } // if

    if (can_back)
    {
        backbutton = bcount++;
        buttons[backbutton] = entry->xstrdup(entry->_("Back"));
    } // if

    buttons[bcount++] = entry->xstrdup(entry->_("Cancel"));

    mojobox = makeBox(name, (char *) data, buttons, bcount, false, true);
    while ((rc = upkeepBox(&mojobox, wgetch(mojobox->mainwin))) == -1) {}
    freeBox(mojobox, true);

    for (i = 0; i < bcount; i++)
        free(buttons[i]);

    if (rc == backbutton)
        return -1;
    else if (rc == fwdbutton)
        return 1;

    return 0;  // error? Cancel?
} // MojoGui_ncurses_readme


static int MojoGui_ncurses_options(MojoGuiSetupOptions *opts,
                                 boolean can_back, boolean can_fwd)
{
    return 1;
} // MojoGui_ncurses_options


static char *MojoGui_ncurses_destination(const char **recommends, int recnum,
                                       int *command, boolean can_back,
                                       boolean can_fwd)
{
    return NULL;
} // MojoGui_ncurses_destination


static boolean MojoGui_ncurses_insertmedia(const char *medianame)
{
    char *fmt = entry->xstrdup(entry->_("Please insert '%s'"));
    const size_t len = strlen(fmt) + strlen(medianame) + 1;
    char *text = (char *) entry->xmalloc(len);
    char *localized_ok = entry->xstrdup(entry->_("Ok"));
    char *localized_cancel = entry->xstrdup(entry->_("Cancel"));
    char *buttons[] = { localized_ok, localized_cancel };
    MojoBox *mojobox = NULL;
    int rc = 0;

    snprintf(text, len, fmt, medianame);
    free(fmt);

    mojobox = makeBox(entry->_("Media change"), text, buttons, 2, false, true);
    while ((rc = upkeepBox(&mojobox, wgetch(mojobox->mainwin))) == -1) {}

    freeBox(mojobox, true);
    free(text);
    free(localized_cancel);
    free(localized_ok);
    return (rc == 0);
} // MojoGui_ncurses_insertmedia


static boolean MojoGui_ncurses_progress(const char *type, const char *component,
                                      int percent, const char *item)
{
    return true;
} // MojoGui_ncurses_progress


static void MojoGui_ncurses_final(const char *msg)
{
    MojoGui_ncurses_msgbox(entry->_("Finish"), msg);
} // MojoGui_ncurses_final

// end of gui_ncurses.c ...


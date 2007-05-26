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
//  offers, so rolling our own isn't too painful (well, compared to massive
//  head trauma, I guess).
//
// Pradeep Padala's ncurses HOWTO was very helpful in teaching me curses
//  quickly: http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html

typedef enum
{
    MOJOCOLOR_BACKGROUND=1,
    MOJOCOLOR_BORDERTOP,
    MOJOCOLOR_BORDERBOTTOM,
    MOJOCOLOR_BORDERTITLE,
    MOJOCOLOR_TEXT,
    MOJOCOLOR_BUTTONHOVER,
    MOJOCOLOR_BUTTONNORMAL,
    MOJOCOLOR_BUTTONBORDER,
    MOJOCOLOR_TODO,
    MOJOCOLOR_DONE,
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


static char *lastProgressType = NULL;
static char *lastComponent = NULL;
static uint32 percentTicks = 0;
static char *title = NULL;
static MojoBox *progressBox = NULL;


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
        int pos = 0;
        int furthest = 0;

        for (i = 0; (text[i]) && (i < (scrw-4)); i++)
        {
            const int ch = text[i];
            if ((ch == '\r') || (ch == '\n'))
            {
                count++;
                retval = (char **) entry->xrealloc(retval,
                                                   count * sizeof (char *));
                text[i] = '\0';
                retval[count-1] = entry->xstrdup(text);
                text += i;
                *text = ch;
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

        // line overflow or end of stream...
        pos = (text[i]) ? furthest : i;
        if ((text[i]) && (furthest == 0))  // uhoh, no split at all...hack it.
        {
            // !!! FIXME: might be chopping in the middle of a UTF-8 seq.
            pos = strlen(text);
            if (pos > scrw-4)
                pos = scrw-4;
        } // if

        if (pos > 0)
        {
            char ch = text[pos];
            count++;
            retval = (char **) entry->xrealloc(retval, count * sizeof (char*));
            text[pos] = '\0';
            retval[count-1] = entry->xstrdup(text);
            text += pos;
            *text = ch;
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
    int borderattr = 0;
    WINDOW *win = mojobox->buttons[button];
    const char *str = mojobox->buttontext[button];
    int w, h;
    getmaxyx(win, h, w);

    if (!hover)
        wbkgdset(win, COLOR_PAIR(MOJOCOLOR_BUTTONNORMAL));
    else
    {
        borderattr = COLOR_PAIR(MOJOCOLOR_BUTTONBORDER) | A_BOLD;
        wbkgdset(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER));
    } // else

    werase(win);
    wmove(win, 0, 0);
    waddch(win, borderattr | '<');
    wmove(win, 0, w-1);
    waddch(win, borderattr | '>');
    wmove(win, 0, 2);

    if (!hover)
        waddstr(win, str);
    else
    {
        wattron(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
        waddstr(win, str);
        wattroff(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
    } // else
} // drawButton


static void drawText(MojoBox *mojobox)
{
    int i;
    const int tcount = mojobox->textlinecount;
    int pos = mojobox->textpos;
    int w, h;
    WINDOW *win = mojobox->textwin;
    getmaxyx(win, h, w);

    werase(mojobox->textwin);
    for (i = 0; (pos < tcount) && (i < h); i++, pos++)
        mvwaddstr(win, i, 0, mojobox->textlines[pos]);

    if (tcount > h)
    {
        const int pct = (int) ((((double) pos) / ((double) tcount)) * 100.0);
        win = mojobox->mainwin;
        wattron(win, COLOR_PAIR(MOJOCOLOR_BORDERTITLE) | A_BOLD);
        mvwprintw(win, h+1, w-5, "(%3d%%)", pct);
        wattroff(win, COLOR_PAIR(MOJOCOLOR_BORDERTITLE) | A_BOLD);
    } // if
} // drawText


static void drawBackground(WINDOW *win)
{
    wclear(win);
    if (title != NULL)
    {
        int w, h;
        getmaxyx(win, h, w);
        wattron(win, COLOR_PAIR(MOJOCOLOR_BACKGROUND) | A_BOLD);
        mvwaddstr(win, 0, 0, title);
        mvwhline(win, 1, 1, ACS_HLINE, w-2);
        wattroff(win, COLOR_PAIR(MOJOCOLOR_BACKGROUND) | A_BOLD);
    } // if
} // drawBackground


static MojoBox *makeBox(const char *title, const char *text,
                        char **buttons, int bcount,
                        boolean ndelay, boolean hidecursor)
{
    MojoBox *retval = NULL;
    WINDOW *win = NULL;
    int scrw, scrh;
    int len = 0;
    int buttonsw = 0;
    int x = 0;
    int y = 0;
    int h = 0;
    int w = 0;
    int texth = 0;
    int i;

    getmaxyx(stdscr, scrh, scrw);
    scrh--; // -1 to save the title at the top of the screen...
    if ((scrw < 16) || (scrh < 16))
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
            buttonsw += strcells(buttons[i]) + 5;  // '<', ' ', ' ', '>', ' '
        if (buttonsw > w)
            w = buttonsw;
        // !!! FIXME: what if these overflow the screen?
    } // if

    w += 4;
    h = retval->textlinecount + 2;
    if (bcount > 0)
        h += 2;

    if (h > scrh-2)
        h = scrh-2;

    x = (scrw - w) / 2;
    y = ((scrh - h) / 2) + 1;
    win = retval->mainwin = newwin(h, w, y, x);
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
    wattron(win, COLOR_PAIR(MOJOCOLOR_BORDERTITLE) | A_BOLD);
    waddch(win, ' ');
    waddstr(win, retval->title);
    wmove(win, 0, ((w-len)/2)+len);
    waddch(win, ' ');
    wattroff(win, COLOR_PAIR(MOJOCOLOR_BORDERTITLE) | A_BOLD);

    if (bcount > 0)
    {
        const int buttony = (y + h) - 2;
        int buttonx = (x + w) - ((w - buttonsw) / 2);
        wmove(win, h-3, 1);
        whline(win, ACS_HLINE | A_BOLD | COLOR_PAIR(MOJOCOLOR_BORDERTOP), w-2);
        for (i = 0; i < bcount; i++)
        {
            len = strcells(buttons[i]) + 4;
            buttonx -= len+1;
            win = retval->buttons[i] = newwin(1, len, buttony, buttonx);
            keypad(win, TRUE);
            nodelay(win, ndelay);
        } // for
    } // if

    texth = h-2;
    if (bcount > 0)
        texth -= 2;
    win = retval->textwin = newwin(texth, w-4, y+1, x+2);
	keypad(win, TRUE);
    nodelay(win, ndelay);
    wbkgdset(win, COLOR_PAIR(MOJOCOLOR_TEXT));
    drawText(retval);

    drawBackground(stdscr);
    wnoutrefresh(stdscr);
    wnoutrefresh(retval->mainwin);
    wnoutrefresh(retval->textwin);
    for (i = 0; i < bcount; i++)
    {
        drawButton(retval, i);
        wnoutrefresh(retval->buttons[i]);
    } // for

    doupdate();  // push it all to the screen.

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
    static boolean justResized = false;
    MEVENT mevt;
    int w, h;
    MojoBox *mojobox = *_mojobox;
    if (mojobox == NULL)
        return -2;

    if (justResized)   // !!! FIXME: this is a kludge.
    {
        justResized = false;
        if (ch == ERR)
            return -1;
    } // if

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

        case KEY_UP:
            if (mojobox->textpos > 0)
            {
                mojobox->textpos--;
                drawText(mojobox);
                wrefresh(mojobox->textwin);
            } // if
            return -1;

        case KEY_DOWN:
            getmaxyx(mojobox->textwin, h, w);
            if (mojobox->textpos < (mojobox->textlinecount-h))
            {
                mojobox->textpos++;
                drawText(mojobox);
                wrefresh(mojobox->textwin);
            } // if
            return -1;

        case KEY_PPAGE:
            if (mojobox->textpos > 0)
            {
                getmaxyx(mojobox->textwin, h, w);
                mojobox->textpos -= h;
                if (mojobox->textpos < 0)
                    mojobox->textpos = 0;
                drawText(mojobox);
                wrefresh(mojobox->textwin);
            } // if
            return -1;

        case KEY_NPAGE:
            getmaxyx(mojobox->textwin, h, w);
            if (mojobox->textpos < (mojobox->textlinecount-h))
            {
                mojobox->textpos += h;
                if (mojobox->textpos > (mojobox->textlinecount-h))
                    mojobox->textpos = (mojobox->textlinecount-h);
                drawText(mojobox);
                wrefresh(mojobox->textwin);
            } // if
            return -1;

        case KEY_LEFT:
            if (mojobox->buttoncount > 1)
            {
                if (mojobox->hoverover < (mojobox->buttoncount-1))
                {
                    mojobox->hoverover++;
                    drawButton(mojobox, mojobox->hoverover-1);
                    drawButton(mojobox, mojobox->hoverover);
                    wrefresh(mojobox->buttons[mojobox->hoverover-1]);
                    wrefresh(mojobox->buttons[mojobox->hoverover]);
                } // if
            } // if
            return -1;

        case KEY_RIGHT:
            if (mojobox->buttoncount > 1)
            {
                if (mojobox->hoverover > 0)
                {
                    mojobox->hoverover--;
                    drawButton(mojobox, mojobox->hoverover+1);
                    drawButton(mojobox, mojobox->hoverover);
                    wrefresh(mojobox->buttons[mojobox->hoverover+1]);
                    wrefresh(mojobox->buttons[mojobox->hoverover]);
                } // if
            } // if
            return -1;

        case KEY_RESIZE:
            mojobox = makeBox(mojobox->title, mojobox->text,
                              mojobox->buttontext, mojobox->buttoncount,
                              mojobox->ndelay, mojobox->hidecursor);
            mojobox->cursval = (*_mojobox)->cursval;  // keep this sane.
            mojobox->hoverover = (*_mojobox)->hoverover;
            freeBox(*_mojobox, false);
            if (mojobox->hidecursor)
                curs_set(0); // make sure this stays sane.
            *_mojobox = mojobox;
            justResized = true;  // !!! FIXME: kludge.
            return -1;

        case KEY_MOUSE:
            if ((getmouse(&mevt) == OK) && (mevt.bstate & BUTTON1_CLICKED))
            {
                int i;
                for (i = 0; i < mojobox->buttoncount; i++)
                {
                    int x1, y1, x2, y2;
                    getbegyx(mojobox->buttons[i], y1, x1);
                    getmaxyx(mojobox->buttons[i], y2, x2);
                    x2 += x1;
                    y2 += y1;
                    if ( (mevt.x >= x1) && (mevt.x < x2) &&
                         (mevt.y >= y1) && (mevt.y < y2) )
                        return i;
                } // for
            } // if
            return -1;
    } // switch

    return -1;
} // upkeepBox


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
    start_color();
    mousemask(BUTTON1_CLICKED, NULL);
    init_pair(MOJOCOLOR_BACKGROUND, COLOR_CYAN, COLOR_BLUE);
    init_pair(MOJOCOLOR_BORDERTOP, COLOR_WHITE, COLOR_WHITE);
    init_pair(MOJOCOLOR_BORDERBOTTOM, COLOR_BLACK, COLOR_WHITE);
    init_pair(MOJOCOLOR_BORDERTITLE, COLOR_YELLOW, COLOR_WHITE);
    init_pair(MOJOCOLOR_TEXT, COLOR_BLACK, COLOR_WHITE);
    init_pair(MOJOCOLOR_BUTTONHOVER, COLOR_YELLOW, COLOR_BLUE);
    init_pair(MOJOCOLOR_BUTTONNORMAL, COLOR_BLACK, COLOR_WHITE);
    init_pair(MOJOCOLOR_BUTTONBORDER, COLOR_WHITE, COLOR_BLUE);
    init_pair(MOJOCOLOR_DONE, COLOR_YELLOW, COLOR_RED);
    init_pair(MOJOCOLOR_TODO, COLOR_CYAN, COLOR_BLUE);

    wbkgdset(stdscr, COLOR_PAIR(MOJOCOLOR_BACKGROUND));
    wclear(stdscr);
    wrefresh(stdscr);

    percentTicks = 0;
    return true;   // always succeeds.
} // MojoGui_ncurses_init


static void MojoGui_ncurses_deinit(void)
{
    freeBox(progressBox, false);
    progressBox = NULL;
    endwin();
    delwin(stdscr);  // not sure if this is safe, but valgrind said it leaks.
    stdscr = NULL;
    free(title);
    title = NULL;
    free(lastComponent);
    lastComponent = NULL;
    free(lastProgressType);
    lastProgressType = NULL;
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


static boolean MojoGui_ncurses_start(const char *_title, const char *splash)
{
    free(title);
    title = entry->xstrdup(_title);
    drawBackground(stdscr);
    wrefresh(stdscr);
    return true;
} // MojoGui_ncurses_start


static void MojoGui_ncurses_stop(void)
{
    free(title);
    title = NULL;
    drawBackground(stdscr);
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


static int toggle_option(MojoGuiSetupOptions *parent,
                         MojoGuiSetupOptions *opts, int *line, int target)
{
    int rc = -1;
    if ((opts != NULL) && (target > *line))
    {
        if (++(*line) == target)
        {
            const boolean toggled = ((opts->value) ? false : true);

            if (opts->is_group_parent)
                return 0;

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

            return 1;  // we found it, bail.
        } // if

        if (opts->value) // if option is toggled on, descend to children.
            rc = toggle_option(opts, opts->child, line, target);
        if (rc == -1)
            rc = toggle_option(parent, opts->next_sibling, line, target);
    } // if

    return rc;
} // toggle_option


// This code is pretty scary.
static void build_options(MojoGuiSetupOptions *opts, int *line, int level,
                          int maxw, char **lines)
{
    if (opts != NULL)
    {
        char *spacebuf = (char *) entry->xmalloc(maxw+1);
        char *buf = (char *) entry->xmalloc(maxw+1);
        int len = 0;
        int spacing = 1+level;

        if (spacing > (maxw-5))
            spacing = 0;  // oh well.

        if (spacing > 0)
            memset(spacebuf, ' ', spacing);  // null-term'd by xmalloc().

        if (opts->is_group_parent)
            len = snprintf(buf, maxw-2, "%s%s", spacebuf, opts->description);
        else
        {
            (*line)++;
            len = snprintf(buf, maxw-2, "%s[%c] %s", spacebuf,
                            opts->value ? 'X' : ' ',
                            opts->description);
        } // else

        free(spacebuf);

        if (len >= maxw-1)
            strcpy(buf+(maxw-4), "...");  // !!! FIXME: Unicode issues!

        *lines = (char*) entry->xrealloc(*lines, strlen(*lines)+strlen(buf)+2);
        strcat(*lines, buf);
        strcat(*lines, "\n");  // I'm sorry, Joel Spolsky!

        if ((opts->value) || (opts->is_group_parent))
            build_options(opts->child, line, level+1, maxw, lines);
        build_options(opts->next_sibling, line, level, maxw, lines);
    } // if
} // build_options


static int MojoGui_ncurses_options(MojoGuiSetupOptions *opts,
                                 boolean can_back, boolean can_fwd)
{
    char *title = entry->xstrdup(entry->_("Options"));
    MojoBox *mojobox = NULL;
    char *buttons[4] = { NULL, NULL, NULL, NULL };
    boolean ignoreerr = false;
    int lasthoverover = 0;
    int lasttextpos = 0;
    int bcount = 0;
    int backbutton = -99;
    int fwdbutton = -99;
    int togglebutton = -99;
    int cancelbutton = -99;
    int selected = 0;
    int ch = 0;
    int rc = -1;
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

    lasthoverover = togglebutton = bcount++;
    buttons[togglebutton] = entry->xstrdup(entry->_("Toggle"));
    cancelbutton = bcount++;
    buttons[cancelbutton] = entry->xstrdup(entry->_("Cancel"));

    do
    {
        if (mojobox == NULL)
        {
            int y = 0;
            int line = 0;
            int maxw, maxh;
            getmaxyx(stdscr, maxh, maxw);
            char *text = entry->xstrdup("");
            build_options(opts, &line, 1, maxw-6, &text);
            mojobox = makeBox(title, text, buttons, bcount, false, true);
            free(text);

            getmaxyx(mojobox->textwin, maxh, maxw);

            if (lasthoverover != mojobox->hoverover)
            {
                const int orighover = mojobox->hoverover;
                mojobox->hoverover = lasthoverover;
                drawButton(mojobox, orighover);
                drawButton(mojobox, lasthoverover);
                wrefresh(mojobox->buttons[orighover]);
                wrefresh(mojobox->buttons[lasthoverover]);
            } // if

            if (lasttextpos != mojobox->textpos)
            {
                mojobox->textpos = lasttextpos;
                drawText(mojobox);
            } // if

            if (selected >= (mojobox->textlinecount - 1))
                selected = mojobox->textlinecount - 1;
            if (selected >= mojobox->textpos+maxh)
                selected = (mojobox->textpos+maxh) - 1;
            y = selected - lasttextpos;

            wattron(mojobox->textwin, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
            mvwhline(mojobox->textwin, y, 0, ' ', maxw);
            mvwaddstr(mojobox->textwin, y, 0, mojobox->textlines[selected]);
            wattroff(mojobox->textwin, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
            wrefresh(mojobox->textwin);
        } // if

        lasttextpos = mojobox->textpos;
        lasthoverover = mojobox->hoverover;

        ch = wgetch(mojobox->mainwin);

        if (ignoreerr)  // kludge.
        {
            ignoreerr = false;
            if (ch == ERR)
                continue;
        } // if

        if (ch == KEY_RESIZE)
        {
            freeBox(mojobox, false);  // catch and rebuild without upkeepBox,
            mojobox = NULL;           //  so we can rebuild the text ourself.
            ignoreerr = true;  // kludge.
        } // if

        else if (ch == KEY_UP)
        {
            if (selected > 0)
            {
                WINDOW *win = mojobox->textwin;
                int maxw, maxh;
                int y = --selected - mojobox->textpos;
                getmaxyx(win, maxh, maxw);
                if (selected < mojobox->textpos)
                {
                    upkeepBox(&mojobox, ch);  // upkeepBox does scrolling
                    y++;
                } // if
                else
                {
                    wattron(win, COLOR_PAIR(MOJOCOLOR_TEXT));
                    mvwhline(win, y+1, 0, ' ', maxw);
                    mvwaddstr(win, y+1, 0, mojobox->textlines[selected+1]);
                    wattroff(win, COLOR_PAIR(MOJOCOLOR_TEXT));
                } // else
                wattron(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
                mvwhline(win, y, 0, ' ', maxw);
                mvwaddstr(win, y, 0, mojobox->textlines[selected]);
                wattroff(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
                wrefresh(win);
            } // if
        } // else if

        else if (ch == KEY_DOWN)
        {
            if (selected < (mojobox->textlinecount-1))
            {
                WINDOW *win = mojobox->textwin;
                int maxw, maxh;
                int y = ++selected - mojobox->textpos;
                getmaxyx(win, maxh, maxw);
                if (selected >= mojobox->textpos+maxh)
                {
                    upkeepBox(&mojobox, ch);  // upkeepBox does scrolling
                    y--;
                } // if
                else
                {
                    wattron(win, COLOR_PAIR(MOJOCOLOR_TEXT));
                    mvwhline(win, y-1, 0, ' ', maxw);
                    mvwaddstr(win, y-1, 0, mojobox->textlines[selected-1]);
                    wattroff(win, COLOR_PAIR(MOJOCOLOR_TEXT));
                } // else
                wattron(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
                mvwhline(win, y, 0, ' ', maxw);
                mvwaddstr(win, y, 0, mojobox->textlines[selected]);
                wattroff(win, COLOR_PAIR(MOJOCOLOR_BUTTONHOVER) | A_BOLD);
                wrefresh(win);
            } // if
        } // else if

        else if ((ch == KEY_NPAGE) || (ch == KEY_NPAGE))
        {
            // !!! FIXME: maybe handle this when I'm not so lazy.
            // !!! FIXME:  For now, this if statement is to block
            // !!! FIXME:  upkeepBox() from scrolling and screwing up state.
        } // else if

        else  // let upkeepBox handle other input (button selection, etc).
        {
            rc = upkeepBox(&mojobox, ch);
            if (rc == togglebutton)
            {
                int line = 0;
                rc = -1;  // reset so we don't stop processing input.
                if (toggle_option(NULL, opts, &line, selected+1) == 1)
                {
                    freeBox(mojobox, false);  // rebuild to reflect new options...
                    mojobox = NULL;
                } // if
            } // if
        } // else
    } while (rc == -1);

    freeBox(mojobox, true);

    for (i = 0; i < bcount; i++)
        free(buttons[i]);

    free(title);

    if (rc == backbutton)
        return -1;
    else if (rc == fwdbutton)
        return 1;

    return 0;  // error? Cancel?
} // MojoGui_ncurses_options


static char *MojoGui_ncurses_destination(const char **recommends, int recnum,
                                       int *command, boolean can_back,
                                       boolean can_fwd)
{
    // !!! FIXME: clearly this isn't right.
    if (recnum > 0)
    {
        *command = 1;
        return entry->xstrdup(recommends[0]);
    } // if

    *command = 0;
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
    const uint32 now = entry->ticks();
    boolean rebuild = (progressBox == NULL);
    int ch = 0;
    int rc = -1;

    if ( (lastComponent == NULL) ||
         (strcmp(lastComponent, component) != 0) ||
         (lastProgressType == NULL) ||
         (strcmp(lastProgressType, type) != 0) )
    {
        free(lastProgressType);
        free(lastComponent);
        lastProgressType = entry->xstrdup(type);
        lastComponent = entry->xstrdup(component);
        rebuild = true;
    } // if

    if (rebuild)
    {
        int w, h;
        char *text = NULL;
        char *localized_cancel = entry->xstrdup(entry->_("Cancel"));
        char *buttons[] = { localized_cancel };
        char *spacebuf = NULL;
        getmaxyx(stdscr, h, w);
        w -= 10;
        text = (char *) entry->xmalloc((w * 3) + 16);
        if (snprintf(text, w, "%s", component) > (w-4))
            strcpy((text+w)-4, "...");  // !!! FIXME: Unicode problem.
        strcat(text, "\n\n");
        spacebuf = (char *) entry->xmalloc(w+1);
        memset(spacebuf, ' ', w);  // xmalloc provides null termination.
        strcat(text, spacebuf);
        free(spacebuf);
        strcat(text, "\n\n ");

        freeBox(progressBox, false);
        progressBox = makeBox(type, text, buttons, 1, true, true);
        free(text);
        free(localized_cancel);
    } // if

    // limit update spam... will only write every one second, tops.
    if ((rebuild) || (percentTicks <= now))
    {
        static boolean unknownToggle = false;
        char *buf = NULL;
        WINDOW *win = progressBox->textwin;
        int w, h;
        getmaxyx(win, h, w);
        w -= 2;
        buf = (char *) entry->xmalloc(w+1);

        if (percent < 0)
        {
            const boolean origToggle = unknownToggle;
            int i;
            wmove(win, h-3, 1);
            for (i = 0; i < w; i++)
            {
                if (unknownToggle)
                    waddch(win, ' ' | COLOR_PAIR(MOJOCOLOR_TODO));
                else
                    waddch(win, ' ' | COLOR_PAIR(MOJOCOLOR_DONE));
                unknownToggle = !unknownToggle;
            } // for
            unknownToggle = !origToggle;  // animate by reversing next time.
        } // if
        else
        {
            int cells = (int) ( ((double) w) * (((double) percent) / 100.0) );
            snprintf(buf, w+1, "%d%%", percent);
            mvwaddstr(win, h-3, ((w+2) - strcells(buf)) / 2, buf);
            mvwchgat(win, h-3, 1, cells, A_BOLD, MOJOCOLOR_DONE, NULL);
            mvwchgat(win, h-3, 1+cells, w-cells, A_BOLD, MOJOCOLOR_TODO, NULL);
        } // else

        wtouchln(win, h-3, 1, 1);  // force reattributed cells to update.

        if (snprintf(buf, w+1, "%s", item) > (w-4))
            strcpy((buf+w)-4, "...");  // !!! FIXME: Unicode problem.
        mvwhline(win, h-2, 1, ' ', w);
        mvwaddstr(win, h-2, ((w+2) - strcells(buf)) / 2, buf);

        free(buf);
        wrefresh(win);

        percentTicks = now + 1000;
    } // if

    // !!! FIXME: check for input here for cancel button, resize, etc...
    ch = wgetch(progressBox->mainwin);
    if (ch == KEY_RESIZE)
    {
        freeBox(progressBox, false);
        progressBox = NULL;
    } // if
    else if (ch == ERR)  // can't distinguish between an error and a timeout!
    {
        // do nothing...
    } // else if
    else
    {
        rc = upkeepBox(&progressBox, ch);
    } // else

    return (rc == -1);
} // MojoGui_ncurses_progress


static void MojoGui_ncurses_final(const char *msg)
{
    MojoGui_ncurses_msgbox(entry->_("Finish"), msg);
} // MojoGui_ncurses_final

// end of gui_ncurses.c ...


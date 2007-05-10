
// Not only does GTK+ 2.x _look_ better than 1.x, it offers a lot of basic
//  functionality, like message boxes, that you'd expect to exist in a GUI
//  toolkit. In GTK+v1, you'd have to roll all that on your own!
//
// It's easier to implement in that regard, and produces a smaller DLL, but
//  it has a million dependencies, so you might need to use a GTK+v1 plugin,
//  too, in case they break backwards compatibility.

#if !SUPPORT_GUI_GTKPLUS2
#error Something is wrong in the build system.
#endif

#define BUILDING_EXTERNAL_PLUGIN 1
#include "gui.h"

MOJOGUI_PLUGIN(gtkplus2)

#if !GUI_STATIC_LINK_GTKPLUS2
CREATE_MOJOGUI_ENTRY_POINT(gtkplus2)
#endif

#include <gtk/gtk.h>

typedef enum
{
    PAGE_README,
    PAGE_OPTIONS,
    PAGE_DESTINATION,
} WizardPages;

static GtkWidget *gtkwindow = NULL;
static GtkWidget *pagetitle = NULL;
static GtkWidget *notebook = NULL;
static GtkWidget *readme = NULL;
static GtkWidget *cancel = NULL;
static GtkWidget *back = NULL;
static GtkWidget *next = NULL;
static GtkWidget *msgbox = NULL;

static volatile enum
{
    CLICK_BACK=-1,
    CLICK_CANCEL,
    CLICK_NEXT,
    CLICK_NONE
} click_value = CLICK_NONE;


static int run_wizard(const char *name, gint page,
                      boolean can_back, boolean can_fwd)
{
    int retval = 0;
    char *markup = g_markup_printf_escaped(
                        "<span size='large' weight='bold'>%s</span>",
                        name);

    gtk_label_set_markup(GTK_LABEL(pagetitle), markup);
    g_free (markup);

    gtk_widget_set_sensitive(back, can_back);
    gtk_widget_set_sensitive(next, can_fwd);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page);

    assert(click_value == CLICK_NONE);
    assert(gtkwindow != NULL);
    assert(msgbox == NULL);

    while (click_value == CLICK_NONE)
    {
        // signals fired under gtk_main_iteration can change click_value, too.
        gtk_main_iteration();

        if (click_value == CLICK_CANCEL)
        {
            // !!! FIXME: language.
            char *title = entry->xstrdup(entry->_("Stop installation"));
            char *text = entry->xstrdup(entry->_("Are you sure you want to stop installation?"));
            if (!MojoGui_gtkplus2_promptyn(title, text))
                click_value = CLICK_NONE;
            free(title);
            free(text);
        } // if
    } // while

    assert(click_value < CLICK_NONE);
    retval = (int) click_value;
    click_value = CLICK_NONE;
    return retval;
} // run_wizard


static gboolean signal_delete(GtkWidget *w, GdkEvent *evt, gpointer data)
{
    click_value = CLICK_CANCEL;
    return TRUE;  /* eat event: default handler destroys window! */
} // signal_delete


static void signal_clicked(GtkButton *_button, gpointer data)
{
    GtkWidget *button = GTK_WIDGET(_button);
    if (button == back)
        click_value = CLICK_BACK;
    else if (button == cancel)
        click_value = CLICK_CANCEL;
    else if (button == next)
        click_value = CLICK_NEXT;
    else
    {
        assert(0 && "Unknown click event.");
    } // else
} // signal_clicked


static uint8 MojoGui_gtkplus2_priority(void)
{
    // !!! FIXME: This would disallow gtkfb, gtk+/mac, gtk+windows, etc...
    if (getenv("DISPLAY") == NULL)
        return MOJOGUI_PRIORITY_NEVER_TRY;

    // !!! FIXME: I have no idea if this is a valid test.
    if (getenv("GNOME_DESKTOP_SESSION_ID") != NULL)
        return MOJOGUI_PRIORITY_TRY_FIRST;

    return MOJOGUI_PRIORITY_TRY_NORMAL;
} // MojoGui_gtkplus2_priority


static boolean MojoGui_gtkplus2_init(void)
{
    int tmpargc = 0;
    char *args[] = { NULL, NULL };
    char **tmpargv = args;
    return gtk_init_check(&tmpargc, &tmpargv) ? true : false;
} // MojoGui_gtkplus2_init


static void MojoGui_gtkplus2_deinit(void)
{
    // !!! FIXME: GTK+ doesn't have a deinit function...it installs a crappy
    // !!! FIXME:  atexit() function!
} // MojoGui_gtkplus2_deinit


static gint do_msgbox(const char *title, const char *text,
                      GtkMessageType mtype, GtkButtonsType btype)
{
    gint retval = 0;
    assert(msgbox == NULL);
    msgbox = gtk_message_dialog_new(GTK_WINDOW(gtkwindow), GTK_DIALOG_MODAL,
                                    mtype, btype, "%s", title);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(msgbox),
                                             "%s", text);
    retval = gtk_dialog_run(GTK_DIALOG(msgbox));
    gtk_widget_destroy(msgbox);
    msgbox = NULL;
    return retval;
} // do_msgbox


static void MojoGui_gtkplus2_msgbox(const char *title, const char *text)
{
    do_msgbox(title, text, GTK_MESSAGE_INFO, GTK_BUTTONS_OK);
} // MojoGui_gtkplus2_msgbox


static boolean MojoGui_gtkplus2_promptyn(const char *title, const char *text)
{
    gint rc = do_msgbox(title, text, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO);
    return (rc == GTK_RESPONSE_YES);
} // MojoGui_gtkplus2_promptyn


static GtkWidget *create_button(GtkWidget *box, const char *iconname,
                                const char *text)
{
    // !!! FIXME: gtk_button_set_use_stock()?
    GtkWidget *button = gtk_button_new();
    GtkWidget *alignment = gtk_alignment_new (0.5, 0.5, 0, 0);
    GtkWidget *hbox = gtk_hbox_new(FALSE, 2);
    GtkWidget *image = gtk_image_new_from_stock(iconname, GTK_ICON_SIZE_BUTTON);
    GtkWidget *label = gtk_label_new_with_mnemonic(text);
    gtk_widget_show(button);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
    gtk_widget_show(alignment);
    gtk_container_add(GTK_CONTAINER(button), alignment);
    gtk_widget_show(hbox);
    gtk_container_add(GTK_CONTAINER(alignment), hbox);
    gtk_widget_show(image);
    gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0);
    gtk_widget_show (label);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(signal_clicked), NULL);
    return button;
} // create_button


GtkWidget *create_gtkwindow(const char *title)
{
    GtkWidget *window;
    GtkWidget *widget;
    GtkWidget *box;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), title);

    box = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(box);
    gtk_container_add(GTK_CONTAINER(window), box);

    pagetitle = gtk_label_new("");
    gtk_widget_show(pagetitle);
    gtk_box_pack_start(GTK_BOX(box), pagetitle, FALSE, TRUE, 16);
    gtk_label_set_justify(GTK_LABEL(pagetitle), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(pagetitle), FALSE);

    widget = gtk_hseparator_new();
    gtk_widget_show(widget);
    gtk_box_pack_start(GTK_BOX(box), widget, FALSE, TRUE, 0);

    notebook = gtk_notebook_new();
    gtk_widget_show(notebook);
    gtk_box_pack_start(GTK_BOX(box), notebook, TRUE, TRUE, 0);
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
    gtk_widget_set_size_request(notebook,
                                (gint) (((float) gdk_screen_width()) * 0.3),
                                (gint) (((float) gdk_screen_height()) * 0.3));

#if 0
  empty_notebook_page = gtk_vbox_new(FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook), empty_notebook_page);

  label1 = gtk_label_new (_("intro"));
  gtk_widget_show (label1);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label1);
#endif

    widget = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(notebook), widget);

    readme = gtk_text_view_new();
    gtk_widget_show(readme);
    gtk_container_add(GTK_CONTAINER(widget), readme);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(readme), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(readme), GTK_WRAP_WORD);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(readme), FALSE);

#if 0
  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook), empty_notebook_page);

  label3 = gtk_label_new (_("options"));
  gtk_widget_show (label3);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 2), label3);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook), empty_notebook_page);

  label4 = gtk_label_new (_("destination"));
  gtk_widget_show (label4);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 3), label4);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook), empty_notebook_page);

  label5 = gtk_label_new (_("insertmedia"));
  gtk_widget_show (label5);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 4), label5);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook), empty_notebook_page);

  label6 = gtk_label_new (_("progress"));
  gtk_widget_show (label6);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 5), label6);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook), empty_notebook_page);

  label7 = gtk_label_new (_("status"));
  gtk_widget_show (label7);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 6), label7);
#endif

    widget = gtk_hseparator_new();
    gtk_widget_show(widget);
    gtk_box_pack_start(GTK_BOX(box), widget, FALSE, TRUE, 0);

    widget = gtk_hbox_new(TRUE, 0);
    gtk_widget_show(widget);
    gtk_box_pack_start(GTK_BOX(box), widget, FALSE, FALSE, 0);

    box = widget;
    cancel = create_button(box, "gtk-cancel", entry->_("Cancel"));
    back = create_button(box, "gtk-go-back", entry->_("Back"));
    next = create_button(box, "gtk-go-forward", entry->_("Next"));

    gtk_signal_connect(GTK_OBJECT(window), "delete-event",
                       GTK_SIGNAL_FUNC(signal_delete), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show(window);
    return window;
} // create_gtkwindow


static boolean MojoGui_gtkplus2_start(const char *title, const char *splash)
{
    gtkwindow = create_gtkwindow(title);
    return (gtkwindow != NULL);
} // MojoGui_gtkplus2_start


static void MojoGui_gtkplus2_stop(void)
{
    assert(msgbox == NULL);
    if (gtkwindow != NULL)
        gtk_widget_destroy(gtkwindow);

    gtkwindow = NULL;
    pagetitle = NULL;
    notebook = NULL;
    readme = NULL;
    cancel = NULL;
    back = NULL;
    next = NULL;
} // MojoGui_gtkplus2_stop


static int MojoGui_gtkplus2_readme(const char *name, const uint8 *data,
                                       size_t datalen, boolean can_back,
                                       boolean can_fwd)
{
    GtkTextBuffer *textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(readme));
    gtk_text_buffer_set_text(textbuf, (const gchar *) data, datalen);
    return run_wizard(name, PAGE_README, can_back, can_fwd);
} // MojoGui_gtkplus2_readme


static int MojoGui_gtkplus2_options(MojoGuiSetupOptions *opts,
                                   boolean can_back, boolean can_fwd)
{
    // !!! FIXME: better text.
    return run_wizard(entry->_("Options"), PAGE_OPTIONS, can_back, can_fwd);
} // MojoGui_gtkplus2_options


static char *MojoGui_gtkplus2_destination(const char **recommends, int reccount,
                                       boolean can_back, boolean can_fwd)
{
    // !!! FIXME: better text.
    run_wizard(entry->_("Destination"), PAGE_DESTINATION, can_back, can_fwd);
    return NULL;
} // MojoGui_gtkplus2_destination


static boolean MojoGui_gtkplus2_insertmedia(const char *medianame)
{
    // !!! FIXME: Use stock GTK icon for "media"?
    // !!! FIXME: better text.
    const char *title = entry->_("Media change");
    // !!! FIXME: better text.
    const char *fmt = entry->_("Please insert %s");
    size_t len = strlen(fmt) + strlen(medianame) + 1;
    char *text = (char *) entry->xmalloc(len);
    snprintf(text, len, fmt, medianame);
    gint rc = do_msgbox(title, text, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK_CANCEL);
    return (rc == GTK_RESPONSE_OK);
} // MojoGui_gtkplus2_insertmedia


static boolean MojoGui_gtkplus2_progress(const char *type, const char *component,
                                      int percent, const char *item)
{
    return true;
} // MojoGui_gtkplus2_progress

// end of gui_gtkplus2.c ...


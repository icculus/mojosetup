/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 *
 *  GTK4 GUI plugin - ported from gui_gtkplus3.c.
 */

#if !SUPPORT_GUI_GTK4
#error Something is wrong in the build system.
#endif

#define BUILDING_EXTERNAL_PLUGIN 1
#include "gui.h"

MOJOGUI_PLUGIN(gtk4)

#if !GUI_STATIC_LINK_GTK4
CREATE_MOJOGUI_ENTRY_POINT(gtk4)
#endif

#undef format
#include <gtk/gtk.h>
#define format entry->format

typedef enum
{
    PAGE_INTRO,
    PAGE_README,
    PAGE_OPTIONS,
    PAGE_DEST,
    PAGE_PRODUCTKEY,
    PAGE_PROGRESS,
    PAGE_FINAL
} WizardPages;

static WizardPages currentpage = PAGE_INTRO;
static gboolean canfwd = TRUE;
static GtkWidget *gtkwindow = NULL;
static GtkWidget *pagetitle = NULL;
static GtkWidget *notebook = NULL;
static GtkWidget *readme = NULL;
static GtkWidget *cancel = NULL;
static GtkWidget *back = NULL;
static GtkWidget *next = NULL;
static GtkWidget *finish = NULL;
static GtkWidget *msgbox = NULL;
static GtkWidget *productkey = NULL;
static GtkWidget *progressbar = NULL;
static GtkWidget *progresslabel = NULL;
static GtkWidget *finallabel = NULL;
static GtkWidget *browse = NULL;
static GtkWidget *splash = NULL;

// Destination widgets: GtkEntry + GtkDropDown backed by GtkStringList.
static GtkWidget *dest_entry = NULL;
static GtkWidget *dest_dropdown = NULL;
static GtkStringList *dest_model = NULL;

static volatile enum
{
    CLICK_BACK=-1,
    CLICK_CANCEL,
    CLICK_NEXT,
    CLICK_NONE
} click_value = CLICK_NONE;


static void prepare_wizard(const char *name, WizardPages page,
                           boolean can_back, boolean can_fwd,
                           boolean can_cancel, boolean fwd_at_start)
{
    char *markup = g_markup_printf_escaped(
                        "<span size='large' weight='bold'>%s</span>",
                        name);

    currentpage = page;
    canfwd = can_fwd;

    gtk_label_set_markup(GTK_LABEL(pagetitle), markup);
    g_free(markup);

    gtk_widget_set_sensitive(back, can_back);
    gtk_widget_set_sensitive(next, fwd_at_start);
    gtk_widget_set_sensitive(cancel, can_cancel);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), (gint) page);

    assert(click_value == CLICK_NONE);
    assert(gtkwindow != NULL);
    assert(msgbox == NULL);
} // prepare_wizard


static int wait_event(void)
{
    int retval = 0;

    // In GTK4, gtk_main_iteration() is gone; use g_main_context_iteration().
    g_main_context_iteration(g_main_context_default(), TRUE);
    if (click_value == CLICK_CANCEL)
    {
        if (!MojoGui_gtk4_promptyn(_("Cancel installation"), _("Are you sure you want to cancel installation?"), false))
            click_value = CLICK_NONE;
    } // if

    assert(click_value <= CLICK_NONE);
    retval = (int) click_value;
    click_value = CLICK_NONE;
    return retval;
} // wait_event


static int pump_events(void)
{
    int retval = (int) CLICK_NONE;
    while ( (retval == ((int) CLICK_NONE)) &&
            (g_main_context_pending(g_main_context_default())) )
        retval = wait_event();
    return retval;
} // pump_events


static int run_wizard(const char *name, WizardPages page,
                      boolean can_back, boolean can_fwd, boolean can_cancel,
                      boolean fwd_at_start)
{
    int retval = CLICK_NONE;
    prepare_wizard(name, page, can_back, can_fwd, can_cancel, fwd_at_start);
    while (retval == ((int) CLICK_NONE))
        retval = wait_event();

    assert(retval < ((int) CLICK_NONE));
    return retval;
} // run_wizard


static gboolean signal_close_request(GtkWindow *w, gpointer data)
{
    if (gtk_widget_get_visible(finish))
        click_value = CLICK_NEXT;
    else
        click_value = CLICK_CANCEL;
    return TRUE;  /* eat event: default handler destroys window! */
} // signal_close_request


static void signal_clicked(GtkButton *_button, gpointer data)
{
    GtkWidget *button = GTK_WIDGET(_button);
    if (button == back)
        click_value = CLICK_BACK;
    else if (button == cancel)
        click_value = CLICK_CANCEL;
    else if ((button == next) || (button == finish))
        click_value = CLICK_NEXT;
    else
    {
        assert(0 && "Unknown click event.");
    } // else
} // signal_clicked


/*
 * GTK4 replaces GtkFileChooserDialog with GtkFileDialog (available in 4.10+).
 * We use GtkFileDialog with a synchronous-like approach via a GMainLoop.
 */
static char *browse_folder_result = NULL;
static GMainLoop *browse_loop = NULL;

static void file_dialog_open_cb(GObject *source, GAsyncResult *result,
                                gpointer user_data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *folder = gtk_file_dialog_select_folder_finish(dialog, result, NULL);
    if (folder != NULL)
    {
        g_free(browse_folder_result);
        browse_folder_result = g_file_get_path(folder);
        g_object_unref(folder);
    } // if

    if (browse_loop != NULL)
        g_main_loop_quit(browse_loop);
} // file_dialog_open_cb


static void signal_browse_clicked(GtkButton *_button, gpointer data)
{
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, _("Destination"));
    gtk_file_dialog_set_modal(dialog, TRUE);

    // Set the initial folder from the current destination text.
    const gchar *cur_text = gtk_editable_get_text(GTK_EDITABLE(dest_entry));
    if (cur_text != NULL && *cur_text != '\0')
    {
        GFile *initial = g_file_new_for_path(cur_text);
        gtk_file_dialog_set_initial_folder(dialog, initial);
        g_object_unref(initial);
    } // if

    g_free(browse_folder_result);
    browse_folder_result = NULL;

    browse_loop = g_main_loop_new(NULL, FALSE);
    gtk_file_dialog_select_folder(dialog, GTK_WINDOW(gtkwindow), NULL,
                                  file_dialog_open_cb, NULL);
    g_main_loop_run(browse_loop);
    g_main_loop_unref(browse_loop);
    browse_loop = NULL;

    if (browse_folder_result != NULL)
    {
        gchar *utfname = g_filename_to_utf8(browse_folder_result, -1,
                                            NULL, NULL, NULL);
        if (utfname != NULL)
        {
            gtk_editable_set_text(GTK_EDITABLE(dest_entry), utfname);
            g_free(utfname);
        } // if
    } // if

    g_free(browse_folder_result);
    browse_folder_result = NULL;
    g_object_unref(dialog);
} // signal_browse_clicked


static void signal_dest_entry_changed(GtkEditable *editable, gpointer user_data)
{
    // Disable the forward button when the destination entry is blank.
    if ((currentpage == PAGE_DEST) && (canfwd))
    {
        const gchar *str = gtk_editable_get_text(editable);
        const gboolean filled_in = ((str != NULL) && (*str != '\0'));
        gtk_widget_set_sensitive(next, filled_in);
    } // if
} // signal_dest_entry_changed


// When the user picks from the dropdown, copy the selection into the entry.
static void signal_dest_dropdown_selected(GObject *obj, GParamSpec *pspec,
                                          gpointer user_data)
{
    guint selected = gtk_drop_down_get_selected(GTK_DROP_DOWN(dest_dropdown));
    if (selected != GTK_INVALID_LIST_POSITION && dest_model != NULL)
    {
        const char *str = gtk_string_list_get_string(dest_model, selected);
        if (str != NULL)
            gtk_editable_set_text(GTK_EDITABLE(dest_entry), str);
    } // if
} // signal_dest_dropdown_selected


static void signal_productkey_changed(GtkEditable *edit, gpointer user_data)
{
    // Disable the forward button when the entry is blank.
    if ((currentpage == PAGE_PRODUCTKEY) && (canfwd))
    {
        const char *fmt = (const char *) user_data;
        const char *key = gtk_editable_get_text(edit);
        const gboolean okay = isValidProductKey(fmt, key);
        gtk_widget_set_sensitive(next, okay);
    } // if
} // signal_productkey_changed


static uint8 MojoGui_gtk4_priority(boolean istty)
{
    // gnome-session exports this environment variable since 2002.
    if (getenv("GNOME_DESKTOP_SESSION_ID") != NULL)
        return MOJOGUI_PRIORITY_TRY_FIRST;
    else if (getenv("GNOME_SHELL_SESSION_MODE") != NULL)  // this is newer.
        return MOJOGUI_PRIORITY_TRY_FIRST;
    return MOJOGUI_PRIORITY_TRY_NORMAL;
} // MojoGui_gtk4_priority


static boolean MojoGui_gtk4_init(void)
{
    // In GTK4, gtk_init_check() takes no arguments.
    if (!gtk_init_check())
    {
        logInfo("gtk4: gtk_init_check() failed, use another UI.");
        return false;
    } // if
    return true;
} // MojoGui_gtk4_init


static void MojoGui_gtk4_deinit(void)
{
    // GTK4 doesn't have a deinit function either.
} // MojoGui_gtk4_deinit


/*
 * GTK4 removed GtkMessageDialog/gtk_dialog_run(). We build modal alert
 * dialogs using GtkAlertDialog (4.10+).
 */

static gint msgbox_response = GTK_RESPONSE_NONE;
static GMainLoop *msgbox_loop = NULL;

static void alert_dialog_response_cb(GObject *source, GAsyncResult *result,
                                     gpointer user_data)
{
    GtkAlertDialog *dialog = GTK_ALERT_DIALOG(source);
    int button = gtk_alert_dialog_choose_finish(dialog, result, NULL);
    gint *resp_map = (gint *) user_data;
    if (button >= 0 && resp_map != NULL)
        msgbox_response = resp_map[button];
    else
        msgbox_response = GTK_RESPONSE_CLOSE;

    if (msgbox_loop != NULL)
        g_main_loop_quit(msgbox_loop);
} // alert_dialog_response_cb


/**
 * Synchronous modal dialog using GtkAlertDialog + GMainLoop.
 * @param buttons  Array of button labels (NULL-terminated).
 * @param resp_map Array of response ints parallel to buttons.
 * @param n_buttons Number of buttons.
 * @param default_button Index of the default button, or -1 for none.
 */
static gint do_msgbox(const char *title, const char *text,
                      const char **buttons, const gint *resp_map,
                      int n_buttons, int default_button)
{
    gint retval = GTK_RESPONSE_NONE;

    assert(msgbox == NULL);

    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title);
    gtk_alert_dialog_set_detail(dialog, text);
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_modal(dialog, TRUE);
    if (default_button >= 0)
        gtk_alert_dialog_set_default_button(dialog, default_button);
    gtk_alert_dialog_set_cancel_button(dialog, n_buttons - 1);

    // We use a temporary GMainLoop to get synchronous behavior.
    msgbox_response = GTK_RESPONSE_NONE;
    msgbox_loop = g_main_loop_new(NULL, FALSE);

    // Mark that a msgbox is active (using a non-NULL sentinel).
    msgbox = (GtkWidget *) dialog;

    gtk_alert_dialog_choose(dialog,
                            gtkwindow ? GTK_WINDOW(gtkwindow) : NULL,
                            NULL,
                            alert_dialog_response_cb,
                            (gpointer) resp_map);

    g_main_loop_run(msgbox_loop);
    g_main_loop_unref(msgbox_loop);
    msgbox_loop = NULL;

    retval = msgbox_response;

    g_object_unref(dialog);
    msgbox = NULL;

    return retval;
} // do_msgbox


static void MojoGui_gtk4_msgbox(const char *title, const char *text)
{
    static const char *buttons[] = { NULL, NULL };
    static const gint resp[] = { GTK_RESPONSE_CLOSE };
    buttons[0] = _("Close");
    do_msgbox(title, text, buttons, resp, 1, 0);
} // MojoGui_gtk4_msgbox


static boolean MojoGui_gtk4_promptyn(const char *title, const char *text,
                                     boolean defval)
{
    const char *buttons[] = { NULL, NULL, NULL };
    static const gint resp[] = { GTK_RESPONSE_YES, GTK_RESPONSE_NO };
    buttons[0] = _("Yes");
    buttons[1] = _("No");
    gint rc = do_msgbox(title, text, buttons, resp, 2,
                        defval ? 0 : 1);
    return (rc == GTK_RESPONSE_YES);
} // MojoGui_gtk4_promptyn


static MojoGuiYNAN MojoGui_gtk4_promptynan(const char *title,
                                            const char *text,
                                            boolean defval)
{
    const char *buttons[] = { NULL, NULL, NULL, NULL, NULL };
    static const gint resp[] = { GTK_RESPONSE_YES, GTK_RESPONSE_NO, 1, 0 };
    buttons[0] = _("Yes");
    buttons[1] = _("No");
    buttons[2] = _("_Always");
    buttons[3] = _("N_ever");
    const gint rc = do_msgbox(title, text, buttons, resp, 4,
                              defval ? 0 : 1);
    switch (rc)
    {
        case GTK_RESPONSE_YES: return MOJOGUI_YES;
        case GTK_RESPONSE_NO: return MOJOGUI_NO;
        case 1: return MOJOGUI_ALWAYS;
        case 0: return MOJOGUI_NEVER;
    } // switch

    assert(false && "BUG: unhandled case in switch statement");
    return MOJOGUI_NO;  // just in case.
} // MojoGui_gtk4_promptynan


static GtkWidget *create_button(GtkWidget *box, const char *iconname,
                                const char *text,
                                void (*signal_callback)
                                    (GtkButton *button, gpointer data))
{
    GtkWidget *button = gtk_button_new_with_mnemonic(text);
    gtk_button_set_icon_name(GTK_BUTTON(button), iconname);
    gtk_box_append(GTK_BOX(box), button);
    g_signal_connect(button, "clicked", G_CALLBACK(signal_callback), NULL);
    return button;
} // create_button


static GtkWidget *build_splash(const MojoGuiSplash *splash)
{
    GtkWidget *retval = NULL;
    GdkTexture *texture = NULL;
    GBytes *bytes = NULL;
    const uint32 splashlen = splash->w * splash->h * 4;

    if (splash->position == MOJOGUI_SPLASH_NONE)
        return NULL;

    if ((splash->rgba == NULL) || (splashlen == 0))
        return NULL;

    // GTK4: Use GdkTexture via GdkMemoryTexture instead of GdkPixbuf.
    bytes = g_bytes_new(splash->rgba, splashlen);
    texture = gdk_memory_texture_new(splash->w, splash->h,
                                     GDK_MEMORY_R8G8B8A8,
                                     bytes, splash->w * 4);
    g_bytes_unref(bytes);

    if (texture != NULL)
    {
        retval = gtk_picture_new_for_paintable(GDK_PAINTABLE(texture));
        gtk_picture_set_can_shrink(GTK_PICTURE(retval), FALSE);
        g_object_unref(texture);
    } // if

    return retval;
} // build_splash


static GtkWidget *create_gtkwindow(const char *title,
                                   const MojoGuiSplash *_splash)
{
    GtkWidget *splashbox = NULL;
    GtkWidget *window;
    GtkWidget *widget;
    GtkWidget *box;
    GtkWidget *hbox;

    currentpage = PAGE_INTRO;
    canfwd = TRUE;

    window = gtk_window_new();
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 550);

    gtk_window_set_icon_name(GTK_WINDOW(window), "system-software-installer");

    assert(splash == NULL);
    splash = build_splash(_splash);
    if (splash != NULL)
    {
        const MojoGuiSplashPos pos = _splash->position;
        if ((pos == MOJOGUI_SPLASH_LEFT) || (pos == MOJOGUI_SPLASH_RIGHT))
        {
            splashbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
            gtk_window_set_child(GTK_WINDOW(window), splashbox);
            if (pos == MOJOGUI_SPLASH_LEFT)
                gtk_box_prepend(GTK_BOX(splashbox), splash);
        } // if
        else if ((pos == MOJOGUI_SPLASH_TOP) || (pos == MOJOGUI_SPLASH_BOTTOM))
        {
            splashbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
            gtk_window_set_child(GTK_WINDOW(window), splashbox);
            if (pos == MOJOGUI_SPLASH_TOP)
                gtk_box_prepend(GTK_BOX(splashbox), splash);
        } // else if
    } // if

    if (splashbox == NULL)
        splashbox = window;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 8);
    gtk_widget_set_margin_bottom(box, 8);

    if (splashbox == window)
        gtk_window_set_child(GTK_WINDOW(window), box);
    else
        gtk_box_append(GTK_BOX(splashbox), box);

    // If splash goes on right/bottom, append it now (after main content).
    if (splash != NULL && splashbox != window)
    {
        const MojoGuiSplashPos pos = _splash->position;
        if (pos == MOJOGUI_SPLASH_RIGHT || pos == MOJOGUI_SPLASH_BOTTOM)
            gtk_box_append(GTK_BOX(splashbox), splash);
    } // if

    pagetitle = gtk_label_new("");
    gtk_widget_set_margin_top(pagetitle, 16);
    gtk_widget_set_margin_bottom(pagetitle, 16);
    gtk_box_append(GTK_BOX(box), pagetitle);

    notebook = gtk_notebook_new();
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
    gtk_widget_set_vexpand(notebook, TRUE);
    gtk_widget_set_hexpand(notebook, TRUE);
    gtk_box_append(GTK_BOX(box), notebook);

    widget = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_halign(widget, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(box), widget);

    box = widget;
    cancel = create_button(box, "process-stop", _("Cancel"), signal_clicked);
    back = create_button(box, "go-previous", _("Back"), signal_clicked);
    next = create_button(box, "go-next", _("Next"), signal_clicked);
    finish = create_button(box, "go-last", _("Finish"), signal_clicked);
    gtk_widget_set_visible(finish, FALSE);

    // Intro page (PAGE_INTRO).
    widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), widget, NULL);

    // README/EULA page (PAGE_README).
    widget = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(widget),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), widget, NULL);

    readme = gtk_text_view_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(widget), readme);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(readme), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(readme), GTK_WRAP_NONE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(readme), FALSE);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(readme), 4);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(readme), 4);

    // Options page (PAGE_OPTIONS).
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, NULL);

    // Destination page (PAGE_DEST).
    // Uses GtkEntry + GtkDropDown + GtkStringList (modern GTK4 replacement
    // for the deprecated GtkComboBoxText with entry).
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    widget = gtk_label_new(_("Folder:"));
    gtk_box_append(GTK_BOX(hbox), widget);

    dest_entry = gtk_entry_new();
    gtk_widget_set_hexpand(dest_entry, TRUE);
    g_signal_connect(dest_entry, "changed",
                     G_CALLBACK(signal_dest_entry_changed), NULL);
    gtk_box_append(GTK_BOX(hbox), dest_entry);

    browse = create_button(hbox, "document-open",
                           _("B_rowse..."), signal_browse_clicked);
    gtk_box_append(GTK_BOX(box), hbox);

    // Dropdown for recommended destinations (shown below the entry).
    dest_model = gtk_string_list_new(NULL);
    dest_dropdown = gtk_drop_down_new(G_LIST_MODEL(dest_model), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dest_dropdown),
                               GTK_INVALID_LIST_POSITION);
    g_signal_connect(dest_dropdown, "notify::selected",
                     G_CALLBACK(signal_dest_dropdown_selected), NULL);
    gtk_box_append(GTK_BOX(box), dest_dropdown);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, NULL);

    // Product key page (PAGE_PRODUCTKEY).
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    widget = gtk_label_new(_("Please enter your product key"));
    gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_CENTER);
    gtk_label_set_wrap(GTK_LABEL(widget), TRUE);
    gtk_box_append(GTK_BOX(box), widget);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    productkey = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(productkey), TRUE);
    gtk_entry_set_visibility(GTK_ENTRY(productkey), TRUE);
    gtk_box_append(GTK_BOX(hbox), productkey);
    gtk_box_append(GTK_BOX(box), hbox);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, NULL);

    // Progress page (PAGE_PROGRESS).
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    progressbar = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progressbar), TRUE);
    gtk_widget_set_halign(GTK_WIDGET(progressbar), GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(progressbar, TRUE);
    gtk_box_append(GTK_BOX(box), progressbar);
    progresslabel = gtk_label_new("");
    gtk_box_append(GTK_BOX(box), progresslabel);
    gtk_label_set_justify(GTK_LABEL(progresslabel), GTK_JUSTIFY_LEFT);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, NULL);

    // Final page (PAGE_FINAL).
    widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), widget, NULL);
    finallabel = gtk_label_new("");
    gtk_box_append(GTK_BOX(widget), finallabel);
    gtk_label_set_justify(GTK_LABEL(finallabel), GTK_JUSTIFY_LEFT);
    gtk_label_set_wrap(GTK_LABEL(finallabel), TRUE);

    g_signal_connect(window, "close-request", G_CALLBACK(signal_close_request), NULL);

    gtk_widget_set_visible(window, TRUE);

    gtk_widget_grab_focus(next);

    return window;
} // create_gtkwindow


static boolean MojoGui_gtk4_start(const char *title,
                                  const MojoGuiSplash *splash)
{
    gtkwindow = create_gtkwindow(title, splash);
    return (gtkwindow != NULL);
} // MojoGui_gtk4_start


static void MojoGui_gtk4_stop(void)
{
    assert(msgbox == NULL);
    if (gtkwindow != NULL)
        gtk_window_destroy(GTK_WINDOW(gtkwindow));

    gtkwindow = NULL;
    pagetitle = NULL;
    finallabel = NULL;
    progresslabel = NULL;
    progressbar = NULL;
    dest_entry = NULL;
    dest_dropdown = NULL;
    dest_model = NULL;
    productkey = NULL;
    notebook = NULL;
    readme = NULL;
    cancel = NULL;
    back = NULL;
    next = NULL;
    finish = NULL;
    splash = NULL;
} // MojoGui_gtk4_stop


static int MojoGui_gtk4_readme(const char *name, const uint8 *data,
                               size_t datalen, boolean can_back,
                               boolean can_fwd)
{
    GtkTextBuffer *textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(readme));
    gtk_text_buffer_set_text(textbuf, (const gchar *) data, datalen);
    return run_wizard(name, PAGE_README, can_back, can_fwd, true, can_fwd);
} // MojoGui_gtk4_readme


static void set_option_tree_sensitivity(MojoGuiSetupOptions *opts, boolean val)
{
    if (opts != NULL)
    {
        gtk_widget_set_sensitive((GtkWidget *) opts->guiopaque, val);
        set_option_tree_sensitivity(opts->next_sibling, val);
        set_option_tree_sensitivity(opts->child, val && opts->value);
    } // if
} // set_option_tree_sensitivity


// GTK4: GtkCheckButton "toggled" signal handler.
static void signal_check_toggled(GtkCheckButton *check, gpointer data)
{
    MojoGuiSetupOptions *opts = (MojoGuiSetupOptions *) data;
    const boolean enabled = gtk_check_button_get_active(check);
    opts->value = enabled;
    set_option_tree_sensitivity(opts->child, enabled);
} // signal_check_toggled


static GtkWidget *new_option_level(GtkWidget *box)
{
    GtkWidget *retval = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(retval, GTK_ALIGN_START);
    gtk_widget_set_valign(retval, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(retval, 15);
    gtk_box_append(GTK_BOX(box), retval);
    return retval;
} // new_option_level


static void build_options(MojoGuiSetupOptions *opts, GtkWidget *box,
                          gboolean sensitive)
{
    if (opts != NULL)
    {
        GtkWidget *widget = NULL;

        if (opts->is_group_parent)
        {
            MojoGuiSetupOptions *kids = opts->child;
            GtkWidget *childbox = NULL;
            widget = gtk_label_new(opts->description);
            gtk_widget_set_halign(widget, GTK_ALIGN_START);
            gtk_widget_set_valign(widget, GTK_ALIGN_CENTER);
            opts->guiopaque = widget;
            gtk_widget_set_sensitive(widget, sensitive);
            gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_LEFT);
            gtk_box_append(GTK_BOX(box), widget);

            if (opts->tooltip != NULL)
                gtk_widget_set_tooltip_text(widget, opts->tooltip);

            widget = NULL;
            childbox = new_option_level(box);
            while (kids)
            {
                // GTK4: GtkCheckButton replaces GtkRadioButton.
                // Use gtk_check_button_set_group() for radio-button behavior.
                GtkWidget *radio = gtk_check_button_new_with_label(kids->description);
                if (widget != NULL)
                    gtk_check_button_set_group(GTK_CHECK_BUTTON(radio),
                                               GTK_CHECK_BUTTON(widget));
                kids->guiopaque = radio;
                gtk_check_button_set_active(GTK_CHECK_BUTTON(radio),
                                            kids->value);
                gtk_widget_set_sensitive(radio, sensitive);
                gtk_box_append(GTK_BOX(childbox), radio);
                g_signal_connect(radio, "toggled",
                                 G_CALLBACK(signal_check_toggled), kids);

                if (kids->tooltip != NULL)
                    gtk_widget_set_tooltip_text(radio, kids->tooltip);

                if (kids->child != NULL)
                {
                    build_options(kids->child, new_option_level(childbox),
                                  sensitive);
                } // if

                widget = radio;
                kids = kids->next_sibling;
            } // while
        } // if

        else
        {
            // GTK4: GtkCheckButton is standalone (not derived from GtkToggleButton).
            widget = gtk_check_button_new_with_label(opts->description);
            opts->guiopaque = widget;
            gtk_check_button_set_active(GTK_CHECK_BUTTON(widget),
                                        opts->value);
            gtk_widget_set_sensitive(widget, sensitive);
            gtk_box_append(GTK_BOX(box), widget);
            g_signal_connect(widget, "toggled",
                             G_CALLBACK(signal_check_toggled), opts);

            if (opts->tooltip != NULL)
                gtk_widget_set_tooltip_text(widget, opts->tooltip);

            if (opts->child != NULL)
            {
                build_options(opts->child, new_option_level(box),
                                ((sensitive) && (opts->value)) );
            } // if
        } // else

        build_options(opts->next_sibling, box, sensitive);
    } // if
} // build_options


static int MojoGui_gtk4_options(MojoGuiSetupOptions *opts,
                                boolean can_back, boolean can_fwd)
{
    int retval;
    GtkWidget *box;
    GtkWidget *page;  // this is a vbox.

    page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), PAGE_OPTIONS);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(page), box);

    build_options(opts, box, TRUE);
    retval = run_wizard(_("Options"), PAGE_OPTIONS,
                        can_back, can_fwd, true, can_fwd);

    // GTK4: remove the options box from the page.
    gtk_box_remove(GTK_BOX(page), box);
    return retval;
} // MojoGui_gtk4_options


static char *MojoGui_gtk4_destination(const char **recommends, int recnum,
                                      int *command, boolean can_back,
                                      boolean can_fwd)
{
    const boolean fwd_at_start = ( (recnum > 0) && (*(recommends[0])) );
    char *retval = NULL;
    int i;
    guint n;

    // Populate the string list model with recommended destinations.
    for (i = 0; i < recnum; i++)
        gtk_string_list_append(dest_model, recommends[i]);

    // Select the first item and copy it into the entry.
    if (recnum > 0)
    {
        gtk_drop_down_set_selected(GTK_DROP_DOWN(dest_dropdown), 0);
        gtk_editable_set_text(GTK_EDITABLE(dest_entry), recommends[0]);
    } // if

    *command = run_wizard(_("Destination"), PAGE_DEST,
                          can_back, can_fwd, true, fwd_at_start);

    const gchar *str = gtk_editable_get_text(GTK_EDITABLE(dest_entry));

    // shouldn't ever be empty ("next" should be disabled in that case).
    assert( (*command <= 0) || ((str != NULL) && (*str != '\0')) );

    retval = xstrdup(str);

    // Clear all items from the model.
    n = g_list_model_get_n_items(G_LIST_MODEL(dest_model));
    if (n > 0)
        gtk_string_list_splice(dest_model, 0, n, NULL);

    gtk_drop_down_set_selected(GTK_DROP_DOWN(dest_dropdown),
                               GTK_INVALID_LIST_POSITION);

    return retval;
} // MojoGui_gtk4_destination


static int MojoGui_gtk4_productkey(const char *desc, const char *fmt,
                                   char *buf, const int buflen,
                                   boolean can_back, boolean can_fwd)
{
    int retval = 0;
    const boolean fwd_at_start = isValidProductKey(fmt, buf);

    gtk_entry_set_max_length(GTK_ENTRY(productkey), buflen - 1);
    gtk_editable_set_width_chars(GTK_EDITABLE(productkey), buflen - 1);
    gtk_editable_set_text(GTK_EDITABLE(productkey), (const gchar *) buf);

    const guint connid = g_signal_connect(productkey, "changed",
                                    G_CALLBACK(signal_productkey_changed),
                                    (void *) fmt);
    retval = run_wizard(desc, PAGE_PRODUCTKEY,
                        can_back, can_fwd, true, fwd_at_start);
    g_signal_handler_disconnect(productkey, connid);

    const gchar *str = gtk_editable_get_text(GTK_EDITABLE(productkey));
    // should never be invalid ("next" should be disabled in that case).
    assert( (retval <= 0) || ((str) && (isValidProductKey(fmt, str))) );
    assert(strlen(str) < buflen);
    strcpy(buf, str);
    gtk_editable_set_text(GTK_EDITABLE(productkey), "");

    return retval;
} // MojoGui_gtk4_productkey


static boolean MojoGui_gtk4_insertmedia(const char *medianame)
{
    gint rc = 0;
    const char *text = format(_("Please insert '%0'"), medianame);
    const char *buttons[] = { NULL, NULL, NULL };
    static const gint resp[] = { GTK_RESPONSE_OK, GTK_RESPONSE_CANCEL };
    buttons[0] = _("OK");
    buttons[1] = _("Cancel");
    rc = do_msgbox(_("Media change"), text, buttons, resp, 2, 0);
    free((void *) text);
    return (rc == GTK_RESPONSE_OK);
} // MojoGui_gtk4_insertmedia


static void MojoGui_gtk4_progressitem(void)
{
    // no-op in this UI target.
} // MojoGui_gtk4_progressitem


static boolean MojoGui_gtk4_progress(const char *type, const char *component,
                                     int percent, const char *item,
                                     boolean can_cancel)
{
    static uint32 lastTicks = 0;
    const uint32 curTicks = ticks();
    int rc;

    GtkProgressBar *progress = GTK_PROGRESS_BAR(progressbar);
    const char *ptext = gtk_progress_bar_get_text(progress);
    // only update once in a while or if the component changed...
    if ((curTicks - lastTicks) > 200 || ptext == NULL || strcmp(ptext, component) != 0)
    {
        if (percent < 0)
            gtk_progress_bar_pulse(progress);
        else
            gtk_progress_bar_set_fraction(progress, ((gdouble) percent) / 100.0);

        gtk_progress_bar_set_text(progress, component);
        gtk_label_set_text(GTK_LABEL(progresslabel), item);
        lastTicks = curTicks;
    } // if

    prepare_wizard(type, PAGE_PROGRESS, false, false, can_cancel, false);
    rc = pump_events();
    assert( (rc == ((int) CLICK_CANCEL)) || (rc == ((int) CLICK_NONE)) );
    return (rc != CLICK_CANCEL);
} // MojoGui_gtk4_progress


static void MojoGui_gtk4_pump(void)
{
    while (g_main_context_pending(g_main_context_default()))
        g_main_context_iteration(g_main_context_default(), FALSE);
} // MojoGui_gtk4_pump


static void MojoGui_gtk4_final(const char *msg)
{
    gtk_widget_set_visible(next, FALSE);
    gtk_widget_set_visible(finish, TRUE);
    gtk_label_set_text(GTK_LABEL(finallabel), msg);
    run_wizard(_("Finish"), PAGE_FINAL, false, true, false, true);
} // MojoGui_gtk4_final

// end of gui_gtk4.c ...

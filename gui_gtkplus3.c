/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#if !SUPPORT_GUI_GTKPLUS3
#error Something is wrong in the build system.
#endif

#define BUILDING_EXTERNAL_PLUGIN 1
#include "gui.h"

MOJOGUI_PLUGIN(gtkplus3)

#if !GUI_STATIC_LINK_GTKPLUS3
CREATE_MOJOGUI_ENTRY_POINT(gtkplus3)
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
static GtkWidget *destination = NULL;
static GtkWidget *productkey = NULL;
static GtkWidget *progressbar = NULL;
static GtkWidget *progresslabel = NULL;
static GtkWidget *finallabel = NULL;
static GtkWidget *browse = NULL;
static GtkWidget *splash = NULL;

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

    // signals fired under gtk_main_iteration can change click_value.
    gtk_main_iteration();
    if (click_value == CLICK_CANCEL)
    {
        if (!MojoGui_gtkplus3_promptyn(_("Cancel installation"), _("Are you sure you want to cancel installation?"), false))
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
    while ( (retval == ((int) CLICK_NONE)) && (gtk_events_pending()) )
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
    else if ((button == next) || (button == finish))
        click_value = CLICK_NEXT;
    else
    {
        assert(0 && "Unknown click event.");
    } // else
} // signal_clicked


static void signal_browse_clicked(GtkButton *_button, gpointer data)
{
    GtkWidget *dialog = gtk_file_chooser_dialog_new (
        _("Destination"),
        GTK_WINDOW(gtkwindow),
        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
        _("Cancel"),GTK_RESPONSE_CANCEL,
        _("OK"), GTK_RESPONSE_ACCEPT,
        NULL);

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
            gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(destination)));

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename;
        gchar *utfname;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        utfname = g_filename_to_utf8(filename, -1, NULL, NULL, NULL);
        gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(destination), utfname);
        gtk_combo_box_set_active(GTK_COMBO_BOX(destination), 0);

        g_free(utfname);
        g_free(filename);
    }
    
    // !!! FIXME: should append the package name to the directory they chose?
    // !!! FIXME:  This is annoying, they might have created the folder
    // !!! FIXME:  themselves in the dialog.
    // !!! FIXME: Could warn when the target directory already contains files?

    gtk_widget_destroy(dialog);
} // signal_browse_clicked


static void signal_dest_changed(GtkComboBoxText *combo, gpointer user_data)
{
    // Disable the forward button when the destination entry is blank.
    if ((currentpage == PAGE_DEST) && (canfwd))
    {
        gchar *str = gtk_combo_box_text_get_active_text(combo);
        const gboolean filled_in = ((str != NULL) && (*str != '\0'));
        g_free(str);
        gtk_widget_set_sensitive(next, filled_in);
    } // if
} // signal_dest_changed


static void signal_productkey_changed(GtkEditable *edit, gpointer user_data)
{
    // Disable the forward button when the entry is blank.
    if ((currentpage == PAGE_PRODUCTKEY) && (canfwd))
    {
        const char *fmt = (const char *) user_data;
        char *key = (char *) gtk_editable_get_chars(edit, 0, -1);
        const gboolean okay = isValidProductKey(fmt, key);
        g_free(key);
        gtk_widget_set_sensitive(next, okay);
    } // if
} // signal_productkey_changed


static uint8 MojoGui_gtkplus3_priority(boolean istty)
{
    // gnome-session exports this environment variable since 2002.
    if (getenv("GNOME_DESKTOP_SESSION_ID") != NULL)
        return MOJOGUI_PRIORITY_TRY_FIRST;
    else if (getenv("GNOME_SHELL_SESSION_MODE") != NULL)  // this is newer.
        return MOJOGUI_PRIORITY_TRY_FIRST;
    return MOJOGUI_PRIORITY_TRY_NORMAL;
} // MojoGui_gtkplus3_priority


static boolean MojoGui_gtkplus3_init(void)
{
    int tmpargc = 0;
    char *args[] = { NULL, NULL };
    char **tmpargv = args;
    if (!gtk_init_check(&tmpargc, &tmpargv))
    {
        logInfo("gtkplus3: gtk_init_check() failed, use another UI.");
        return false;
    } // if
    return true;
} // MojoGui_gtkplus3_init


static void MojoGui_gtkplus3_deinit(void)
{
    // !!! FIXME: GTK+ doesn't have a deinit function...it installs a crappy
    // !!! FIXME:  atexit() function!
} // MojoGui_gtkplus3_deinit

/**
 * 
 * @param defbutton The response ID to use when enter is pressed, or 0
 * to leave it up to GTK+.
 */
static gint do_msgbox(const char *title, const char *text,
                      GtkMessageType mtype, GtkButtonsType btype,
                      GtkResponseType defbutton,
                      void (*addButtonCallback)(GtkWidget *_msgbox))
{
    gint retval = 0;
    
    // Modal dialog, this will never be called recursively.
    assert(msgbox == NULL);

    msgbox = gtk_message_dialog_new(GTK_WINDOW(gtkwindow), GTK_DIALOG_MODAL,
                                    mtype, btype, "%s", title);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(msgbox),
                                             "%s", text);

    if (defbutton)
        gtk_dialog_set_default_response(GTK_DIALOG(msgbox), defbutton);

    if (addButtonCallback != NULL)
        addButtonCallback(msgbox);

    retval = gtk_dialog_run(GTK_DIALOG(msgbox));
    gtk_widget_destroy(msgbox);
    msgbox = NULL;

    return retval;
} // do_msgbox


static void MojoGui_gtkplus3_msgbox(const char *title, const char *text)
{
    do_msgbox(title, text, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, 0, NULL);
} // MojoGui_gtkplus3_msgbox


static boolean MojoGui_gtkplus3_promptyn(const char *title, const char *text,
                                         boolean defval)
{
    gint rc = do_msgbox(title, text, GTK_MESSAGE_QUESTION,
                        GTK_BUTTONS_YES_NO,
                        defval ? GTK_RESPONSE_YES : GTK_RESPONSE_NO,
                        NULL);

    return (rc == GTK_RESPONSE_YES);
} // MojoGui_gtkplus3_promptyn


static void promptynanButtonCallback(GtkWidget *_msgbox)
{
    gtk_dialog_add_buttons(GTK_DIALOG(_msgbox),
                           _("Yes"), GTK_RESPONSE_YES,
                           _("No"), GTK_RESPONSE_NO,
                           _("_Always"), 1,
                           _("N_ever"), 0,
                           NULL);
} // promptynanButtonCallback


static MojoGuiYNAN MojoGui_gtkplus3_promptynan(const char *title,
                                               const char *text,
                                               boolean defval)
{
    const gint rc = do_msgbox(title, text, GTK_MESSAGE_QUESTION,
                              GTK_BUTTONS_NONE,
                              defval ? GTK_RESPONSE_YES : GTK_RESPONSE_NO,
                              promptynanButtonCallback);
    switch (rc)
    {
        case GTK_RESPONSE_YES: return MOJOGUI_YES;
        case GTK_RESPONSE_NO: return MOJOGUI_NO;
        case 1: return MOJOGUI_ALWAYS;
        case 0: return MOJOGUI_NEVER;
    } // switch

    assert(false && "BUG: unhandled case in switch statement");
    return MOJOGUI_NO;  // just in case.
} // MojoGui_gtkplus3_promptynan


static GtkWidget *create_button(GtkWidget *box, const char *iconname,
                                const char *text,
                                void (*signal_callback)
                                    (GtkButton *button, gpointer data))
{
    GtkWidget *button = gtk_button_new_with_mnemonic(text);
    GtkWidget *image = gtk_image_new_from_icon_name(iconname, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image (GTK_BUTTON(button), image);
    gtk_widget_show(button);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 6);
    g_signal_connect(button, "clicked", G_CALLBACK(signal_callback), NULL);
    return button;
} // create_button


static void free_splash(guchar *pixels, gpointer data)
{
    free(pixels);
} // free_splash


static GtkWidget *build_splash(const MojoGuiSplash *splash)
{
    GtkWidget *retval = NULL;
    GdkPixbuf *pixbuf = NULL;
    guchar *rgba = NULL;
    const uint32 splashlen = splash->w * splash->h * 4;

    if (splash->position == MOJOGUI_SPLASH_NONE)
        return NULL;

    if ((splash->rgba == NULL) || (splashlen == 0))
        return NULL;

    rgba = (guchar *) xmalloc(splashlen);
    memcpy(rgba, splash->rgba, splashlen);
    pixbuf = gdk_pixbuf_new_from_data(rgba, GDK_COLORSPACE_RGB, TRUE, 8,
                                      splash->w, splash->h, splash->w * 4,
                                      free_splash, NULL);
    if (pixbuf == NULL)
        free(rgba);
    else
    {
        retval = gtk_image_new_from_pixbuf(pixbuf);
        g_object_unref(pixbuf);  // retval adds a ref to pixbuf, so lose our's.
        if (retval != NULL)
            gtk_widget_show(retval);
    } // else

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

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 550);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    GdkPixbuf *icon = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),
                                                "system-software-installer",
                                                48, 0, NULL);
    if (icon)
        gtk_window_set_icon(GTK_WINDOW(window), icon);

    assert(splash == NULL);
    splash = build_splash(_splash);
    if (splash != NULL)
    {
        // !!! FIXME: MOJOGUI_SPLASH_BACKGROUND?
        const MojoGuiSplashPos pos = _splash->position;
        if ((pos == MOJOGUI_SPLASH_LEFT) || (pos == MOJOGUI_SPLASH_RIGHT))
        {
            splashbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
            gtk_widget_show(splashbox);
            gtk_container_add(GTK_CONTAINER(window), splashbox);
            if (pos == MOJOGUI_SPLASH_LEFT)
                gtk_box_pack_start(GTK_BOX(splashbox), splash, FALSE, FALSE, 6);
            else
                gtk_box_pack_end(GTK_BOX(splashbox), splash, FALSE, FALSE, 6);
        } // if

        else if ((pos == MOJOGUI_SPLASH_TOP) || (pos == MOJOGUI_SPLASH_BOTTOM))
        {
            splashbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
            gtk_widget_show(splashbox);
            gtk_container_add(GTK_CONTAINER(window), splashbox);
            if (pos == MOJOGUI_SPLASH_TOP)
                gtk_box_pack_start(GTK_BOX(splashbox), splash, FALSE, FALSE, 6);
            else
                gtk_box_pack_end(GTK_BOX(splashbox), splash, FALSE, FALSE, 6);
        } // else if
    } // if

    if (splashbox == NULL)  // no splash, use the window for the top container.
        splashbox = window;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_show(box);
    gtk_container_add(GTK_CONTAINER(splashbox), box);

    pagetitle = gtk_label_new("");
    gtk_widget_show(pagetitle);
    gtk_box_pack_start(GTK_BOX(box), pagetitle, FALSE, TRUE, 16);
    
    notebook = gtk_notebook_new();
    gtk_widget_show(notebook);
    gtk_container_set_border_width(GTK_CONTAINER(notebook), 0);
    gtk_box_pack_start(GTK_BOX(box), notebook, TRUE, TRUE, 0);
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
    gtk_widget_set_vexpand(notebook, TRUE);
    gtk_widget_set_hexpand(notebook, TRUE);

    widget = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_show(widget);
    gtk_box_pack_start(GTK_BOX(box), widget, FALSE, FALSE, 0);
    gtk_button_box_set_layout(GTK_BUTTON_BOX (widget), GTK_BUTTONBOX_END);
    //gtk_button_box_set_child_ipadding(GTK_BUTTON_BOX (widget), 6, 0);
    //gtk_button_box_set_spacing(GTK_BUTTON_BOX (widget), 6);

    box = widget;
    cancel = create_button(box, "gtk-cancel", _("Cancel"), signal_clicked);
    back = create_button(box, "gtk-go-back", _("Back"), signal_clicked);
    next = create_button(box, "gtk-go-forward", _("Next"), signal_clicked);
    finish = create_button(box, "gtk-goto-last", _("Finish"), signal_clicked);
    gtk_widget_hide(finish);

    // !!! FIXME: intro page.
    widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(notebook), widget);

    // README/EULA page.
    widget = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(widget),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_shadow_type(
        GTK_SCROLLED_WINDOW(widget),
        GTK_SHADOW_IN);
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(notebook), widget);

    readme = gtk_text_view_new();
    gtk_widget_show(readme);
    gtk_container_add(GTK_CONTAINER(widget), readme);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(readme), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(readme), GTK_WRAP_NONE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(readme), FALSE);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(readme), 4);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(readme), 4);

    // !!! FIXME: options page.
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show(box);
    gtk_container_add(GTK_CONTAINER(notebook), box);

    // Destination page
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show(box);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    widget = gtk_label_new(_("Folder:"));
    gtk_widget_show(widget);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 0);
    gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(widget), FALSE);
    destination = gtk_combo_box_text_new_with_entry();
    g_signal_connect(destination, "changed", G_CALLBACK(signal_dest_changed), NULL);
    gtk_widget_set_halign(GTK_WIDGET(destination), GTK_ALIGN_FILL);
    gtk_widget_set_valign(GTK_WIDGET(destination), GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(hbox), destination, TRUE, TRUE, 0);
    browse = create_button(hbox, "gtk-open",
                           _("B_rowse..."), signal_browse_clicked);
    gtk_widget_show_all (hbox);
    gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(notebook), box);    

    // Product key page
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show(box);

    widget = gtk_label_new(_("Please enter your product key"));
    gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(widget), TRUE);
    gtk_widget_show(widget);
    gtk_box_pack_start(GTK_BOX(box), widget, FALSE, TRUE, 0);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_show(hbox);
    productkey = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(productkey), TRUE);
    gtk_entry_set_visibility(GTK_ENTRY(productkey), TRUE);
    gtk_widget_show(productkey);
    gtk_box_pack_start(GTK_BOX(hbox), productkey, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(notebook), box);    

    // Progress page
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_show(box);
    progressbar = gtk_progress_bar_new();
    gtk_widget_show(progressbar);
    gtk_widget_set_halign(GTK_WIDGET(progressbar), GTK_ALIGN_FILL);
    gtk_widget_set_valign(GTK_WIDGET(progressbar), GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), progressbar, FALSE, TRUE, 0);
    progresslabel = gtk_label_new("");
    gtk_widget_show(progresslabel);
    gtk_box_pack_start(GTK_BOX(box), progresslabel, FALSE, TRUE, 0);
    gtk_label_set_justify(GTK_LABEL(progresslabel), GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap(GTK_LABEL(progresslabel), FALSE);
    gtk_container_add(GTK_CONTAINER(notebook), box);

    // !!! FIXME: final page.
    widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(notebook), widget);
    finallabel = gtk_label_new("");
    gtk_widget_show(finallabel);
    gtk_box_pack_start(GTK_BOX(widget), finallabel, FALSE, TRUE, 0);
    gtk_label_set_justify(GTK_LABEL(finallabel), GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap(GTK_LABEL(finallabel), TRUE);

    g_signal_connect(window, "delete-event", G_CALLBACK(signal_delete), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show(window);

    gtk_widget_grab_focus(next);

    return window;
} // create_gtkwindow


static boolean MojoGui_gtkplus3_start(const char *title,
                                      const MojoGuiSplash *splash)
{
    gtkwindow = create_gtkwindow(title, splash);
    return (gtkwindow != NULL);
} // MojoGui_gtkplus3_start


static void MojoGui_gtkplus3_stop(void)
{
    assert(msgbox == NULL);
    if (gtkwindow != NULL)
        gtk_widget_destroy(gtkwindow);

    gtkwindow = NULL;
    pagetitle = NULL;
    finallabel = NULL;
    progresslabel = NULL;
    progressbar = NULL;
    destination = NULL;
    productkey = NULL;
    notebook = NULL;
    readme = NULL;
    cancel = NULL;
    back = NULL;
    next = NULL;
    finish = NULL;
    splash = NULL;
} // MojoGui_gtkplus3_stop


static int MojoGui_gtkplus3_readme(const char *name, const uint8 *data,
                                   size_t datalen, boolean can_back,
                                   boolean can_fwd)
{
    GtkTextBuffer *textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(readme));
    gtk_text_buffer_set_text(textbuf, (const gchar *) data, datalen);
    return run_wizard(name, PAGE_README, can_back, can_fwd, true, can_fwd);
} // MojoGui_gtkplus3_readme


static void set_option_tree_sensitivity(MojoGuiSetupOptions *opts, boolean val)
{
    if (opts != NULL)
    {
        gtk_widget_set_sensitive((GtkWidget *) opts->guiopaque, val);
        set_option_tree_sensitivity(opts->next_sibling, val);
        set_option_tree_sensitivity(opts->child, val && opts->value);
    } // if
} // set_option_tree_sensitivity


static void signal_option_toggled(GtkToggleButton *toggle, gpointer data)
{
    MojoGuiSetupOptions *opts = (MojoGuiSetupOptions *) data;
    const boolean enabled = gtk_toggle_button_get_active(toggle);
    opts->value = enabled;
    set_option_tree_sensitivity(opts->child, enabled);
} // signal_option_toggled


static GtkWidget *new_option_level(GtkWidget *box)
{
    GtkWidget *retval = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(retval, GTK_ALIGN_START);
    gtk_widget_set_valign(retval, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(retval, 15);
    gtk_widget_show(retval);
    gtk_box_pack_start(GTK_BOX(box), retval, TRUE, TRUE, 0);
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
            gtk_widget_show(widget);
            gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_LEFT);
            gtk_label_set_line_wrap(GTK_LABEL(widget), FALSE);
            gtk_box_pack_start(GTK_BOX(box), widget, FALSE, TRUE, 0);

            if (opts->tooltip != NULL)
                gtk_widget_set_tooltip_text(widget, opts->tooltip);

            widget = NULL;
            childbox = new_option_level(box);
            while (kids)
            {
                widget = gtk_radio_button_new_with_label_from_widget(
                                                    GTK_RADIO_BUTTON(widget),
                                                    kids->description);
                kids->guiopaque = widget;
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),
                                             kids->value);
                gtk_widget_set_sensitive(widget, sensitive);
                gtk_widget_show(widget);
                gtk_box_pack_start(GTK_BOX(childbox), widget, FALSE, TRUE, 0);
                g_signal_connect(widget, "toggled", G_CALLBACK(signal_option_toggled), kids);

                if (kids->tooltip != NULL)
                    gtk_widget_set_tooltip_text(widget, kids->tooltip);

                if (kids->child != NULL)
                {
                    build_options(kids->child, new_option_level(childbox),
                                  sensitive);
                } // if
                kids = kids->next_sibling;
            } // while
        } // if

        else
        {
            widget = gtk_check_button_new_with_label(opts->description);
            opts->guiopaque = widget;
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),
                                         opts->value);
            gtk_widget_set_sensitive(widget, sensitive);
            gtk_widget_show(widget);
            gtk_box_pack_start(GTK_BOX(box), widget, FALSE, TRUE, 0);
            g_signal_connect(widget, "toggled", G_CALLBACK(signal_option_toggled), opts);

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


static int MojoGui_gtkplus3_options(MojoGuiSetupOptions *opts,
                                    boolean can_back, boolean can_fwd)
{
    int retval;
    GtkWidget *box;
    GtkWidget *page;  // this is a vbox.

    page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), PAGE_OPTIONS);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show(box);
    gtk_box_pack_start(GTK_BOX(page), box, FALSE, FALSE, 0);

    build_options(opts, box, TRUE);
    retval = run_wizard(_("Options"), PAGE_OPTIONS,
                        can_back, can_fwd, true, can_fwd);
    gtk_widget_destroy(box);
    return retval;
} // MojoGui_gtkplus3_options


static char *MojoGui_gtkplus3_destination(const char **recommends, int recnum,
                                          int *command, boolean can_back,
                                          boolean can_fwd)
{
    GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(destination);
    const boolean fwd_at_start = ( (recnum > 0) && (*(recommends[0])) );
    gchar *str = NULL;
    char *retval = NULL;
    int i;

    for (i = 0; i < recnum; i++)
        gtk_combo_box_text_append_text(combo, recommends[i]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

    *command = run_wizard(_("Destination"), PAGE_DEST,
                          can_back, can_fwd, true, fwd_at_start);

    str = gtk_combo_box_text_get_active_text(combo);

    // shouldn't ever be empty ("next" should be disabled in that case).
    assert( (*command <= 0) || ((str != NULL) && (*str != '\0')) );

    retval = xstrdup(str);
    g_free(str);

    for (i = recnum-1; i >= 0; i--)
        gtk_combo_box_text_remove(combo, i);

    return retval;
} // MojoGui_gtkplus3_destination


static int MojoGui_gtkplus3_productkey(const char *desc, const char *fmt,
                                       char *buf, const int buflen,
                                       boolean can_back, boolean can_fwd)
{
    gchar *str = NULL;
    int retval = 0;
    const boolean fwd_at_start = isValidProductKey(fmt, buf);

    gtk_entry_set_max_length(GTK_ENTRY(productkey), buflen - 1);
    gtk_entry_set_width_chars(GTK_ENTRY(productkey), buflen - 1);
    gtk_entry_set_text(GTK_ENTRY(productkey), (gchar *) buf);

    const guint connid = g_signal_connect(productkey, "changed",
                                    G_CALLBACK(signal_productkey_changed),
                                    (void *) fmt);
    retval = run_wizard(desc, PAGE_PRODUCTKEY,
                        can_back, can_fwd, true, fwd_at_start);
    g_signal_handler_disconnect(productkey, connid);

    str = gtk_editable_get_chars(GTK_EDITABLE(productkey), 0, -1);
    // should never be invalid ("next" should be disabled in that case).
    assert( (retval <= 0) || ((str) && (isValidProductKey(fmt, str))) );
    assert(strlen(str) < buflen);
    strcpy(buf, (char *) str);
    g_free(str);
    gtk_entry_set_text(GTK_ENTRY(productkey), "");

    return retval;
} // MojoGui_gtkplus3_productkey


static boolean MojoGui_gtkplus3_insertmedia(const char *medianame)
{
    gint rc = 0;
    // !!! FIXME: Use stock GTK icon for "media"?
    // !!! FIXME: improve the title and text wording?
    const char *text = format(_("Please insert '%0'"), medianame);
    rc = do_msgbox(_("Media change"), text, GTK_MESSAGE_WARNING,
                   GTK_BUTTONS_OK_CANCEL, GTK_RESPONSE_OK, NULL);
    free((void *) text);
    return (rc == GTK_RESPONSE_OK);
} // MojoGui_gtkplus3_insertmedia


static void MojoGui_gtkplus3_progressitem(void)
{
    // no-op in this UI target.
} // MojoGui_gtkplus3_progressitem


static boolean MojoGui_gtkplus3_progress(const char *type, const char *component,
                                         int percent, const char *item,
                                         boolean can_cancel)
{
    static uint32 lastTicks = 0;
    const uint32 ticks = ticks();
    int rc;

    if ((ticks - lastTicks) > 200)  // just not to spam this...
    {
        GtkProgressBar *progress = GTK_PROGRESS_BAR(progressbar);
        if (percent < 0)
            gtk_progress_bar_pulse(progress);
        else
            gtk_progress_bar_set_fraction(progress, ((gdouble) percent) / 100.0);

        gtk_progress_bar_set_text(progress, component);
        gtk_label_set_text(GTK_LABEL(progresslabel), item);
        lastTicks = ticks;
    } // if

    prepare_wizard(type, PAGE_PROGRESS, false, false, can_cancel, false);
    rc = pump_events();
    assert( (rc == ((int) CLICK_CANCEL)) || (rc == ((int) CLICK_NONE)) );
    return (rc != CLICK_CANCEL);
} // MojoGui_gtkplus3_progress


static void MojoGui_gtkplus3_final(const char *msg)
{
    gtk_widget_hide(next);
    gtk_widget_show(finish);
    gtk_label_set_text(GTK_LABEL(finallabel), msg);
    run_wizard(_("Finish"), PAGE_FINAL, false, true, false, true);
} // MojoGui_gtkplus3_final

// end of gui_gtkplus3.c ...


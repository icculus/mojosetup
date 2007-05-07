#if !SUPPORT_GUI_MACOSX
#error Something is wrong in the build system.
#endif

#include <Carbon/Carbon.h>
#undef true
#undef false

#define BUILDING_EXTERNAL_PLUGIN 1
#include "../gui.h"

MOJOGUI_PLUGIN(macosx)

#if !GUI_STATIC_LINK_MACOSX
CREATE_MOJOGUI_ENTRY_POINT(macosx)
#endif

// (A lot of this is stolen from MojoPatch: http://icculus.org/mojopatch/ ...)

static uint8 MojoGui_macosx_priority(void)
{
    return MOJOGUI_PRIORITY_TRY_FIRST;
} // MojoGui_macosx_priority

static boolean MojoGui_macosx_init(void)
{
    // This lets a stdio app become a GUI app. Otherwise, you won't get
    //  GUI events from the system and other things will fail to work.
    // Putting the app in an application bundle does the same thing.
    //  TransformProcessType() is a 10.3+ API. SetFrontProcess() is 10.0+.
    if (TransformProcessType != NULL)  // check it as a weak symbol.
    {
        ProcessSerialNumber psn = { 0, kCurrentProcess };
        TransformProcessType(&psn, kProcessTransformToForegroundApplication);
        SetFrontProcess(&psn);
    } // if

    return true;
} // MojoGui_macosx_init

static void MojoGui_macosx_deinit(void)
{
    // no-op
} // MojoGui_macosx_deinit

static int do_msgbox(const char *_title, const char *str, AlertType alert_type,
                     AlertStdCFStringAlertParamRec *param,
                     DialogItemIndex *idx)
{
    int retval = 0;
    DialogItemIndex val = 0;
    CFStringRef title = CFStringCreateWithBytes(NULL,
                                                (const unsigned char *) _title,
                                                strlen(_title),
                                                kCFStringEncodingUTF8, 0);
    CFStringRef msg = CFStringCreateWithBytes(NULL,
                                              (const unsigned char *) str,
                                              strlen(str),
                                              kCFStringEncodingUTF8, 0);
    if ((msg != NULL) && (title != NULL))
    {
        DialogRef dlg = NULL;

        if (CreateStandardAlert(alert_type, title, msg, param, &dlg) == noErr)
        {
            RunStandardAlert(dlg, NULL, (idx) ? idx : &val);
            retval = 1;
        } /* if */
    } /* if */

    if (msg != NULL)
        CFRelease(msg);

    if (title != NULL)
        CFRelease(title);

    return(retval);
} // do_msgbox

static void MojoGui_macosx_msgbox(const char *title, const char *text)
{
    do_msgbox(title, text, kAlertNoteAlert, NULL, NULL);
} // MojoGui_macosx_msgbox


static boolean do_prompt(const char *title, const char *text,
                         boolean yes, const char *yesstr, const char *nostr)
{
    boolean retval = false;
    OSStatus err = noErr;
    AlertStdCFStringAlertParamRec params;

    err = GetStandardAlertDefaultParams(&params, kStdCFStringAlertVersionOne);
    if (err == noErr)
    {
        DialogItemIndex item;
        CFStringRef yes, no;
        const CFStringEncoding enc = kCFStringEncodingUTF8;
        yes = CFStringCreateWithCString(NULL, yesstr, enc);
        no = CFStringCreateWithCString(NULL, nostr, enc);

        params.movable = TRUE;
        params.helpButton = FALSE;
        params.defaultText = yes;
        params.cancelText = no;
        params.defaultButton = (yes) ? kAlertStdAlertOKButton :
                                       kAlertStdAlertCancelButton;
        params.cancelButton = kAlertStdAlertCancelButton;
        if (do_msgbox(title, text, kAlertCautionAlert, &params, &item))
            retval = (item == kAlertStdAlertOKButton);

        CFRelease(yes);
        CFRelease(no);
    } // if

    return retval;
} // do_promptyn


static boolean MojoGui_macosx_promptyn(const char *title, const char *text)
{
    return do_prompt(title, text, true, entry->_("Yes"), entry->_("No"));
} // MojoGui_macosx_promptyn


static boolean MojoGui_macosx_start(const char *title, const char *splash)
{
    return true;  // !!! FIXME
} // MojoGui_macosx_start


static void MojoGui_macosx_stop(void)
{
    // no-op.
} // MojoGui_macosx_stop


static boolean MojoGui_macosx_readme(const char *name, const uint8 *data,
                                    size_t len, boolean can_go_back,
                                    boolean can_go_forward)
{
    STUBBED("macosx readme");
    return true;
} // MojoGui_macosx_readme


static boolean MojoGui_macosx_options(MojoGuiSetupOptions *opts,
                       boolean can_go_back, boolean can_go_forward)
{
    // !!! FIXME: write me.
    STUBBED("macosx options");
    return true;
} // MojoGui_macosx_options

static char *MojoGui_macosx_destination(const char **recommends, int reccount,
                                       boolean can_go_back, boolean can_go_fwd)
{
    // !!! FIXME: write me.
    STUBBED("macosx destination");
    return entry->xstrdup("/Applications");
} // MojoGui_macosx_destination


static boolean MojoGui_macosx_insertmedia(const char *medianame)
{
    // !!! FIXME: "please insert '%s' ..."
    // !!! FIXME: localization.
    return do_prompt("Please insert", medianame, true,
                     entry->_("OK"), entry->_("Cancel"));
} // MojoGui_macosx_insertmedia

static boolean MojoGui_macosx_progress(const char *type, const char *component,
                                       int percent, const char *item)
{
    // !!! FIXME: write me.
    STUBBED(__FUNCTION__)
    return true;
} // MojoGui_macosx_progress

// end of gui_macosx.c ...


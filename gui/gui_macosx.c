#define BUILDING_EXTERNAL_PLUGIN 1
#include "../gui.h"

#if SUPPORT_GUI_MACOSX

MOJOGUI_PLUGIN(macosx)

#if !GUI_STATIC_LINK_MACOSX
CREATE_MOJOGUI_ENTRY_POINT(macosx)
#endif

#include <Carbon/Carbon.h>

// (A lot of this is stolen from MojoPatch: http://icculus.org/mojopatch/ ...)

static uint8 MojoGui_macosx_priority(void)
{
    return MOJOGUI_PRIORITY_TRY_FIRST;
} // MojoGui_macosx_priority

static boolean MojoGui_macosx_init(void)
{
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


static boolean do_promptyn(const char *title, const char *text, boolean yes)
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
        yes = CFStringCreateWithCString(NULL, entry->_("Yes"), enc);
        no = CFStringCreateWithCString(NULL, entry->_("No"), enc);

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
    return do_promptyn(title, text, true);
}

#endif // SUPPORT_GUI_MACOSX

// end of gui_macosx.c ...


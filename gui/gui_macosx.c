#define BUILDING_EXTERNAL_PLUGIN 1
#include "../gui.h"

#if SUPPORT_GUI_MACOSX

#include <Carbon/Carbon.h>

// (A lot of this is stolen from MojoPatch: http://icculus.org/mojopatch/ ...)

static uint8 MojoGui_macosx_priority(MojoGui *gui)
{
    return MOJOGUI_PRIORITY_TRY_FIRST;
} // MojoGui_macosx_priority

static boolean MojoGui_macosx_init(MojoGui *gui)
{
    return true;
} // MojoGui_macosx_init

static void MojoGui_macosx_deinit(MojoGui *gui)
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
                                                kCFStringEncodingISOLatin1, 0);
    CFStringRef msg = CFStringCreateWithBytes(NULL,
                                              (const unsigned char *) str,
                                              strlen(str),
                                              kCFStringEncodingISOLatin1, 0);
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

static void MojoGui_macosx_msgbox(MojoGui *gui, const char *title,
                                  const char *text)
{
    do_msgbox(title, text, kAlertNoteAlert, NULL, NULL);
} // MojoGui_macosx_msgbox

static boolean do_promptyn(const char *title, const char *text, boolean yes)
{
    OSStatus err;
    DialogItemIndex item;
    AlertStdCFStringAlertParamRec params;
    err = GetStandardAlertDefaultParams(&params, kStdCFStringAlertVersionOne);
    if (err != noErr)
        return false;

    params.movable = TRUE;
    params.helpButton = FALSE;
    params.defaultText = CFSTR("Yes");  // !!! FIXME: localize
    params.cancelText = CFSTR("No");
    params.defaultButton = (yes) ? kAlertStdAlertOKButton :
                                   kAlertStdAlertCancelButton;
    params.cancelButton = kAlertStdAlertCancelButton;
    if (!do_msgbox(title, text, kAlertCautionAlert, &params, &item))
        return false; /* oh well. */

    return(item == kAlertStdAlertOKButton);
} // do_promptyn


static boolean MojoGui_macosx_promptyn(MojoGui *gui, const char *title,
                                        const char *text)
{
    return do_promptyn(title, text, true);
}

MOJOGUI_PLUGIN(macosx)

#if !GUI_STATIC_LINK_MACOSX
CREATE_MOJOGUI_ENTRY_POINT(macosx)
#endif

#endif // SUPPORT_GUI_MACOSX

// end of gui_macosx.c ...


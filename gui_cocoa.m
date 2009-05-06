/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#if !SUPPORT_GUI_COCOA
#error Something is wrong in the build system.
#endif

#import <Cocoa/Cocoa.h>
#undef true
#undef false

#define BUILDING_EXTERNAL_PLUGIN 1
#include "gui.h"

MOJOGUI_PLUGIN(cocoa)

#if !GUI_STATIC_LINK_COCOA
CREATE_MOJOGUI_ENTRY_POINT(cocoa)
#endif

typedef enum
{
    CLICK_BACK=-1,
    CLICK_CANCEL,
    CLICK_NEXT,
    CLICK_NONE
} ClickValue;

static NSAutoreleasePool *GAutoreleasePool = nil;

@interface MojoSetupController : NSView
{
    IBOutlet NSButton *BackButton;
    IBOutlet NSButton *CancelButton;
    IBOutlet NSButton *NextButton;
    IBOutlet NSWindow *MainWindow;
    IBOutlet NSComboBox *DestinationCombo;
    IBOutlet NSTextField *FinalText;
    IBOutlet NSTextView *ReadmeText;
    IBOutlet NSTabView *TabView;
    IBOutlet NSTextField *TitleLabel;
    ClickValue clickValue;
    boolean canForward;
}
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification;
- (void)prepareWidgets:(const char*)winTitle;
- (void)unprepareWidgets;
- (IBAction)backClicked:(NSButton *)sender;
- (IBAction)cancelClicked:(NSButton *)sender;
- (IBAction)nextClicked:(NSButton *)sender;
- (IBAction)browseClicked:(NSButton *)sender;
- (int)runPage:(NSString *)pageId title:(const char *)_title canBack:(boolean)canBack canFwd:(boolean)canFwd canCancel:(boolean)canCancel canFwdAtStart:(boolean)canFwdAtStart;
- (int)doReadme:(const char *)title text:(NSString *)text canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (int)doOptions:(MojoGuiSetupOptions *)opts canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (char *)doDestination:(const char **)recommends recnum:(int)recnum command:(int *)command canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (int)doProductKey:(const char *)desc fmt:(const char *)fmt buf:(char *)buf buflen:(const int)buflen canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (int)doProgress:(const char *)type component:(const char *)component percent:(int)percent item:(const char *)item canCancel:(boolean)canCancel;
- (void)doFinal:(const char *)msg;
@end // interface MojoSetupController

@implementation MojoSetupController
    - (void)applicationDidFinishLaunching:(NSNotification *)aNotification
    {
        printf("didfinishlaunching\n");
        [NSApp stop:self];  // break out of NSApp::run()
    } // applicationDidFinishLaunching

    - (void)prepareWidgets:(const char*)winTitle
    {
        [BackButton setTitle:[NSString stringWithUTF8String:_("Back")]];
        [NextButton setTitle:[NSString stringWithUTF8String:_("Next")]];
        [CancelButton setTitle:[NSString stringWithUTF8String:_("Cancel")]];
        [MainWindow setTitle:[NSString stringWithUTF8String:winTitle]];
        [MainWindow center];
        [MainWindow makeKeyAndOrderFront:self];
    } // prepareWidgets

    - (void)unprepareWidgets
    {
        [MainWindow orderOut:self];
    } // unprepareWidgets

    - (IBAction)backClicked:(NSButton *)sender
    {
        clickValue = CLICK_BACK;
        [NSApp stop:self];
    } // backClicked

    - (IBAction)cancelClicked:(NSButton *)sender
    {
        char *title = xstrdup(_("Cancel installation"));
        char *text = xstrdup(_("Are you sure you want to cancel installation?"));
        const boolean rc = MojoGui_cocoa_promptyn(title, text, false);
        free(title);
        free(text);
        if (rc)
        {
            clickValue = CLICK_CANCEL;
            [NSApp stop:self];
        } // if
    } // cancelClicked

    - (IBAction)nextClicked:(NSButton *)sender
    {
        clickValue = CLICK_NEXT;
        [NSApp stop:self];
    } // nextClicked

    - (IBAction)browseClicked:(NSButton *)sender
    {
        // !!! FIXME: write me!
        STUBBED("browseClicked");
    } // nextClicked

    - (int)runPage:(NSString *)pageId title:(const char *)_title canBack:(boolean)canBack canFwd:(boolean)canFwd canCancel:(boolean)canCancel canFwdAtStart:(boolean)canFwdAtStart
    {
        [TitleLabel setStringValue:[NSString stringWithUTF8String:_title]];
        clickValue = CLICK_NONE;
        canForward = canFwd;
        [BackButton setEnabled:canBack ? YES : NO];
        [NextButton setEnabled:canFwdAtStart ? YES : NO];
        [CancelButton setEnabled:canCancel ? YES : NO];
        [TabView selectTabViewItemWithIdentifier:pageId];
        [NSApp run];
        assert(clickValue < CLICK_NONE);
        return (int) clickValue;
    } // runPage

    - (int)doReadme:(const char *)title text:(NSString *)text canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        NSRange range = {0, 1};  // reset scrolling to start of text.
        [ReadmeText setString:text];
        [ReadmeText scrollRangeToVisible:range];
        return [self runPage:@"Readme" title:title canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:canFwd];
    } // doReadme

    - (int)doOptions:(MojoGuiSetupOptions *)opts canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        // !!! FIXME: write me!
        return [self runPage:@"Options" title:_("Options") canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:canFwd];
    } // doOptions

    - (char *)doDestination:(const char **)recommends recnum:(int)recnum command:(int *)command canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        // !!! FIXME: write me!
        const boolean fwdAtStart = ( (recnum > 0) && (*(recommends[0])) );
        *command = [self runPage:@"Destination" title:_("Destination") canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:fwdAtStart];
        return xstrdup("/Applications");
    } // doDestination

    - (int)doProductKey:(const char *)desc fmt:(const char *)fmt buf:(char *)buf buflen:(const int)buflen canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        // !!! FIXME: write me!
        return [self runPage:@"ProductKey" title:desc canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:canFwd];
    } // doProductKey

    - (int)doProgress:(const char *)type component:(const char *)component percent:(int)percent item:(const char *)item canCancel:(boolean)canCancel
    {
        // !!! FIXME: write me!
        return [self runPage:@"Progress" title:type canBack:false canFwd:false canCancel:canCancel canFwdAtStart:false];
    } // doProgress

    - (void)doFinal:(const char *)msg
    {
        [FinalText setStringValue:[NSString stringWithUTF8String:msg]];
        [NextButton setTitle:[NSString stringWithUTF8String:_("Finish")]];
        [self runPage:@"Final" title:_("Finish") canBack:false canFwd:true canCancel:false canFwdAtStart:true];
    } // doFinal
@end // implementation MojoSetupController


static uint8 MojoGui_cocoa_priority(boolean istty)
{
    // obviously this is the thing you want on Mac OS X.
    return MOJOGUI_PRIORITY_TRY_FIRST;
} // MojoGui_cocoa_priority


static boolean MojoGui_cocoa_init(void)
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

	GAutoreleasePool = [[NSAutoreleasePool alloc] init];

    // !!! FIXME: make sure we have access to the desktop...may be ssh'd in
    // !!! FIXME:  as another user that doesn't have the Finder loaded or
    // !!! FIXME:  something.
    [NSApplication sharedApplication];
    if ([NSBundle loadNibNamed:@"MojoSetup" owner:NSApp] == NO)
        return false;

    // Force NSApp initialization stuff. MojoSetupController is set, in the
    //  .nib, to be NSApp's delegate. Its applicationDidFinishLaunching calls
    //  [NSApp stop] to break event loop right away so we can continue.
    [NSApp run];

    return true;  // always succeeds.
} // MojoGui_cocoa_init


static void MojoGui_cocoa_deinit(void)
{
    [GAutoreleasePool release];
    GAutoreleasePool = nil;
    // !!! FIXME: destroy nib and NSApp?
} // MojoGui_cocoa_deinit


static void MojoGui_cocoa_msgbox(const char *_title, const char *_text)
{
    NSString *title = [NSString stringWithUTF8String:_title];
    NSString *text = [NSString stringWithUTF8String:_text];
    NSString *ok = [NSString stringWithUTF8String:_("OK")];
    NSRunInformationalAlertPanel(title, text, ok, nil, nil);
} // MojoGui_cocoa_msgbox


static boolean MojoGui_cocoa_promptyn(const char *_title, const char *_text,
                                      boolean defval)
{
    NSString *title = [NSString stringWithUTF8String:_title];
    NSString *text = [NSString stringWithUTF8String:_text];
    NSString *yesstr = [NSString stringWithUTF8String:_("Yes")];
    NSString *nostr = [NSString stringWithUTF8String:_("No")];
    const NSInteger rc = NSRunAlertPanel(title, text, yesstr, nostr, nil);
    return (rc == NSAlertDefaultReturn);
} // MojoGui_cocoa_promptyn


static MojoGuiYNAN MojoGui_cocoa_promptynan(const char *title,
                                            const char *text,
                                            boolean defval)
{
    STUBBED("ynan");
    return MojoGui_cocoa_promptyn(title, text, defval);  // !!! FIXME
} // MojoGui_cocoa_promptynan


static boolean MojoGui_cocoa_start(const char *title,
                                   const MojoGuiSplash *splash)
{
printf("start\n");
    // !!! FIXME: deal with (splash).
    [[NSApp delegate] prepareWidgets:title];
    return true;
} // MojoGui_cocoa_start


static void MojoGui_cocoa_stop(void)
{
printf("stop\n");
    [[NSApp delegate] unprepareWidgets];
} // MojoGui_cocoa_stop


static int MojoGui_cocoa_readme(const char *name, const uint8 *data,
                                    size_t len, boolean can_back,
                                    boolean can_fwd)
{
printf("readme\n");
    NSString *str = [[NSString alloc] initWithBytes:data length:len encoding:NSUTF8StringEncoding];
    return [[NSApp delegate] doReadme:name text:str canBack:can_back canFwd:can_fwd];
} // MojoGui_cocoa_readme


static int MojoGui_cocoa_options(MojoGuiSetupOptions *opts,
                       boolean can_back, boolean can_fwd)
{
printf("options\n");
    return [[NSApp delegate] doOptions:opts canBack:can_back canFwd:can_fwd];
} // MojoGui_cocoa_options


static char *MojoGui_cocoa_destination(const char **recommends, int recnum,
                                       int *command, boolean can_back,
                                       boolean can_fwd)
{
printf("destination\n");
    return [[NSApp delegate] doDestination:recommends recnum:recnum command:command canBack:can_back canFwd:can_fwd];
} // MojoGui_cocoa_destination


static int MojoGui_cocoa_productkey(const char *desc, const char *fmt,
                                    char *buf, const int buflen,
                                    boolean can_back, boolean can_fwd)
{
printf("productkey\n");
    return [[NSApp delegate] doProductKey:desc fmt:fmt buf:buf buflen:buflen canBack:can_back canFwd:can_fwd];
} // MojoGui_cocoa_productkey


static boolean MojoGui_cocoa_insertmedia(const char *medianame)
{
printf("insertmedia\n");
    NSString *title = [NSString stringWithUTF8String:_("Media change")];
    char *fmt = xstrdup(_("Please insert '%0'"));
    char *_text = format(fmt, medianame);
    NSString *text = [NSString stringWithUTF8String:_text];
    free(_text);
    free(fmt);
    NSString *okstr = [NSString stringWithUTF8String:_("OK")];
    NSString *cancelstr = [NSString stringWithUTF8String:_("Cancel")];
    const NSInteger rc = NSRunAlertPanel(title, text, okstr, cancelstr, nil);
    return (rc == NSAlertDefaultReturn);
} // MojoGui_cocoa_insertmedia


static void MojoGui_cocoa_progressitem(void)
{
printf("progressitem\n");
    // no-op in this UI target.
} // MojoGui_cocoa_progressitem


static int MojoGui_cocoa_progress(const char *type, const char *component,
                                  int percent, const char *item,
                                  boolean can_cancel)
{
printf("progress\n");
    return [[NSApp delegate] doProgress:type component:component percent:percent item:item canCancel:can_cancel];
} // MojoGui_cocoa_progress


static void MojoGui_cocoa_final(const char *msg)
{
printf("final\n");
    [[NSApp delegate] doFinal:msg];
} // MojoGui_cocoa_final

// end of gui_cocoa.m ...


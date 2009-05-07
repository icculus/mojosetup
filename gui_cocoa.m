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

// This nasty hack is because we appear to need to be under
//  -[NSApp run] when calling things like NSRunAlertPanel().
// So we push a custom event, call -[NSApp run], catch it, do
//  the panel, then call -[NSApp stop]. Yuck.
typedef enum
{
    CUSTOMEVENT_RUNQUEUE,
    CUSTOMEVENT_MSGBOX,
    CUSTOMEVENT_PROMPTYN,
    CUSTOMEVENT_PROMPTYNAN,
    CUSTOMEVENT_INSERTMEDIA,
} CustomEvent;


static NSAutoreleasePool *GAutoreleasePool = nil;

@interface MojoSetupController : NSView
{
    IBOutlet NSButton *BackButton;
    IBOutlet NSButton *CancelButton;
    IBOutlet NSComboBox *DestinationCombo;
    IBOutlet NSTextField *FinalText;
    IBOutlet NSWindow *MainWindow;
    IBOutlet NSButton *NextButton;
    IBOutlet NSProgressIndicator *ProgressBar;
    IBOutlet NSTextField *ProgressComponentLabel;
    IBOutlet NSTextField *ProgressItemLabel;
    IBOutlet NSTextView *ReadmeText;
    IBOutlet NSTabView *TabView;
    IBOutlet NSTextField *TitleLabel;
    IBOutlet NSMenuItem *QuitMenuItem;
    ClickValue clickValue;
    boolean canForward;
    boolean needToBreakEventLoop;
    boolean finalPage;
    MojoGuiYNAN answerYNAN;
}
- (void)awakeFromNib;
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification;
- (void)prepareWidgets:(const char*)winTitle;
- (void)unprepareWidgets;
- (void)fireCustomEvent:(CustomEvent)eventType data1:(NSInteger)data1 data2:(NSInteger)data2 atStart:(BOOL)atStart;
- (void)doCustomEvent:(NSEvent *)event;
- (void)doMsgBox:(const char *)title text:(const char *)text;
- (void)doPromptYN:(const char *)title text:(const char *)text;
- (void)doPromptYNAN:(const char *)title text:(const char *)text;
- (void)doInsertMedia:(const char *)medianame;
- (MojoGuiYNAN)getAnswerYNAN;
- (IBAction)backClicked:(NSButton *)sender;
- (IBAction)cancelClicked:(NSButton *)sender;
- (IBAction)nextClicked:(NSButton *)sender;
- (IBAction)browseClicked:(NSButton *)sender;
- (IBAction)menuQuit:(NSMenuItem *)sender;
- (int)doPage:(NSString *)pageId title:(const char *)_title canBack:(boolean)canBack canFwd:(boolean)canFwd canCancel:(boolean)canCancel canFwdAtStart:(boolean)canFwdAtStart shouldBlock:(BOOL)shouldBlock;
- (int)doReadme:(const char *)title text:(NSString *)text canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (int)doOptions:(MojoGuiSetupOptions *)opts canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (char *)doDestination:(const char **)recommends recnum:(int)recnum command:(int *)command canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (int)doProductKey:(const char *)desc fmt:(const char *)fmt buf:(char *)buf buflen:(const int)buflen canBack:(boolean)canBack canFwd:(boolean)canFwd;
- (int)doProgress:(const char *)type component:(const char *)component percent:(int)percent item:(const char *)item canCancel:(boolean)canCancel;
- (void)doFinal:(const char *)msg;
@end // interface MojoSetupController

@implementation MojoSetupController
    - (void)awakeFromNib
    {
        clickValue = CLICK_NONE;
        canForward = false;
        answerYNAN = MOJOGUI_NO;
        needToBreakEventLoop = false;
        finalPage = false;
    } // awakeFromNib

    - (void)applicationDidFinishLaunching:(NSNotification *)aNotification
    {
        printf("didfinishlaunching\n");
        [NSApp stop:self];  // break out of NSApp::run()
    } // applicationDidFinishLaunching

    - (void)prepareWidgets:(const char*)winTitle
    {
        #if 1
        [BackButton setTitle:[NSString stringWithUTF8String:_("Back")]];
        [NextButton setTitle:[NSString stringWithUTF8String:_("Next")]];
        [CancelButton setTitle:[NSString stringWithUTF8String:_("Cancel")]];
        #else
        // !!! FIXME: there's probably a better way to do this.
        // Set the correct localization for the buttons, then resize them so
        //  the new text fits perfectly. After that, we need to reposition
        //  them so they don't look scattered.
        NSRect frameBack = [BackButton frame];
        NSRect frameNext = [NextButton frame];
        NSRect frameCancel = [CancelButton frame];
        const float startX = frameCancel.origin.x + frameCancel.size.width;
        const float spacing = (frameBack.origin.x + frameBack.size.width) - frameNext.origin.x;
        [BackButton setTitle:[NSString stringWithUTF8String:_("Back")]];
        [NextButton setTitle:[NSString stringWithUTF8String:_("Next")]];
        [CancelButton setTitle:[NSString stringWithUTF8String:_("Cancel")]];
        [BackButton sizeToFit];
        [NextButton sizeToFit];
        [CancelButton sizeToFit];
        frameBack = [BackButton frame];
        frameNext = [NextButton frame];
        frameCancel = [CancelButton frame];
        frameCancel.origin.x = startX - frameCancel.size.width;
        frameNext.origin.x = (frameCancel.origin.x - frameNext.size.width) - spacing;
        frameBack.origin.x = (frameNext.origin.x - frameBack.size.width) - spacing;
        [CancelButton setFrame:frameCancel];
        [CancelButton setNeedsDisplay:YES];
        [NextButton setFrame:frameNext];
        [NextButton setNeedsDisplay:YES];
        [BackButton setFrame:frameBack];
        [BackButton setNeedsDisplay:YES];
        #endif

        [ProgressBar setUsesThreadedAnimation:YES];  // we don't pump fast enough.
        [ProgressBar startAnimation:self];
        [MainWindow setTitle:[NSString stringWithUTF8String:winTitle]];
        [MainWindow center];
        [MainWindow makeKeyAndOrderFront:self];
    } // prepareWidgets

    - (void)unprepareWidgets
    {
        [MainWindow orderOut:self];
    } // unprepareWidgets

    - (void)fireCustomEvent:(CustomEvent)eventType data1:(NSInteger)data1 data2:(NSInteger)data2 atStart:(BOOL)atStart
    {
        NSEvent *event = [NSEvent otherEventWithType:NSApplicationDefined location:NSZeroPoint modifierFlags:0 timestamp:0 windowNumber:0 context:nil subtype:(short)eventType data1:data1 data2:data2];
        [NSApp postEvent:event atStart:atStart];
        [NSApp run];  // event handler _must_ call -[NSApp stop], or you block here forever.
    } // fireCustomEvent

    - (void)doCustomEvent:(NSEvent*)event
    {
        printf("custom event!\n");
        switch ((CustomEvent) [event subtype])
        {
            case CUSTOMEVENT_RUNQUEUE:
                if ([NSApp modalWindow] != nil)
                {
                    // If we're in a modal thing, so don't break the event loop.
                    //  Just make a note to break it later.
                    needToBreakEventLoop = true;
                    return;
                } // if
                break;  // we just need the -[NSApp stop] call.
            case CUSTOMEVENT_MSGBOX:
                [self doMsgBox:(const char *)[event data1] text:(const char *)[event data2]];
                break;
            case CUSTOMEVENT_PROMPTYN:
                [self doPromptYN:(const char *)[event data1] text:(const char *)[event data2]];
                break;
            case CUSTOMEVENT_PROMPTYNAN:
                [self doPromptYNAN:(const char *)[event data1] text:(const char *)[event data2]];
                break;
            case CUSTOMEVENT_INSERTMEDIA:
                [self doInsertMedia:(const char *)[event data1]];
                break;
            default:
                assert(false && "Cocoa: Unexpected custom event");
                return;  // let it go without breaking the event loop.
        } // switch

        [NSApp stop:self];  // break the event loop.
    } // doCustomEvent

    - (void)doMsgBox:(const char *)title text:(const char *)text
    {
        NSString *titlestr = [NSString stringWithUTF8String:title];
        NSString *textstr = [NSString stringWithUTF8String:text];
        NSString *okstr = [NSString stringWithUTF8String:_("OK")];
        NSRunInformationalAlertPanel(titlestr, textstr, okstr, nil, nil);
        if (needToBreakEventLoop)
        {
            needToBreakEventLoop = false;
            [self fireCustomEvent:CUSTOMEVENT_RUNQUEUE data1:0 data2:0 atStart:NO];
        } // if
    } // doMsgBox

    - (void)doPromptYN:(const char *)title text:(const char *)text
    {
        NSString *titlestr = [NSString stringWithUTF8String:title];
        NSString *textstr = [NSString stringWithUTF8String:text];
        NSString *yesstr = [NSString stringWithUTF8String:_("Yes")];
        NSString *nostr = [NSString stringWithUTF8String:_("No")];
        const NSInteger rc = NSRunAlertPanel(titlestr, textstr, yesstr, nostr, nil);
        answerYNAN = ((rc == NSAlertDefaultReturn) ? MOJOGUI_YES : MOJOGUI_NO);
        if (needToBreakEventLoop)
        {
            needToBreakEventLoop = false;
            [self fireCustomEvent:CUSTOMEVENT_RUNQUEUE data1:0 data2:0 atStart:NO];
        } // if
    } // doPromptYN

    - (void)doPromptYNAN:(const char *)title text:(const char *)text
    {
        // !!! FIXME
        [self doPromptYN:title text:text];
    } // doPromptYN

    - (void)doInsertMedia:(const char *)medianame
    {
        NSString *title = [NSString stringWithUTF8String:_("Media change")];
        char *fmt = xstrdup(_("Please insert '%0'"));
        char *_text = format(fmt, medianame);
        NSString *text = [NSString stringWithUTF8String:_text];
        free(_text);
        free(fmt);
        NSString *okstr = [NSString stringWithUTF8String:_("OK")];
        NSString *cancelstr = [NSString stringWithUTF8String:_("Cancel")];
        const NSInteger rc = NSRunAlertPanel(title, text, okstr, cancelstr, nil);
        answerYNAN = ((rc == NSAlertDefaultReturn) ? MOJOGUI_YES : MOJOGUI_NO);
        if (needToBreakEventLoop)
        {
            needToBreakEventLoop = false;
            [self fireCustomEvent:CUSTOMEVENT_RUNQUEUE data1:0 data2:0 atStart:NO];
        } // if
    } // doInsertMedia

    - (MojoGuiYNAN)getAnswerYNAN
    {
        return answerYNAN;
    } // getAnswerYNAN

    - (IBAction)backClicked:(NSButton *)sender
    {
        clickValue = CLICK_BACK;
        [NSApp stop:self];
    } // backClicked

    - (IBAction)cancelClicked:(NSButton *)sender
    {
        char *title = xstrdup(_("Cancel installation"));
        char *text = xstrdup(_("Are you sure you want to cancel installation?"));
        [self doPromptYN:title text:text];
        free(title);
        free(text);
        if (answerYNAN == MOJOGUI_YES)
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

    - (IBAction)menuQuit:(NSMenuItem *)sender
    {
        if (finalPage)  // make this work like you clicked "finished".
            [self nextClicked:nil];
        else  // make this work like you clicked "cancel".
            [self cancelClicked:nil];
    } // menuQuit

    - (int)doPage:(NSString *)pageId title:(const char *)_title canBack:(boolean)canBack canFwd:(boolean)canFwd canCancel:(boolean)canCancel canFwdAtStart:(boolean)canFwdAtStart shouldBlock:(BOOL)shouldBlock
    {
        [TitleLabel setStringValue:[NSString stringWithUTF8String:_title]];
        clickValue = CLICK_NONE;
        canForward = canFwd;
        [BackButton setEnabled:canBack ? YES : NO];
        [NextButton setEnabled:canFwdAtStart ? YES : NO];
        [CancelButton setEnabled:canCancel ? YES : NO];
        [QuitMenuItem setEnabled:canCancel ? YES : NO];
        [TabView selectTabViewItemWithIdentifier:pageId];
        if (shouldBlock == NO)
            [self fireCustomEvent:CUSTOMEVENT_RUNQUEUE data1:0 data2:0 atStart:NO];
        else
        {
            [NSApp run];
            assert(clickValue < CLICK_NONE);
        } // else
        return (int) clickValue;
    } // doPage

    - (int)doReadme:(const char *)title text:(NSString *)text canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        NSRange range = {0, 1};  // reset scrolling to start of text.
        [ReadmeText setString:text];
        [ReadmeText scrollRangeToVisible:range];
        return [self doPage:@"Readme" title:title canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:canFwd shouldBlock:YES];
    } // doReadme

    - (int)doOptions:(MojoGuiSetupOptions *)opts canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        // !!! FIXME: write me!
        return [self doPage:@"Options" title:_("Options") canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:canFwd shouldBlock:YES];
    } // doOptions

    - (char *)doDestination:(const char **)recommends recnum:(int)recnum command:(int *)command canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        // !!! FIXME: write me!
        const boolean fwdAtStart = ( (recnum > 0) && (*(recommends[0])) );
        *command = [self doPage:@"Destination" title:_("Destination") canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:fwdAtStart shouldBlock:YES];
        return xstrdup("/Applications");
    } // doDestination

    - (int)doProductKey:(const char *)desc fmt:(const char *)fmt buf:(char *)buf buflen:(const int)buflen canBack:(boolean)canBack canFwd:(boolean)canFwd
    {
        // !!! FIXME: write me!
        return [self doPage:@"ProductKey" title:desc canBack:canBack canFwd:canFwd canCancel:true canFwdAtStart:canFwd shouldBlock:YES];
    } // doProductKey

    - (int)doProgress:(const char *)type component:(const char *)component percent:(int)percent item:(const char *)item canCancel:(boolean)canCancel
    {
        const BOOL indeterminate = (percent < 0) ? YES : NO;
        [ProgressComponentLabel setStringValue:[NSString stringWithUTF8String:component]];
        [ProgressItemLabel setStringValue:[NSString stringWithUTF8String:item]];
        [ProgressBar setIndeterminate:indeterminate];
        if (!indeterminate)
            [ProgressBar setDoubleValue:(double)percent];
        return [self doPage:@"Progress" title:type canBack:false canFwd:false canCancel:canCancel canFwdAtStart:false shouldBlock:NO];
    } // doProgress

    - (void)doFinal:(const char *)msg
    {
        finalPage = true;
        [FinalText setStringValue:[NSString stringWithUTF8String:msg]];
        [NextButton setTitle:[NSString stringWithUTF8String:_("Finish")]];
        [self doPage:@"Final" title:_("Finish") canBack:false canFwd:true canCancel:false canFwdAtStart:true shouldBlock:YES];
    } // doFinal
@end // implementation MojoSetupController

// Override [NSApplication sendEvent], so we can catch custom events.
@interface MojoSetupApplication : NSApplication
{
}
- (void)sendEvent:(NSEvent *)event;
@end // interface MojoSetupApplication

@implementation MojoSetupApplication
    - (void)sendEvent:(NSEvent *)event
    {
        if ([event type] == NSApplicationDefined)
            [((MojoSetupController *)[self delegate]) doCustomEvent:event];
        [super sendEvent:event];
    } // sendEvent
@end // implementation MojoSetupApplication


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

    // For NSApp to be our subclass, instead of default NSApplication.
    [MojoSetupApplication sharedApplication];
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


static void MojoGui_cocoa_msgbox(const char *title, const char *text)
{
    [[NSApp delegate] fireCustomEvent:CUSTOMEVENT_MSGBOX data1:(NSInteger)title data2:(NSInteger)text atStart:YES];
} // MojoGui_cocoa_msgbox


static boolean MojoGui_cocoa_promptyn(const char *title, const char *text,
                                      boolean defval)
{
    [[NSApp delegate] fireCustomEvent:CUSTOMEVENT_PROMPTYN data1:(NSInteger)title data2:(NSInteger)text atStart:YES];
    const MojoGuiYNAN ynan = [[NSApp delegate] getAnswerYNAN];
    assert((ynan == MOJOGUI_YES) || (ynan == MOJOGUI_NO));
    return (ynan == MOJOGUI_YES);
} // MojoGui_cocoa_promptyn


static MojoGuiYNAN MojoGui_cocoa_promptynan(const char *title,
                                            const char *text,
                                            boolean defval)
{
    [[NSApp delegate] fireCustomEvent:CUSTOMEVENT_PROMPTYNAN data1:(NSInteger)title data2:(NSInteger)text atStart:YES];
    return [[NSApp delegate] getAnswerYNAN];
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
    [[NSApp delegate] fireCustomEvent:CUSTOMEVENT_INSERTMEDIA data1:(NSInteger)medianame data2:0 atStart:YES];
    const MojoGuiYNAN ynan = [[NSApp delegate] getAnswerYNAN];
    assert((ynan == MOJOGUI_YES) || (ynan == MOJOGUI_NO));
    return (ynan == MOJOGUI_YES);
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


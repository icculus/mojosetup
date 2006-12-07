#ifndef _INCL_GUI_H_
#define _INCL_GUI_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    MOJOGUI_PRIORITY_NEVER_TRY = 0,
    MOJOGUI_PRIORITY_USER_REQUESTED,
    MOJOGUI_PRIORITY_TRY_FIRST,
    MOJOGUI_PRIORITY_TRY_NORMAL,
    MOJOGUI_PRIORITY_TRY_LAST,
    MOJOGUI_PRIORITY_TOTAL
} MojoGuiPluginPriority;


/*
 * Abstract GUI interfaces.
 */

#define MOJOGUI_ENTRY_POINT MojoSetup_Gui_GetInterface
#define MOJOGUI_ENTRY_POINT_STR DEFINE_TO_STR(MOJOGUI_ENTRY_POINT)

// Increment this value when MojoGui's structure changes.
#define MOJOGUI_INTERFACE_REVISION 1

typedef struct MojoGui MojoGui;
struct MojoGui
{
    uint8 (*priority)(void);
    const char* (*name)(void);
    boolean (*init)(void);
    void (*deinit)(void);
    void (*msgbox)(const char *title, const char *text);
    boolean (*promptyn)(const char *title, const char *text);
};

typedef const MojoGui* (*MojoGuiEntryPoint)(int revision,
                                            const MojoSetupEntryPoints *e);

#if !BUILDING_EXTERNAL_PLUGIN
extern const MojoGui *GGui;
const MojoGui *MojoGui_initGuiPlugin(void);
void MojoGui_deinitGuiPlugin(void);
#else

__EXPORT__ const MojoGui *MOJOGUI_ENTRY_POINT(int revision,
                                              const MojoSetupEntryPoints *e);

/*
 * We do this as a macro so we only have to update one place, and it
 *  enforces some details in the plugins. Without effort, plugins don't
 *  support anything but the latest version of the interface.
 */
#define MOJOGUI_PLUGIN(module) \
static const MojoSetupEntryPoints *entry = NULL; \
static uint8 MojoGui_##module##_priority(void); \
static const char* MojoGui_##module##_name(void) { return #module; } \
static boolean MojoGui_##module##_init(void); \
static void MojoGui_##module##_deinit(void); \
static void MojoGui_##module##_msgbox(const char *title, const char *text); \
static boolean MojoGui_##module##_promptyn(const char *t1, const char *t2); \
const MojoGui *MojoGuiPlugin_##module(int rev, const MojoSetupEntryPoints *e) \
{ \
    if (rev == MOJOGUI_INTERFACE_REVISION) { \
        static const MojoGui retval = { \
            MojoGui_##module##_priority, \
            MojoGui_##module##_name, \
            MojoGui_##module##_init, \
            MojoGui_##module##_deinit, \
            MojoGui_##module##_msgbox, \
            MojoGui_##module##_promptyn, \
        }; \
        entry = e; \
        return &retval; \
    } \
    return NULL; \
} \

#define CREATE_MOJOGUI_ENTRY_POINT(module) \
const MojoGui *MOJOGUI_ENTRY_POINT(int rev, const MojoSetupEntryPoints *e) \
{ \
    return MojoGuiPlugin_##module(rev, e); \
} \

#endif


/*
 * make some decisions about which GUI plugins to build...
 *  We list them all here, but some are built, some aren't. Some are DLLs,
 *  some aren't...
 */

// Probably want to support this always, unless explicitly overridden.
const MojoGui *MojoGuiPlugin_stdio(int rev, const MojoSetupEntryPoints *e);
// !!! FIXME (Windows code isn't actually written yet...)
const MojoGui *MojoGuiPlugin_windows(int rev, const MojoSetupEntryPoints *e);
// !!! FIXME (GTK+ code isn't actually written yet...)
const MojoGui *MojoGuiPlugin_gtkplus(int rev, const MojoSetupEntryPoints *e);
const MojoGui *MojoGuiPlugin_macosx(int rev, const MojoSetupEntryPoints *e);

// !!! FIXME: Qt? KDE? Gnome? Console? Cocoa?

#ifdef __cplusplus
}
#endif

#endif

// end of gui.h ...


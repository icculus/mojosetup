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
    // public
    uint8 (*priority)(MojoGui *gui);
    const char* (*name)(MojoGui *gui);
    boolean (*init)(MojoGui *gui);
    void (*deinit)(MojoGui *gui);
    void (*msgbox)(MojoGui *gui, const char *title, const char *text);
    boolean (*promptyn)(MojoGui *gui, const char *title, const char *text);

    // private
    void *opaque;
};


typedef MojoGui* (*MojoGuiEntryPoint)(int revision);

#ifndef BUILDING_EXTERNAL_PLUGIN
extern MojoGui *GGui;
MojoGui *MojoGui_initGuiPlugin(void);
void MojoGui_deinitGuiPlugin(void);
#else

__EXPORT__ MojoGui *MOJOGUI_ENTRY_POINT(int revision);

/*
 * We do this as a macro so we only have to update one place, and it
 *  enforces some details in the plugins. Without effort, plugins don't
 *  support anything but the latest version of the interface.
 */
#define MOJOGUI_PLUGIN(module) \
static const char* MojoGui_##module##_name(MojoGui *gui) { return #module; } \
MojoGui *MojoGuiPlugin_##module(int revision) \
{ \
    if (revision == MOJOGUI_INTERFACE_REVISION) { \
        static MojoGui retval; \
        retval.priority = MojoGui_##module##_priority; \
        retval.name = MojoGui_##module##_name; \
        retval.init = MojoGui_##module##_init; \
        retval.deinit = MojoGui_##module##_deinit; \
        retval.msgbox = MojoGui_##module##_msgbox; \
        retval.promptyn = MojoGui_##module##_promptyn; \
        retval.opaque = NULL; \
        return &retval; \
    } \
    return NULL; \
} \

#define CREATE_MOJOGUI_ENTRY_POINT(module) \
MojoGui *MOJOGUI_ENTRY_POINT(int revision) \
{ \
    return MojoGuiPlugin_##module(revision); \
} \

#endif


/*
 * make some decisions about which GUI plugins to build...
 */

// Probably want to support this always, unless explicitly overridden.
MojoGui *MojoGuiPlugin_stdio(int revision);
#ifndef SUPPORT_GUI_STDIO
#define SUPPORT_GUI_STDIO 1
#endif

// Probably want to statically link it, too.
#if SUPPORT_GUI_STDIO
#  ifndef GUI_STATIC_LINK_STDIO
#    define GUI_STATIC_LINK_STDIO 1
#  endif
#endif


// !!! FIXME (Windows code isn't actually written yet...)
// Want to support this always on Windows, unless explicitly overridden.
MojoGui *MojoGuiPlugin_windows(int revision);
#ifndef SUPPORT_GUI_WINDOWS
#  if PLATFORM_WINDOWS
#    define SUPPORT_GUI_WINDOWS 1
#  endif
#endif

// Probably want to statically link it, too.
#if SUPPORT_GUI_WINDOWS
#  ifndef GUI_STATIC_LINK_WINDOWS
#    define GUI_STATIC_LINK_WINDOWS 1
#  endif
#endif


// !!! FIXME (GTK+ code isn't actually written yet...)
// Want to support this always on non-Mac Unix, unless explicitly overridden.
MojoGui *MojoGuiPlugin_gtkplus(int revision);
#ifndef SUPPORT_GUI_GTK_PLUS
#  if ((PLATFORM_UNIX) && (!PLATFORM_MACOSX))
#    define SUPPORT_GUI_GTK_PLUS 1
#  endif
#endif

// Probably DON'T want to statically link it.
#if SUPPORT_GUI_GTK_PLUS
#  ifndef GUI_STATIC_LINK_GTK_PLUS
#    define GUI_STATIC_LINK_GTK_PLUS 0
#  endif
#endif

// Probably want to support this always on Mac OS X.
MojoGui *MojoGuiPlugin_macosx(int revision);
#ifndef SUPPORT_GUI_MACOSX
#  if PLATFORM_MACOSX
#    define SUPPORT_GUI_MACOSX 1
#  endif
#endif

// Probably want to statically link it, too.
#if SUPPORT_GUI_MACOSX
#  ifndef GUI_STATIC_LINK_MACOSX
#    define GUI_STATIC_LINK_MACOSX 1
#  endif
#endif

// !!! FIXME: Qt? KDE? Gnome? Console? Cocoa?

#ifdef __cplusplus
}
#endif

#endif

// end of gui.h ...


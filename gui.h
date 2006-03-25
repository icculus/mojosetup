#ifndef _INCL_GUI_H_
#define _INCL_GUI_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Increment this value when binary compatibility changes. */
#define MOJOGUI_INTERFACE_REVISION 1

typedef enum
{
    MOJOGUI_PRIORITY_NEVER_TRY = 0,
    MOJOGUI_PRIORITY_TRY_FIRST,
    MOJOGUI_PRIORITY_TRY_LAST,
    MOJOGUI_PRIORITY_TRY_NORMAL,
} MojoGuiPluginPriority;

/*
 * Abstract GUI interfaces.
 *
 *  NEVER CHANGE A SHIPPING STRUCTURE! If you have to break binary
 *   compatibility, copy the latest version (MojoGui_revX) to a new struct,
 *   MojoGui_revX+1, and increment MOJOGUI_INTERFACE_REVISION, above.
 */
typedef struct MojoGui_rev1 MojoGui_rev1;
struct MojoGui_rev1
{
    /*public*/
    uint8 (*priority)(MojoGui_rev1 *gui);
    boolean (*init)(MojoGui_rev1 *gui);
    void (*deinit)(MojoGui_rev1 *gui);
    void (*msgbox)(MojoGui_rev1 *gui, const char *title, const char *text);
    boolean (*promptyn)(MojoGui_rev1 *gui, const char *title, const char *text);

    /*private*/
    void *opaque;
};


/* Tapdance to handle interface revisions... */

/*
 * The latest revision struct is just called "MojoGui" for convenience
 *  in places that don't care about backwards compatibility, and can avoid
 *  the Macro Salsa.
 */
#define MOJOGUI_STRUCT_VER(v) MojoGui_rev##v
#define MOJOGUI_STRUCT2(v) MOJOGUI_STRUCT_VER(v)
#define MOJOGUI_STRUCT MOJOGUI_STRUCT2(MOJOGUI_INTERFACE_REVISION)
#define MOJOGUI_ENTRY_POINT_VER(v) MojoSetup_GUI_GetInterface_rev##v
#define MOJOGUI_ENTRY_POINT2(v) MOJOGUI_ENTRY_POINT_VER(v)
#define MOJOGUI_ENTRY_POINT MOJOGUI_ENTRY_POINT2(MOJOGUI_INTERFACE_REVISION)
#define MOJOGUI_ENTRY_POINT_STR3(v) #v
#define MOJOGUI_ENTRY_POINT_STR2(v) MOJOGUI_ENTRY_POINT_STR3(v)
#define MOJOGUI_ENTRY_POINT_STR_VER(v) MOJOGUI_ENTRY_POINT_STR2(MOJOGUI_ENTRY_POINT2(v))
#define MOJOGUI_ENTRY_POINT_STR MOJOGUI_ENTRY_POINT_STR_VER(MOJOGUI_INTERFACE_REVISION)

typedef MOJOGUI_STRUCT MojoGui;

__EXPORT__ MOJOGUI_STRUCT *MOJOGUI_ENTRY_POINT(void);
extern MojoGui *GGui;

/*
 * We do this as a macro so we only have to update one place, and it
 *  enforces some details in the plugins. Without effort, plugins don't
 *  support anything but the latest version of the interface.
 */
#define CREATE_MOJOGUI_ENTRY_POINT(module) \
MOJOGUI_STRUCT *MOJOGUI_ENTRY_POINT(void) \
{ \
    static MOJOGUI_STRUCT retval; \
    retval.priority = MojoGui_##module##_priority; \
    retval.init = MojoGui_##module##_init; \
    retval.deinit = MojoGui_##module##_deinit; \
    retval.msgbox = MojoGui_##module##_msgbox; \
    retval.promptyn = MojoGui_##module##_promptyn; \
    retval.opaque = NULL; \
    return &retval; \
} \

#ifdef __cplusplus
}
#endif

#endif

/* end of gui.h ... */


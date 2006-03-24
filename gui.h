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
#define MOJOGUI_ENTRY_POINT_VER(v) MojoSetup_GUI_GetInterface_rev##v
#define MOJOGUI_ENTRY_POINT MOJOGUI_ENTRY_POINT_VER(MOJOGUI_INTERFACE_REVISION)
#define MOJOGUI_STRUCT_VER(v) MojoGui_rev##v
#define MOJOGUI_STRUCT MOJOGUI_STRUCT_VER(MOJOGUI_INTERFACE_REVISION)
typedef MOJOGUI_STRUCT MojoGui;

#ifdef __cplusplus
}
#endif

#endif

/* end of gui.h ... */


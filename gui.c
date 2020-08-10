/**
 * MojoSetup; a portable, flexible installation application.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#include "gui.h"
#include "platform.h"
#include "fileio.h"

typedef struct S_PLUGINLIST
{
    void *lib;
    const MojoGui *gui;
    MojoGuiPluginPriority priority;
    uint32 imglen;
    uint8 *img;
    struct S_PLUGINLIST *next;
} PluginList;

const MojoGui *GGui = NULL;
static PluginList *pluginDetails = NULL;

typedef struct StaticGuiPlugin
{
    const char* name;
    const MojoGuiEntryPoint entry;
} StaticGuiPlugin;

#define STATIC_GUI_PLUGIN(name) {#name, MojoGuiPlugin_##name}
static StaticGuiPlugin staticGui[] =
{
#if GUI_STATIC_LINK_STDIO
    STATIC_GUI_PLUGIN(stdio),
#endif
#if GUI_STATIC_LINK_COCOA
    STATIC_GUI_PLUGIN(cocoa),
#endif
#if GUI_STATIC_LINK_GTKPLUS3
    STATIC_GUI_PLUGIN(gtkplus3),
#endif
#if GUI_STATIC_LINK_GTKPLUS2
    STATIC_GUI_PLUGIN(gtkplus2),
#endif
#if GUI_STATIC_LINK_WWW
    STATIC_GUI_PLUGIN(www),
#endif
#if GUI_STATIC_LINK_NCURSES
    STATIC_GUI_PLUGIN(ncurses),
#endif
    {NULL, NULL}
};
#undef STATIC_GUI_PLUGIN


static MojoGuiPluginPriority calcGuiPriority(const MojoGui *gui)
{
    MojoGuiPluginPriority retval;

    retval = gui->priority(MojoPlatform_istty());

    // If the plugin isn't saying "don't try me at all" then see if the
    //  user explicitly wants this one.
    if (retval != MOJOGUI_PRIORITY_NEVER_TRY)
    {
        static const char *envr = NULL;
        if (envr == NULL)
            envr = cmdlinestr("ui", "MOJOSETUP_UI", NULL);
        if ((envr != NULL) && (strcasecmp(envr, gui->name()) == 0))
            retval = MOJOGUI_PRIORITY_USER_REQUESTED;
    } // if

    return retval;
} // calcGuiPriority


const MojoGui *MojoSetup_loadGuiPlugin(const uint8 *img, uint32 len, void **lib)
{
    *lib = MojoPlatform_dlopen(img, len);
    if (*lib != NULL)
    {
        void *addr = MojoPlatform_dlsym(*lib, MOJOGUI_ENTRY_POINT_STR);
        MojoGuiEntryPoint entry = (MojoGuiEntryPoint) addr;
        if (entry != NULL)
            return entry(MOJOGUI_INTERFACE_REVISION, &GEntryPoints);
    } // if

    return NULL;
} // MojoSetup_loadGuiPlugin


static PluginList *initGuiPluginsByPriority(PluginList *plugins)
{
    MojoGuiPluginPriority p;
    for (p = MOJOGUI_PRIORITY_USER_REQUESTED; p < MOJOGUI_PRIORITY_TOTAL; p++)
    {
        PluginList *i;
        for (i = plugins->next; i != NULL; i = i->next)
        {
            if (i->priority != p)
                continue;

            if (i->img != NULL)
                i->gui = MojoSetup_loadGuiPlugin(i->img, i->imglen, &i->lib);

            if (i->gui && i->gui->init())
            {
                logInfo("Selected '%0' UI.", i->gui->name());
                return i;
            } // if

            if (i->lib)
            {
                MojoPlatform_dlclose(i->lib);
                i->lib = NULL;
                i->gui = NULL;
            } // if
        } // for
    } // for

    return NULL;
} // initGuiPluginsByPriority


static void deleteGuiPlugin(PluginList *plugin)
{
    if (plugin != NULL)
    {
        if (plugin->gui)
            plugin->gui->deinit();
        if (plugin->lib)
            MojoPlatform_dlclose(plugin->lib);
        free(plugin->img);
        free(plugin);
    } // if
} // deleteGuiPlugin


static PluginList *addGuiPlugin(PluginList *plugins, const MojoGui *gui, MojoGuiPluginPriority priority)
{
    PluginList *plug = xmalloc(sizeof (PluginList));
    plug->lib = NULL;
    plug->gui = gui;
    plug->priority = priority;
    plug->next = plugins->next;
    plugins->next = plug;

    return plug;
} // addGuiPlugin


static void loadStaticGuiPlugins(PluginList *plugins)
{
    int i;
    for (i = 0; staticGui[i].entry != NULL; i++)
    {
        const MojoGui *gui;
        logInfo("Testing static GUI plugin %0", staticGui[i].name);
         gui = staticGui[i].entry(MOJOGUI_INTERFACE_REVISION, &GEntryPoints);
        if (gui != NULL)
            addGuiPlugin(plugins, gui, calcGuiPriority(gui));
    }
} // loadStaticGuiPlugins


static boolean loadDynamicGuiPlugin(PluginList *plugins, MojoArchive *ar)
{
    PluginList *plug = NULL;
    MojoInput *io = ar->openCurrentEntry(ar);
    if (io != NULL)
    {
        const uint32 imglen = (uint32) io->length(io);
        uint8 *img = (uint8 *) xmalloc(imglen);
        const uint32 br = (uint32) io->read(io, img, imglen);
        io->close(io);
        if (br == imglen)
        {
            MojoGuiPluginPriority priority;
            priority = MojoPlatform_getGuiPriority(img, imglen);
            if (priority != MOJOGUI_PRIORITY_NEVER_TRY)
            {
                plug = addGuiPlugin(plugins, NULL, priority);
                plug->img = img;
                plug->imglen = imglen;
            }
        } // if
    } // if

    return plug != NULL;
} // loadDynamicGuiPlugin


static void loadDynamicGuiPlugins(PluginList *plugins)
{
    if (GBaseArchive->enumerate(GBaseArchive))
    {
        const MojoArchiveEntry *entinfo;
        while ((entinfo = GBaseArchive->enumNext(GBaseArchive)) != NULL)
        {
            if (entinfo->type != MOJOARCHIVE_ENTRY_FILE)
                continue;
            else if (strncmp(entinfo->filename, "guis/", 5) != 0)
                continue;

            logInfo("Testing dynamic GUI plugin %0", entinfo->filename);
            loadDynamicGuiPlugin(plugins, GBaseArchive);
        } // while
    } // if
} // loadDynamicGuiPlugins


const MojoGui *MojoGui_initGuiPlugin(void)
{
    PluginList plugins;
    PluginList *i = NULL;

    if (pluginDetails != NULL)
        return pluginDetails->gui;

    memset(&plugins, '\0', sizeof (plugins));
    assert(GGui == NULL);

    loadDynamicGuiPlugins(&plugins);
    loadStaticGuiPlugins(&plugins);

    pluginDetails = initGuiPluginsByPriority(&plugins);

    // cleanout unused plugins...
    i = plugins.next;
    while (i != NULL)
    {
        PluginList *next = i->next;
        if (i != pluginDetails)
            deleteGuiPlugin(i);
        i = next;
    } // while

    if (pluginDetails != NULL)
    {
        GGui = pluginDetails->gui;
        pluginDetails->next = NULL;
    } // if

    return GGui;
} // MojoGui_findGuiPlugin


void MojoGui_deinitGuiPlugin(void)
{
    GGui = NULL;
    deleteGuiPlugin(pluginDetails);
    pluginDetails = NULL;
} // MojoGui_deinitGuiPlugin

// end of gui.c ...


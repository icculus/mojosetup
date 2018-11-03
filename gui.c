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

static const MojoGuiEntryPoint staticGui[] =
{
#if GUI_STATIC_LINK_STDIO
    MojoGuiPlugin_stdio,
#endif
#if GUI_STATIC_LINK_COCOA
    MojoGuiPlugin_cocoa,
#endif
#if GUI_STATIC_LINK_GTKPLUS2
    MojoGuiPlugin_gtkplus2,
#endif
#if GUI_STATIC_LINK_WWW
    MojoGuiPlugin_www,
#endif
#if GUI_STATIC_LINK_NCURSES
    MojoGuiPlugin_ncurses,
#endif
    NULL
};


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
            {
                i->lib = MojoPlatform_dlopen(i->img, i->imglen);
                if (i->lib != NULL)
                {
                    void *addr = MojoPlatform_dlsym(i->lib, MOJOGUI_ENTRY_POINT_STR);
                    MojoGuiEntryPoint entry = (MojoGuiEntryPoint) addr;
                    if (entry != NULL)
                        i->gui = entry(MOJOGUI_INTERFACE_REVISION, &GEntryPoints);
                } // if
            } // if

            if (i->gui && i->gui->init())
            {
                logInfo("Selected '%0' UI.", i->gui->name());
                return i;
            } // if

            if (i->lib)
            {
                MojoPlatform_dlclose(i->lib);
                i->lib = NULL;
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


static PluginList *tryGuiPlugin(PluginList *plugins, MojoGuiEntryPoint entry)
{
    PluginList *plug = NULL;
    const MojoGui *gui = entry(MOJOGUI_INTERFACE_REVISION, &GEntryPoints);
    if (gui != NULL)
    {
        plug = xmalloc(sizeof (PluginList));
        plug->lib = NULL;
        plug->gui = gui;
        plug->priority = calcGuiPriority(gui);
        plug->next = plugins->next;
        plugins->next = plug;
    } // if

    return plug;
} // tryGuiPlugin


static void loadStaticGuiPlugins(PluginList *plugins)
{
    int i;
    for (i = 0; staticGui[i] != NULL; i++)
        tryGuiPlugin(plugins, staticGui[i]);
} // loadStaticGuiPlugins


static boolean loadDynamicGuiPlugin(PluginList *plugins, MojoArchive *ar)
{
    PluginList *plug = NULL;
    MojoInput *io = ar->openCurrentEntry(ar);
    if (io != NULL)
    {
        void *lib = NULL;
        const uint32 imglen = (uint32) io->length(io);
        uint8 *img = (uint8 *) xmalloc(imglen);
        const uint32 br = (uint32) io->read(io, img, imglen);
        io->close(io);
        if (br == imglen)
        {
            lib = MojoPlatform_dlopen(img, imglen);
            if (lib != NULL)
            {
                void *addr = MojoPlatform_dlsym(lib, MOJOGUI_ENTRY_POINT_STR);
                MojoGuiEntryPoint entry = (MojoGuiEntryPoint) addr;
                if (entry != NULL)
                {
                    plug = tryGuiPlugin(plugins, entry);
                    if (plug)
                    {
                        plug->img = img;
                        plug->imglen = imglen;
                    } // if
                } // if

                // always close, because GTK+2 and GTK+3 can't coexist.
                //  we'll reload them when trying them!
                MojoPlatform_dlclose(lib);
            } // if
        } // if

        if (!plug)
            free(img);
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


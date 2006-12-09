#include "gui.h"
#include "platform.h"
#include "fileio.h"

// !!! FIXME: None of these should be here.
#include <time.h>
#include <sys/time.h>

typedef struct S_PLUGINLIST
{
    char *filename;
    void *lib;
    const MojoGui *gui;
    MojoGuiPluginPriority priority;
    struct S_PLUGINLIST *next;
} PluginList;

const MojoGui *GGui = NULL;
static PluginList *pluginDetails = NULL;

static const MojoGuiEntryPoint staticGui[] =
{
#if GUI_STATIC_LINK_STDIO
    MojoGuiPlugin_stdio,
#endif
#if GUI_STATIC_LINK_WINDOWS
    MojoGuiPlugin_windows,
#endif
#if GUI_STATIC_LINK_MACOSX
    MojoGuiPlugin_macosx,
#endif
#if GUI_STATIC_LINK_GTK_PLUS
    MojoGuiPlugin_gtkplus,
#endif
    NULL
};


static MojoGuiPluginPriority calcGuiPriority(const MojoGui *gui)
{
    static char *envr = NULL;
    MojoGuiPluginPriority retval;

    if (envr == NULL)
        envr = getenv("MOJOSETUP_GUI");

    retval = gui->priority();

    // If the plugin isn't saying "don't try me at all" then see if the
    //  user explicitly wants this one.
    if (retval != MOJOGUI_PRIORITY_NEVER_TRY)
    {
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
            if ( (i->priority == p) && (i->gui->init()) )
                return i;
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
        if (plugin->filename)
            MojoPlatform_unlink(plugin->filename);
        free(plugin->filename);
        free(plugin);
    } // if
} // deleteGuiPlugin


static boolean tryGuiPlugin(PluginList *plugins, const char *fname,
                            MojoGuiEntryPoint entry)
{
    boolean retval = false;
    const MojoGui *gui = entry(MOJOGUI_INTERFACE_REVISION, &GEntryPoints);
    if (gui != NULL)
    {
        PluginList *plug = xmalloc(sizeof (PluginList));
        plug->lib = NULL;
        plug->gui = gui;
        plug->priority = calcGuiPriority(gui);
        plug->filename = ((fname != NULL) ? xstrdup(fname) : NULL);
        plug->next = plugins->next;
        plugins->next = plug;
        retval = true;
    } // if

    return retval;
} // tryGuiPlugin


static void loadStaticGuiPlugins(PluginList *plugins)
{
    int i;
    for (i = 0; staticGui[i] != NULL; i++)
        tryGuiPlugin(plugins, NULL, staticGui[i]);
} // loadStaticGuiPlugins


static boolean loadDynamicGuiPlugin(PluginList *plugins, MojoArchive *ar)
{
    char fname[128] = { 0 };
    void *lib = NULL;
    boolean rc;
    MojoInput *io = ar->openCurrentEntry(ar);
    if (io == NULL)
        return false;

    STUBBED("Don't copy if it's a physical file already?");

    STUBBED("Filename creation has to change");
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        snprintf(fname, sizeof (fname), "/tmp/mojosetup-gui-%ld.so", tv.tv_sec);
    }

    rc = mojoInputToPhysicalFile(io, fname);
    io->close(io);

    if (rc)
    {
        rc = false;
        lib = MojoPlatform_dlopen(fname);
        if (lib != NULL)
        {
            void *addr = MojoPlatform_dlsym(lib, MOJOGUI_ENTRY_POINT_STR);
            MojoGuiEntryPoint entry = (MojoGuiEntryPoint) addr;
            if (entry != NULL)
                rc = tryGuiPlugin(plugins, fname, entry);
        } // if
    } // if

    if (!rc)
    {
        if (lib != NULL)
            MojoPlatform_dlclose(lib);
        if (fname[0])
            MojoPlatform_unlink(fname);
    } // if

    return rc;
} // loadDynamicGuiPlugin


static void loadDynamicGuiPlugins(PluginList *plugins)
{
    if (GBaseArchive->enumerate(GBaseArchive, "gui"))
    {
        const MojoArchiveEntryInfo *entinfo;
        while ((entinfo = GBaseArchive->enumNext(GBaseArchive)) != NULL)
        {
            if (entinfo->type != MOJOARCHIVE_ENTRY_FILE)
                continue;

            loadDynamicGuiPlugin(plugins, GBaseArchive);
        } // while
    } // if
} // loadDynamicGuiPlugins


const MojoGui *MojoGui_initGuiPlugin(void)
{
    PluginList plugins;

    if (pluginDetails != NULL)
        return pluginDetails->gui;

    memset(&plugins, '\0', sizeof (plugins));
    assert(GGui == NULL);

    loadDynamicGuiPlugins(&plugins);
    loadStaticGuiPlugins(&plugins);

    pluginDetails = initGuiPluginsByPriority(&plugins);

    // cleanout unused plugins...
    PluginList *i = plugins.next;
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


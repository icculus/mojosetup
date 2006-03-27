#include "gui.h"
#include "fileio.h"

// !!! FIXME: None of these should be here.
#include <dlfcn.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

typedef struct S_PLUGINLIST
{
    char *filename;
    void *lib;
    MojoGui *gui;
    MojoGuiPluginPriority priority;
    struct S_PLUGINLIST *next;
} PluginList;

MojoGui *GGui = NULL;
PluginList *pluginDetails = NULL;


typedef MojoGui* (*MojoGuiStaticEntryPoint)(void);
MojoGui *MojoGuiPlugin_stdio(void);

static const MojoGuiStaticEntryPoint staticGuiPlugins[] =
{
    MojoGuiPlugin_stdio,
};


static MojoGuiPluginPriority calcGuiPriority(MojoGui *gui)
{
    static char *envr = NULL;
    MojoGuiPluginPriority retval;

    if (envr == NULL)
        envr = getenv("MOJOSETUP_GUI");

    retval = gui->priority(gui);

    // If the plugin isn't saying "don't try me at all" then see if the
    //  user explicitly wants this one.
    if (retval != MOJOGUI_PRIORITY_NEVER_TRY)
    {
        if ((envr != NULL) && (strcasecmp(envr, gui->name(gui)) == 0))
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
            if ( (i->priority == p) && (i->gui->init(i->gui)) )
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
            plugin->gui->deinit(plugin->gui);
        if (plugin->lib)
            dlclose(plugin->lib);
        if (plugin->filename)
            unlink(plugin->filename);
        free(plugin->filename);
        free(plugin);
    } // if
} // deleteGuiPlugin


static void loadStaticGuiPlugins(PluginList *plugins)
{
    int i;
    for (i = 0; i < STATICARRAYLEN(staticGuiPlugins); i++)
    {
        PluginList *plug;
        MojoGui *gui = staticGuiPlugins[i]();
        if (gui == NULL)
            continue;
        plug = xmalloc(sizeof (PluginList));
        plug->lib = NULL;
        plug->gui = gui;
        plug->priority = calcGuiPriority(gui);
        plug->next = plugins->next;
        plugins->next = plug;
    } // for
} // loadStaticGuiPlugins


static PluginList *loadDynamicGuiPlugin(MojoArchive *ar)
{
    char fname[128] = { 0 };
    PluginList *retval = NULL;
    void *lib = NULL;
    MojoGuiEntryType entry = NULL;
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
        lib = dlopen(fname, RTLD_NOW | RTLD_GLOBAL);
        STUBBED("abstract out dlopen!");
        if (lib != NULL)
        {
            MojoGui *gui = NULL;
            entry = (MojoGuiEntryType) dlsym(lib, MOJOGUI_ENTRY_POINT_STR);
            if ( (entry != NULL) && ((gui = entry()) != NULL) )
            {
                retval = xmalloc(sizeof (PluginList));
                retval->filename = xstrdup(fname);
                retval->lib = lib;
                retval->gui = gui;
                retval->priority = calcGuiPriority(gui);
                retval->next = NULL;
            } // if
        } // if
    } // if

    if (!retval)
    {
        if (lib != NULL)
            dlclose(lib);
        if (fname[0])
            unlink(fname);
    } // if

    return retval;
} // loadDynamicGuiPlugin


static void loadDynamicGuiPlugins(PluginList *plugins)
{
    if (GBaseArchive->enumerate(GBaseArchive, "gui"))
    {
        const MojoArchiveEntryInfo *entinfo;
        while ((entinfo = GBaseArchive->enumNext(GBaseArchive)) != NULL)
        {
            PluginList *item;

            if (entinfo->type != MOJOARCHIVE_ENTRY_FILE)
                continue;

            item = loadDynamicGuiPlugin(GBaseArchive);
            if (item == NULL)
                continue;

            item->next = plugins->next;
            plugins->next = item;
        } // while
    } // if
} // loadDynamicGuiPlugins


MojoGui *MojoGui_initGuiPlugin(void)
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


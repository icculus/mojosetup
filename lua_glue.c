#include "universal.h"
#include "platform.h"
#include "fileio.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define MOJOSETUP_NAMESPACE "MojoSetup"

static lua_State *luaState = NULL;

// Read data from a MojoInput when loading Lua code.
static const char *MojoLua_reader(lua_State *L, void *data, size_t *size)
{
    MojoInput *in = (MojoInput *) data;
    const char *retval = (const char *) scratchbuf_128k;
    int64 br = in->read(in, scratchbuf_128k, sizeof (scratchbuf_128k));
    if (br <= 0)  // eof or error? (lua doesn't care which?!)
    {
        br = 0;
        retval = NULL;
    } // if

    *size = (size_t) br;
    return retval;
} // MojoLua_reader


boolean MojoLua_runFile(const char *basefname)
{
    boolean retval = false;
    size_t alloclen = strlen(basefname) + 16;
    char *fname = (char *) xmalloc(alloclen);
    int rc = 0;
    MojoInput *io = NULL;

    // !!! FIXME: change this API to accept multiple filenames to search for,
    // !!! FIXME:  since two full enumerations to discover the filename is
    // !!! FIXME:  missing is just wasteful.
    STUBBED("See FIXME above.");
    snprintf(fname, alloclen, "lua/%s.luac", basefname);
    io = MojoInput_newFromArchivePath(GBaseArchive, fname);

    #if !DISABLE_LUA_PARSER
    if (io == NULL)
    {
        snprintf(fname, alloclen, "lua/%s.lua", basefname);
        io = MojoInput_newFromArchivePath(GBaseArchive, fname);
    } // if
    #endif

    if (io != NULL)
    {
        rc = lua_load(luaState, MojoLua_reader, io, fname);
        io->close(io);

        if (rc != 0)
            lua_error(luaState);
        else
        {
            // !!! FIXME: use pcall instead so we can get error backtraces and localize.
            // Call new chunk on top of the stack (lua_call will pop it off).
            lua_call(luaState, 0, 0);  // return values are dumped.
            retval = true;   // if this didn't panic, we succeeded.
        } // if
    } // if

    free(fname);
    return retval;
} // MojoLua_runFile


void MojoLua_collectGarbage(void)
{
    lua_State *L = luaState;
    uint32 ticks = 0;
    int pre = 0;
    int post = 0;

    STUBBED("logging!");
    pre = (lua_gc(L, LUA_GCCOUNT, 0) * 1024) + lua_gc(L, LUA_GCCOUNTB, 0);
    printf("Collecting garbage (currently using %d bytes).\n", pre);
    ticks = MojoPlatform_ticks();
    lua_gc (L, LUA_GCCOLLECT, 0);
    ticks = MojoPlatform_ticks() - ticks;
    printf("Collection finished (took %d milliseconds).\n", (int) ticks);
    post = (lua_gc(L, LUA_GCCOUNT, 0) * 1024) + lua_gc(L, LUA_GCCOUNTB, 0);
    printf("Now using %d bytes (%d bytes savings).\n", post, pre - post);
} // MojoLua_collectGarbage


// Since localization is kept in Lua tables, I stuck this in the Lua glue.
const char *translate(const char *str)
{
    const char *retval = str;
    int popcount = 0;

    if (lua_checkstack(luaState, 3))
    {
        lua_getglobal(luaState, MOJOSETUP_NAMESPACE); popcount++;
        if (lua_istable(luaState, -1))  // namespace is sane?
        {
            lua_getfield(luaState, -1, "translations"); popcount++;
            if (lua_istable(luaState, -1))  // translation table is sane?
            {
                const char *tr = NULL;
                lua_getfield(luaState, -1, str); popcount++;
                tr = lua_tostring(luaState, -1);
                if (tr != NULL)  // translated for this locale?
                {
                    xstrncpy(scratchbuf_128k, tr, sizeof(scratchbuf_128k));
                    retval = scratchbuf_128k;
                } // if
            } // if
        } // if
    } // if

    lua_pop(luaState, popcount);   // remove our stack salsa.
    return retval;
} // translate


// Hook into our error handling in case Lua throws a runtime error.
static int MojoLua_panic(lua_State *L)
{
    const char *errstr = lua_tostring(L, 1);
    if (errstr == NULL)
        errstr = _("Unknown Lua error");
    return fatal(errstr);  // doesn't actually return.
} // MojoLua_panic


// Lua interface to MojoLua_runFile(). This is needed instead of Lua's
//  require(), since it can access scripts inside an archive.
static int luahook_runfile(lua_State *L)
{
    const char *fname = luaL_checkstring(L, 1);
    lua_pushboolean(L, MojoLua_runFile(fname));
    return 1;
} // luahook_runfile


// Lua interface to translate().
static int luahook_translate(lua_State *L)
{
    const char *str = luaL_checkstring(L, 1);
    lua_pushstring(L, translate(str));
    return 1;
} // luahook_translate


// Sets t[sym]=f, where t is on the top of the Lua stack.
static inline void set_cfunc(lua_State *L, lua_CFunction f, const char *sym)
{
    lua_pushcfunction(luaState, f);
    lua_setfield(luaState, -2, sym);
} // set_cfunc


// Sets t[sym]=f, where t is on the top of the Lua stack.
static inline void set_string(lua_State *L, const char *str, const char *sym)
{
    lua_pushstring(luaState, str);
    lua_setfield(luaState, -2, sym);
} // set_string


boolean MojoLua_initLua(void)
{
    char locale[16];
    char ostype[64];
    char osversion[64];

    if (!MojoPlatform_locale(locale, sizeof (locale)))
        xstrncpy(locale, "???", sizeof (locale));
    if (!MojoPlatform_osType(ostype, sizeof (ostype)))
        xstrncpy(ostype, "???", sizeof (ostype));
    if (!MojoPlatform_osVersion(osversion, sizeof (osversion)))
        xstrncpy(osversion, "???", sizeof (osversion));

    assert(luaState == NULL);

    luaState = luaL_newstate();   // !!! FIXME: define our own allocator?
    if (luaState == NULL)
        return false;

    lua_atpanic(luaState, MojoLua_panic);

    if (!lua_checkstack(luaState, 20))  // Just in case.
    {
        lua_close(luaState);
        luaState = NULL;
        return false;
    } // if

    luaL_openlibs(luaState);

    // Build MojoSetup namespace for Lua to access and fill in C bridges...
    lua_newtable(luaState);
        // Set up initial C functions, etc we want to expose to Lua code...
        set_cfunc(luaState, luahook_runfile, "runfile");
        set_cfunc(luaState, luahook_translate, "translate");
        set_string(luaState, locale, "locale");
        set_string(luaState, PLATFORM_NAME, "platform");
        set_string(luaState, PLATFORM_ARCH, "arch");
        set_string(luaState, ostype, "ostype");
        set_string(luaState, osversion, "osversion");
    lua_setglobal(luaState, "MojoSetup");

    // Set up localization table, if possible.
    MojoLua_runFile("localization");

    // Transfer control to Lua to setup some APIs and state...
    if (!MojoLua_runFile("mojosetup_init"))
        return false;

    // ...and run the installer-specific config file.
    if (!MojoLua_runFile("config"))
        return false;

    // We don't need the MojoSetup.schema namespace anymore. Make it
    //  eligible for garbage collection.
    lua_getglobal(luaState, MOJOSETUP_NAMESPACE);
    if (lua_istable(luaState, -1))
    {
        lua_pushnil(luaState);
        lua_setfield(luaState, -2, "schema");
    } // if
    lua_pop(luaState, 1);

    MojoLua_collectGarbage();  // get rid of old init crap we don't need.

    return true;
} // MojoLua_initLua


void MojoLua_deinitLua(void)
{
    if (luaState != NULL)
    {
        lua_close(luaState);
        luaState = NULL;
    } // if
} // MojoLua_deinitLua

// end of lua_glue.c ...


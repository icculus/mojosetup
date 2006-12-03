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
    int pre = 0;
    int post = 0;

    STUBBED("logging!");
    pre = (lua_gc(L, LUA_GCCOUNT, 0) * 1024) + lua_gc(L, LUA_GCCOUNTB, 0);
    printf("Collecting garbage (currently using %d bytes).\n", pre);
    lua_gc (L, LUA_GCCOLLECT, 0);
    printf("Collection finished (took ??? milliseconds).\n"); // !!! FIXME
    post = (lua_gc(L, LUA_GCCOUNT, 0) * 1024) + lua_gc(L, LUA_GCCOUNTB, 0);
    printf("Now using %d bytes (%d bytes savings).\n", post, pre - post);
} // MojoLua_collectGarbage


// Since localization is kept in Lua tables, I stuck this in the Lua glue.
const char *translate(const char *str)
{
    const char *retval = str;
    int popcount = 0;

    if (!lua_checkstack(luaState, 5))
        return str;

    lua_getglobal(luaState, MOJOSETUP_NAMESPACE); popcount++;
    if (lua_istable(luaState, -1))  // namespace is sane?
    {
        lua_getfield(luaState, -1, "translate"); popcount++;
        if (lua_isfunction(luaState, -1))
        {
            const char *tr = NULL;
            lua_pushstring(luaState, str);
            lua_call(luaState, 1, 1);  // popcount ends up the same...
            tr = lua_tostring(luaState, -1);
            if (tr != NULL)
            {
                xstrncpy(scratchbuf_128k, tr, sizeof(scratchbuf_128k));
                retval = scratchbuf_128k;
            } // if
        } // if
    } // if

    lua_pop(luaState, popcount);   // remove our stack salsa.
    return retval;
} // translate


// Hook into our error handling in case Lua throws a runtime error.
static int MojoLua_panic(lua_State *L)
{
    int argc = lua_gettop(L);
    const char *errstr = _("Unknown Lua error");
    if (argc > 0)
        errstr = lua_tostring(L, 1);
    return fatal(errstr);  // doesn't actually return.
} // MojoLua_panic


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
    assert(luaState == NULL);

    if (!MojoPlatform_locale(locale, sizeof (locale)))
        xstrncpy(locale, "???", sizeof (locale));

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
        set_string(luaState, locale, "locale");
    lua_setglobal(luaState, "MojoSetup");

    // Set up localization table, if possible.
    MojoLua_runFile("localization");

    // Transfer control to Lua to setup some APIs and state...
    if (!MojoLua_runFile("mojosetup_init"))
        return false;

    // ...and run the installer-specific config file.
    if (!MojoLua_runFile("config"))
        return false;

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


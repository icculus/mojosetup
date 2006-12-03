#include "universal.h"
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


// Since localization is kept in Lua tables, I stuck this in the Lua glue.
const char *translate(const char *str)
{
    const char *retval = str;
    int popcount = 0;

    if (lua_checkstack(luaState, 4))
    {
        lua_getglobal(luaState, MOJOSETUP_NAMESPACE); popcount++;
        if (lua_istable(luaState, -1))  // namespace is sane?
        {
            lua_getfield(luaState, -1, "translations"); popcount++;
            if (lua_istable(luaState, -1))  // translation table is sane?
            {
                lua_getfield(luaState, -1, str); popcount++;
                if (lua_istable(luaState, -1))  // translation exists at all?
                {
                    const char *tr = NULL;
                    lua_getfield(luaState, -1, GLocale); popcount++;
                    tr = lua_tostring(luaState, -1);
                    if (tr != NULL)  // translated for this locale?
                    {
                        xstrncpy(scratchbuf_128k, tr, sizeof(scratchbuf_128k));
                        retval = scratchbuf_128k;
                    } // if
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
    int argc = lua_gettop(L);
    const char *errstr = _("Unknown Lua error");
    if (argc > 0)
        errstr = lua_tostring(L, 1);
    return fatal(errstr);  // doesn't actually return.
} // MojoLua_panic


static int MojoLua_translate(lua_State *L)
{
    int argc = lua_gettop(L);
    int i;
    for (i = 1; i <= argc; i++)
    {
        const char *str = lua_tostring(L, i);
        if (str == NULL)
        {
            lua_pushstring(L, _("Argument not a string in '"
                               MOJOSETUP_NAMESPACE ".translate'"));
            return lua_error(L);
        } // if

        lua_pushstring(L, translate(str));
    } // for

    return argc;
} // MojoLua_translate


static int MojoLua_locale(lua_State *L)
{
    lua_pushstring(L, GLocale);
    return 1;
} // MojoLua_translate


// Sets t[sym]=f, where t is on the top of the Lua stack.
static inline void set_cfunc(lua_State *L, lua_CFunction f, const char *sym)
{
    lua_pushcfunction(luaState, f);
    lua_setfield(luaState, -2, sym);
} // set_cfunc


boolean MojoLua_initLua(void)
{
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
        // Set up C functions we want to expose to Lua code...
        set_cfunc(luaState, MojoLua_translate, "translate");
        set_cfunc(luaState, MojoLua_locale, "locale");
    lua_setglobal(luaState, "MojoSetup");

    // Set up localization table, if possible.
    MojoLua_runFile("translations");

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


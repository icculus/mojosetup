#include "universal.h"
#include "lua_glue.h"
#include "platform.h"
#include "fileio.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "gui.h"

#define MOJOSETUP_NAMESPACE "MojoSetup"

static lua_State *luaState = NULL;

// Allocator interface for internal Lua use.
static void *MojoLua_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    if (nsize == 0)
    {
        free(ptr);
        return NULL;
    } // if
    return xrealloc(ptr, nsize);
} // MojoLua_alloc


// Read data from a MojoInput when loading Lua code.
static const char *MojoLua_reader(lua_State *L, void *data, size_t *size)
{
    MojoInput *in = (MojoInput *) data;
    char *retval = (char *) scratchbuf_128k;
    int64 br = in->read(in, scratchbuf_128k, sizeof (scratchbuf_128k));
    if (br <= 0)  // eof or error? (lua doesn't care which?!)
    {
        br = 0;
        retval = NULL;
    } // if

    *size = (size_t) br;
    return retval;
} // MojoLua_reader


static inline int snprintfcat(char **ptr, size_t *len, const char *fmt, ...)
{
    int bw = 0;
    va_list ap;
    va_start(ap, fmt);
    bw = vsnprintf(*ptr, *len, fmt, ap);
    va_end(ap);
    *ptr += bw;
    *len -= bw;
    return bw;
} // snprintfcat


static int luahook_stackwalk(lua_State *L)
{
    const char *errstr = lua_tostring(L, 1);
    lua_Debug ldbg;
    int i = 0;

    if (errstr != NULL)
        logDebug("%s\n", errstr);

    logDebug("Lua stack backtrace:");

    // start at 1 to skip this function.
    for (i = 1; lua_getstack(L, i, &ldbg); i++)
    {
        char *ptr = (char *) scratchbuf_128k;
        size_t len = sizeof (scratchbuf_128k);
        int bw = snprintfcat(&ptr, &len, "#%d", i-1);
        const int maxspacing = 4;
        int spacing = maxspacing - bw;
        while (spacing-- > 0)
            snprintfcat(&ptr, &len, " ");

        if (!lua_getinfo(L, "nSl", &ldbg))
        {
            snprintfcat(&ptr, &len, "???\n");
            logDebug((const char *) scratchbuf_128k);
            continue;
        } // if

        if (ldbg.namewhat[0])
            snprintfcat(&ptr, &len, "%s ", ldbg.namewhat);

        if ((ldbg.name) && (ldbg.name[0]))
            snprintfcat(&ptr, &len, "function %s ()", ldbg.name);
        else
        {
            if (strcmp(ldbg.what, "main") == 0)
                snprintfcat(&ptr, &len, "mainline of chunk");
            else if (strcmp(ldbg.what, "tail") == 0)
                snprintfcat(&ptr, &len, "tail call");
            else
                snprintfcat(&ptr, &len, "unidentifiable function");
        } // if

        logDebug((const char *) scratchbuf_128k);
        ptr = (char *) scratchbuf_128k;
        len = sizeof (scratchbuf_128k);

        for (spacing = 0; spacing < maxspacing; spacing++)
            snprintfcat(&ptr, &len, " ");

        if (strcmp(ldbg.what, "C") == 0)
            snprintfcat(&ptr, &len, "in native code");
        else if (strcmp(ldbg.what, "tail") == 0)
            snprintfcat(&ptr, &len, "in Lua code");
        else
        {
            snprintfcat(&ptr, &len, "in Lua code at %s", ldbg.source);
            if (ldbg.currentline != -1)
                snprintfcat(&ptr, &len, ":%d", ldbg.currentline);
        } // else
        logDebug((const char *) scratchbuf_128k);
    } // for

    lua_pushstring(L, errstr ? errstr : "");
    return 1;
} // luahook_stackwalk


boolean MojoLua_runFile(const char *basefname)
{
    MojoArchive *ar = GBaseArchive;   // in case we want to generalize later.
    const MojoArchiveEntryInfo *entinfo;
    boolean retval = false;
    char clua[128];  // compiled filename.
    char ulua[128];  // uncompiled filename.
    int rc = 0;
    MojoInput *io = NULL;

    if (snprintf(clua, sizeof (clua), "%s.luac", basefname) >= sizeof (clua))
        return false;

    if (snprintf(ulua, sizeof (ulua), "%s.lua", basefname) >= sizeof (ulua))
        return false;

    if (ar->enumerate(ar, "lua"))
    {
        while ((entinfo = ar->enumNext(ar)) != NULL)
        {
            boolean match = (strcmp(entinfo->filename, clua) == 0);
            #if !DISABLE_LUA_PARSER
            if (!match)
                match = (strcmp(entinfo->filename, ulua) == 0);
            #endif

            if (match)
            {
                if (entinfo->type == MOJOARCHIVE_ENTRY_FILE)
                    io = ar->openCurrentEntry(ar);
                break;
            } // if
        } // while
    } // if

    if (io != NULL)
    {
        lua_pushcfunction(luaState, luahook_stackwalk);
        rc = lua_load(luaState, MojoLua_reader, io, entinfo->filename);
        io->close(io);

        if (rc != 0)
            lua_error(luaState);
        else
        {
            // Call new chunk on top of the stack (lua_call will pop it off).
            if (lua_pcall(luaState, 0, 0, -2) != 0)  // retvals are dumped.
                lua_error(luaState);   // error on stack has debug info.
            else
                retval = true;   // if this didn't panic, we succeeded.
        } // if
        lua_pop(luaState, 1);   // dump stackwalker.
    } // if

    return retval;
} // MojoLua_runFile


void MojoLua_collectGarbage(void)
{
    lua_State *L = luaState;
    uint32 ticks = 0;
    int pre = 0;
    int post = 0;

    pre = (lua_gc(L, LUA_GCCOUNT, 0) * 1024) + lua_gc(L, LUA_GCCOUNTB, 0);
    logDebug("Collecting garbage (currently using %d bytes).", pre);
    ticks = MojoPlatform_ticks();
    lua_gc (L, LUA_GCCOLLECT, 0);
    profile("Garbage collection", ticks);
    post = (lua_gc(L, LUA_GCCOUNT, 0) * 1024) + lua_gc(L, LUA_GCCOUNTB, 0);
    logDebug("Now using %d bytes (%d bytes savings).\n", post, pre - post);
} // MojoLua_collectGarbage


static inline void pushStringOrNil(lua_State *L, const char *str)
{
    if (str != NULL)
        lua_pushstring(L, str);
    else
        lua_pushnil(L);
} // pushStringOrNil


// Since localization is kept in Lua tables, I stuck this in the Lua glue.
const char *translate(const char *str)
{
    const char *retval = str;

    if (luaState != NULL)  // No translations before Lua is initialized.
    {
        if (lua_checkstack(luaState, 3))
        {
            int popcount = 0;
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
                        char *dst = (char *) scratchbuf_128k;
                        xstrncpy(dst, tr, sizeof(scratchbuf_128k));
                        retval = dst;
                    } // if
                } // if
            } // if
            lua_pop(luaState, popcount);   // remove our stack salsa.
        } // if
    } // if

    return retval;
} // translate


// Hook into our error handling in case Lua throws a runtime error.
static int luahook_panic(lua_State *L)
{
    const char *errstr = lua_tostring(L, 1);
    if (errstr == NULL)
        errstr = _("Unknown error");
    return fatal(errstr);  // doesn't actually return.
} // luahook_panic


// Use this instead of Lua's error() function if you don't have a
//  programatic error, so you don't get stack callback stuff:
// MojoSetup.fatal("You need the base game to install this expansion pack.")
//  Doesn't actually return.
static int luahook_fatal(lua_State *L)
{
    const char *err = luaL_checkstring(L, 1);
    fatal(err);
    return 0;
} // luahook_fatal


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


static int luahook_ticks(lua_State *L)
{
    lua_pushnumber(L, MojoPlatform_ticks());
    return 1;
} // luahook_ticks


static int luahook_msgbox(lua_State *L)
{
    if (GGui != NULL)
    {
        const char *title = luaL_checkstring(L, 1);
        const char *text = luaL_checkstring(L, 2);
        GGui->msgbox(title, text);
    } // if
    return 0;
} // luahook_msgbox


static int luahook_promptyn(lua_State *L)
{
    boolean rc = false;
    if (GGui != NULL)
    {
        const char *title = luaL_checkstring(L, 1);
        const char *text = luaL_checkstring(L, 2);
        rc = GGui->promptyn(title, text);
    } // if

    lua_pushboolean(L, rc);
    return 1;
} // luahook_msgbox


static int luahook_logwarning(lua_State *L)
{
    logWarning(luaL_checkstring(L, 1));
    return 0;
} // luahook_logwarning


static int luahook_logerror(lua_State *L)
{
    logError(luaL_checkstring(L, 1));
    return 0;
} // luahook_logerror


static int luahook_loginfo(lua_State *L)
{
    logInfo(luaL_checkstring(L, 1));
    return 0;
} // luahook_loginfo


static int luahook_logdebug(lua_State *L)
{
    logDebug(luaL_checkstring(L, 1));
    return 0;
} // luahook_logdebug


static int luahook_cmdline(lua_State *L)
{
    const char *arg = luaL_checkstring(L, 1);
    lua_pushboolean(L, cmdline(arg));
    return 1;
} // luahook_cmdline


static int luahook_cmdlinestr(lua_State *L)
{
    int argc = lua_gettop(L);
    const char *arg = luaL_checkstring(L, 1);
    const char *envr = (argc < 2) ? NULL : lua_tostring(L, 2);
    const char *deflt = (argc < 3) ? NULL : lua_tostring(L, 3);
    pushStringOrNil(L, cmdlinestr(arg, envr, deflt));
    return 1;
} // luahook_cmdlinestr


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

static inline void set_string_array(lua_State *L, int argc, const char **argv,
                                    const char *sym)
{
    int i;
    lua_newtable(luaState);
    for (i = 0; i < argc; i++)
    {
        lua_pushinteger(luaState, i+1);  // lua is option base 1!
        lua_pushstring(luaState, argv[i]);
        lua_settable(luaState, -3);
    } // for
    lua_setfield(luaState, -2, sym);
} // set_string_array


void MojoLua_setString(const char *str, const char *sym)
{
    lua_getglobal(luaState, MOJOSETUP_NAMESPACE);
    set_string(luaState, str, sym);
    lua_pop(luaState, 1);
} // MojoLua_setString


void MojoLua_setStringArray(int argc, const char **argv, const char *sym)
{
    lua_getglobal(luaState, MOJOSETUP_NAMESPACE);
    set_string_array(luaState, argc, argv, sym);
    lua_pop(luaState, 1);
} // MojoLua_setStringArray


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

    luaState = lua_newstate(MojoLua_alloc, NULL);
    if (luaState == NULL)
        return false;

    lua_atpanic(luaState, luahook_panic);

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
        set_cfunc(luaState, luahook_ticks, "ticks");
        set_cfunc(luaState, luahook_fatal, "fatal");
        set_cfunc(luaState, luahook_msgbox, "msgbox");
        set_cfunc(luaState, luahook_promptyn, "promptyn");
        set_cfunc(luaState, luahook_stackwalk, "stackwalk");
        set_cfunc(luaState, luahook_logwarning, "logwarning");
        set_cfunc(luaState, luahook_logerror, "logerror");
        set_cfunc(luaState, luahook_loginfo, "loginfo");
        set_cfunc(luaState, luahook_logdebug, "logdebug");
        set_cfunc(luaState, luahook_cmdline, "cmdline");
        set_cfunc(luaState, luahook_cmdlinestr, "cmdlinestr");
        set_string(luaState, locale, "locale");
        set_string(luaState, PLATFORM_NAME, "platform");
        set_string(luaState, PLATFORM_ARCH, "arch");
        set_string(luaState, ostype, "ostype");
        set_string(luaState, osversion, "osversion");
        set_string(luaState, GBuildVer, "buildver");
        set_string(luaState, GLuaLicense, "lualicense");
        set_string_array(luaState, GArgc, GArgv, "argv");
    lua_setglobal(luaState, MOJOSETUP_NAMESPACE);

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


const char *GLuaLicense =
"Lua:\n"
"\n"
"Copyright (C) 1994-2006 Lua.org, PUC-Rio.\n"
"\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
"of this software and associated documentation files (the \"Software\"), to deal\n"
"in the Software without restriction, including without limitation the rights\n"
"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
"copies of the Software, and to permit persons to whom the Software is\n"
"furnished to do so, subject to the following conditions:\n"
"\n"
"The above copyright notice and this permission notice shall be included in\n"
"all copies or substantial portions of the Software.\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE\n"
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n"
"THE SOFTWARE.\n"
"\n";

// end of lua_glue.c ...


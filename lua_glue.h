#ifndef _INCL_LUA_GLUE_H_
#define _INCL_LUA_GLUE_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

// License text for Lua.
extern const char *GLuaLicense;

boolean MojoLua_initLua(void);
void MojoLua_deinitLua(void);
boolean MojoLua_initialized(void);

// Run the code in a given Lua file. This is JUST the base filename.
//  We will look for it in GBaseArchive in the lua/ directory, both as
//  fname.luac and fname.lua. This code chunk will accept no arguments, and
//  return no results, but it can change the global state and alter tables,
//  etc, so it can have lasting side effects.
// Will return false if the file couldn't be loaded, or true if the chunk
//  successfully ran. Will not return if there's a runtime error in the
//  chunk, as it will call fatal() instead.
boolean MojoLua_runFile(const char *fname);

// Set a Lua variable in the MojoSetup namespace to a string:
//  MojoLua_setString("bob", "name");
//  in Lua: print(MojoSetup.name)  -- outputs: bob
void MojoLua_setString(const char *str, const char *sym);

// Same as MojoLua_setString, but it creates an ordered table (array).
void MojoLua_setStringArray(int argc, const char **argv, const char *sym);

void MojoLua_collectGarbage(void);

void MojoLua_debugger(void);

#ifdef __cplusplus
}
#endif

#endif

// end of lua_glue.h ...


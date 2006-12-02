#ifndef _INCL_LUA_GLUE_H_
#define _INCL_LUA_GLUE_H_

#include "universal.h"

#ifdef __cplusplus
extern "C" {
#endif

boolean MojoLua_initLua(void);
void MojoLua_deinitLua(void);

#ifdef __cplusplus
}
#endif

#endif

// end of lua_glue.h ...


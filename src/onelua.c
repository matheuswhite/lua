/*
** Lua core, libraries, and interpreter in a single file.
** Compiling just this file generates a complete Lua stand-alone
** program:
**
** $ gcc -O2 -std=c99 -o lua onelua.c -lm
**
** or
**
** $ gcc -O2 -std=c89 -DLUA_USE_C89 -o lua onelua.c -lm
**
*/

/* default is to build the full interpreter */
#ifndef MAKE_LIB
#ifndef MAKE_LUAC
#ifndef MAKE_LUA
#define MAKE_LUA
#endif
#endif
#endif

/*
** Choose suitable platform-specific features. Default is no
** platform-specific features. Some of these options may need extra
** libraries such as -ldl -lreadline -lncurses
*/
#if 0
#define LUA_USE_LINUX
#define LUA_USE_MACOSX
#define LUA_USE_POSIX
#define LUA_ANSI
#endif

/* no need to change anything below this line ----------------------------- */

#include "lua/lprefix.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* setup for luaconf.h */
#define LUA_CORE
#define LUA_LIB
#define ltable_c
#define lvm_c
#include "lua/luaconf.h"

/* do not export internal symbols */
#undef LUAI_FUNC
#undef LUAI_DDEC
#undef LUAI_DDEF
#define LUAI_FUNC static
#define LUAI_DDEC(def) /* empty */
#define LUAI_DDEF static

/* core -- used by all */
#include "lua/lapi.c"
#include "lua/lcode.c"
#include "lua/lctype.c"
#include "lua/ldebug.c"
#include "lua/ldo.c"
#include "lua/ldump.c"
#include "lua/lfunc.c"
#include "lua/lgc.c"
#include "lua/llex.c"
#include "lua/lmem.c"
#include "lua/lobject.c"
#include "lua/lopcodes.c"
#include "lua/lparser.c"
#include "lua/lstate.c"
#include "lua/lstring.c"
#include "lua/ltable.c"
#include "lua/ltm.c"
#include "lua/lundump.c"
#include "lua/lvm.c"
#include "lua/lzio.c"

/* auxiliary library -- used by all */
#include "lua/lauxlib.c"

/* standard library  -- not used by luac */
#ifndef MAKE_LUAC
#include "lua/lbaselib.c"
#include "lua/lcorolib.c"
#include "lua/ldblib.c"
#include "lua/linit.c"
#include "lua/liolib.c"
#include "lua/lmathlib.c"
#include "lua/loadlib.c"
#include "lua/loslib.c"
#include "lua/lstrlib.c"
#include "lua/ltablib.c"
#include "lua/lutf8lib.c"
#endif

/* lua */
#ifdef MAKE_LUA
#include "lua/lua.c"
#endif

/* luac */
#ifdef MAKE_LUAC
#include "lua/luac.c"
#endif

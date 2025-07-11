/*
** $Id: lzio.c $
** Buffered streams
** See Copyright Notice in lua.h
*/

#define lzio_c
#define LUA_CORE

#include "lua/lprefix.h"

#include <string.h>

#include "lua/lua.h"

#include "lua/lapi.h"
#include "lua/llimits.h"
#include "lua/lmem.h"
#include "lua/lstate.h"
#include "lua/lzio.h"

int luaZ_fill(ZIO *z) {
  size_t size;
  lua_State *L = z->L;
  const char *buff;
  lua_unlock(L);
  buff = z->reader(L, z->data, &size);
  lua_lock(L);
  if (buff == NULL || size == 0)
    return EOZ;
  z->n = size - 1; /* discount char being returned */
  z->p = buff;
  return cast_uchar(*(z->p++));
}

void luaZ_init(lua_State *L, ZIO *z, lua_Reader reader, void *data) {
  z->L = L;
  z->reader = reader;
  z->data = data;
  z->n = 0;
  z->p = NULL;
}

/* --------------------------------------------------------------- read --- */

static int checkbuffer(ZIO *z) {
  if (z->n == 0) {           /* no bytes in buffer? */
    if (luaZ_fill(z) == EOZ) /* try to read more */
      return 0;              /* no more input */
    else {
      z->n++; /* luaZ_fill consumed first byte; put it back */
      z->p--;
    }
  }
  return 1; /* now buffer has something */
}

size_t luaZ_read(ZIO *z, void *b, size_t n) {
  while (n) {
    size_t m;
    if (!checkbuffer(z))
      return n; /* no more input; return number of missing bytes */
    m = (n <= z->n) ? n : z->n; /* min. between n and z->n */
    memcpy(b, z->p, m);
    z->n -= m;
    z->p += m;
    b = (char *)b + m;
    n -= m;
  }
  return 0;
}

const void *luaZ_getaddr(ZIO *z, size_t n) {
  const void *res;
  if (!checkbuffer(z))
    return NULL; /* no more input */
  if (z->n < n)  /* not enough bytes? */
    return NULL; /* block not whole; cannot give an address */
  res = z->p;    /* get block address */
  z->n -= n;     /* consume these bytes */
  z->p += n;
  return res;
}

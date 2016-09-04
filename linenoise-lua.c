/* linenoise-lua.c -- version 0.1
 *
 * linenoise lua bindings
 *
 * see LICENSE for copyright notice
 */

#include <stdlib.h>
#include "linenoise.h"
#include <lua.h>
#include <lauxlib.h>

lua_State *_L = NULL;
int ln_comp_callback_ref = LUA_NOREF;
int ln_hint_callback_ref = LUA_NOREF;
const char * ln_comp_mt_name = "linenoise.completion";

static int ln_comp_add (lua_State *L)
{
  linenoiseCompletions **lcu = luaL_checkudata(L, 1, ln_comp_mt_name);
  const char *comp = luaL_checkstring(L, 2);

  linenoiseAddCompletion(*lcu, comp);
  return 0;
}

static void _ln_completion (const char *buf, linenoiseCompletions *lc)
{
  if (_L == NULL) luaL_error(_L, "Bad lua state pointer!");
  if (ln_comp_callback_ref == LUA_NOREF) return;

  int st = lua_gettop(_L);

  lua_geti(_L, LUA_REGISTRYINDEX, ln_comp_callback_ref);

  lua_pushstring(_L, buf);

  linenoiseCompletions **lcu = lua_newuserdata(_L, sizeof(lc));
  luaL_setmetatable(_L, ln_comp_mt_name);
  *lcu = lc;

  lua_call(_L, 2, 0);
  lua_settop(_L, st);
}

static char * _ln_hint(const char *buf, int *color, int *bold)
{
  if (_L == NULL) luaL_error(_L, "Bad lua state pointer!");
  if (ln_hint_callback_ref == LUA_NOREF) return NULL;

  int st = lua_gettop(_L);

  lua_geti(_L, LUA_REGISTRYINDEX, ln_hint_callback_ref);
  lua_pushstring(_L, buf);
  lua_call(_L, 1, 3);

  char *hint = (char *) luaL_optstring(_L, -3, NULL);
  *color = luaL_optinteger(_L, -2, *color);
  *bold = luaL_optinteger(_L, -1, *bold);

  lua_settop(_L, st);

  return hint;
}

static int ln_read(lua_State *L)
{
  const char *prompt = luaL_optstring(L, 1, "");

  _L = L;
  char *line = linenoise(prompt);
  _L = NULL;
  if (line) {
    lua_pushstring(L, line);
    free(line);

    return 1;
  }

  return 0;
}

static int ln_clearscreen (lua_State *L)
{
  linenoiseClearScreen();

  return 0;
}

static int ln_set_multiline (lua_State *L)
{
  int toggle = lua_toboolean(L, 1);
  linenoiseSetMultiLine(toggle);

  return 0;
}

static int ln_histadd (lua_State *L)
{
  const char *line = luaL_checkstring(L, 1);

  lua_pushboolean(L, linenoiseHistoryAdd(line));
  return 1;
}

static int ln_histload (lua_State *L)
{
  const char *fname = luaL_checkstring(L, 1);

  lua_pushboolean(L, !linenoiseHistoryLoad(fname));

  return 1;
}

static int ln_histsave (lua_State *L)
{
  const char *fname = luaL_checkstring(L, 1);

  lua_pushboolean(L, !linenoiseHistorySave(fname));

  return 0;
}

static int ln_setmaxhistory (lua_State *L)
{
  int len = luaL_checkinteger(L, 1);

  lua_pushboolean(L, linenoiseHistorySetMaxLen(len));

  return 1;
}

static int ln_set_comp_callback (lua_State *L)
{
  luaL_checktype(L, 1, LUA_TFUNCTION);
  luaL_unref(L, LUA_REGISTRYINDEX, ln_comp_callback_ref);
  ln_comp_callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);
  return 0;
}

static int ln_set_hint_callback (lua_State *L)
{
  luaL_checktype(L, 1, LUA_TFUNCTION);
  luaL_unref(L, LUA_REGISTRYINDEX, ln_hint_callback_ref);
  ln_hint_callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);
  return 0;
}

static int ln_printcodes (lua_State *L)
{
  linenoisePrintKeyCodes();
  return 0;
}

luaL_Reg ln_comp_mt[] = {
  {"add", ln_comp_add},
  {NULL, NULL}
};

luaL_Reg ln_funcs[] = {
  { "read", ln_read },
  { "clear", ln_clearscreen },
  { "historyAdd", ln_histadd },
  { "historyLoad", ln_histload },
  { "historySave", ln_histsave },
  { "setMaxHistoryLen", ln_setmaxhistory },
  { "setCompletionCallback", ln_set_comp_callback },
  { "setHintCallback", ln_set_hint_callback },
  { "setMultiline", ln_set_multiline },
  { "printKeyCodes", ln_printcodes },
  { NULL, NULL }
};

extern int luaopen_linenoise (lua_State *L)
{
  lua_newtable(L);
  luaL_setfuncs(L, ln_funcs, 0);

  luaL_newmetatable(L, ln_comp_mt_name);
  luaL_setfuncs(L, ln_comp_mt, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  linenoiseSetCompletionCallback(_ln_completion);
  linenoiseSetHintsCallback(_ln_hint);
  // linenoiseSetFreeHintsCallback(free);

  lua_pop(L, 1);

  return 1;
}

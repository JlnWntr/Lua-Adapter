#ifndef LUA_FUNCTION_H
#include "LuaFunction.hpp"
#endif

bool LuaFunction::Call(const char *functionName, const unsigned short int argc,
                       const int args[], int &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, functionName);

  for (unsigned char i = 0; i < argc; i++)
    if (args + i)
      lua_pushnumber(this->Lua, args[i]);
  if (lua_pcall(this->Lua, argc, 1, 0) != LUA_OK) {
    return false;
  }

  if (lua_isnumber(this->Lua, -1)) {
    result = lua_tointeger(this->Lua, -1);
  }
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *functionName, const char *const string,
                       const size_t length) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, functionName);
  lua_pushlstring(this->Lua, string, length);
  if ((lua_pcall(this->Lua, 1, 1, 0) != LUA_OK)) {
    lua_pop(this->Lua, 1);
    return false;
  }
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *functionName) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, functionName);
  if (lua_pcall(this->Lua, 0, 0, 0) == LUA_OK) {
    return true;
  }
  return false;
}

bool LuaFunction::Call(const char *functionName, const char *const string,
                       size_t &length, std::string &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, functionName);
  lua_pushlstring(this->Lua, string, length);

  if ((lua_pcall(this->Lua, 1, 1, 0) != LUA_OK) ||
      (lua_isstring(this->Lua, -1) == false)) {
    lua_pop(this->Lua, 1);
    return false;
  }
  size_t l{0};
  const char *buffer{lua_tolstring(this->Lua, -1, &l)};
  if ((!buffer) || (l == 0)) {
    return false;
  }
  length = l;
  result = std::string{buffer, length};
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *functionName, const std::string arg,
                       std::string &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, functionName);
  lua_pushlstring(this->Lua, arg.c_str(), arg.length());

  if ((lua_pcall(this->Lua, 1, 1, 0) != LUA_OK) ||
      (lua_isstring(this->Lua, -1) == false)) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tostring(this->Lua, -1);
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *functionName, double &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, functionName);

  if ((lua_pcall(this->Lua, 0, 1, 0) != LUA_OK) ||
      (lua_isnumber(this->Lua, -1) == false)) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tonumber(this->Lua, -1);
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Push(Lua_callback_function function,
                       const char *functionName) {
  if (!this->Lua)
    return false;

  lua_pushcfunction(this->Lua, function);
  lua_setglobal(this->Lua, functionName);
  return true;
}

LuaFunction::LuaFunction(LuaAdapter &lua) : Lua{lua.GetLuaState()} {}

LuaFunction::LuaFunction(lua_State *const lua) : Lua{lua} {}

LuaFunction::~LuaFunction() {}

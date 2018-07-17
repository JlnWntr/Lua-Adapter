#ifndef LUA_FUNCTION_H
#include "LuaFunction.hpp"
#endif

LuaFunction::LuaFunction(LuaAdapter &lua) : Lua{lua.GetLuaState()} {}

LuaFunction::LuaFunction(lua_State *const lua) : Lua{lua} {}

LuaFunction::~LuaFunction() {}

bool LuaFunction::Call(const char *function_name, const unsigned short int argc,
                       const int args[], int &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, function_name);

  for (unsigned char i = 0; i < argc; i++)
    if (args + i)
      lua_pushnumber(this->Lua, args[i]);
  if (this->Pcall(argc, 1, 0) == false) {
    return false;
  }

  if (lua_isnumber(this->Lua, -1)) {
    result = lua_tointeger(this->Lua, -1);
  }
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *function_name, const char *const string,
                       const size_t length) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, function_name);
  lua_pushlstring(this->Lua, string, length);
  if (this->Pcall(1, 1, 0) == false){
    return false;
  }
  //lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *function_name) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, function_name);
  if (this->Pcall(0, 0, 0) == false) {
    return false;
  }
  //lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *function_name, const char *const string,
                       size_t &length, std::string &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, function_name);
  lua_pushlstring(this->Lua, string, length);

  if (this->Pcall(1, 1, 0) == false){
    return false;
  }
  if  (lua_isstring(this->Lua, -1) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  size_t l{0};
  const char *buffer{lua_tolstring(this->Lua, -1, &l)};
  if ((!buffer) || (l == 0)) {
    lua_pop(this->Lua, 1);
    return false;
  }
  length = l;
  result = std::string{buffer, length};
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *function_name, const std::string arg,
                       std::string &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, function_name);
  lua_pushlstring(this->Lua, arg.c_str(), arg.length());

  if (this->Pcall(1, 1, 0) == false){
    return false;
  }
  if  (lua_isstring(this->Lua, -1) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tostring(this->Lua, -1);
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *function_name, double &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, function_name);

  if (this->Pcall(0, 1, 0) == false)
    return false;
  if (lua_isnumber(this->Lua, -1) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tonumber(this->Lua, -1);
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Call(const char *function_name, const int arg, int &result) {
  if (!this->Lua) {
    return false;
  }
  lua_getglobal(this->Lua, function_name);
  lua_pushnumber(this->Lua, arg);
  if (this->Pcall(1, 1, 0) == false)
    return false;
  if (lua_isnumber(this->Lua, -1) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tonumber(this->Lua, -1);
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaFunction::Push(Lua_callback_function function,
                       const char *function_name) {
  if (!this->Lua)
    return false;

  lua_pushcfunction(this->Lua, function);
  lua_setglobal(this->Lua, function_name);
  return true;
}

bool LuaFunction::Pcall(int nargs, int nresults, int msgh){
  const int call {lua_pcall(this->Lua, nargs, nresults, msgh)};
  if (call == LUA_OK){
    return true;
  }
  std::cerr << LUA_PREFIX << "Error: pcall failed. Code: ";
  std::cerr << call;
  std::cerr << ", '" << lua_tostring(this->Lua, -1) << "'\n";
  lua_pop(this->Lua, 1);
  return false;
}
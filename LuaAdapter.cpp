/*
* Copyright (c) 2015-2018 JlnWntr (jlnwntr@gmail.com)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef LUA_ADAPTER_H
#include "LuaAdapter.hpp"
#endif

LuaAdapter::LuaAdapter(lua_State *const lua)
    : Lua{lua}, print{false}, single{(lua) ? false : true} {
  this->Init();
}

LuaAdapter::LuaAdapter(const std::string &filename)
    : Lua{nullptr}, print{false}, single{true} {
  this->Load(filename);
}

void LuaAdapter::Init() {
  if (this->Lua) {
    return;
  }
  this->Lua = luaL_newstate();
  luaL_openlibs(this->Lua);
}

bool LuaAdapter::Load(const char *filename) {
  this->Init();

  if ((luaL_loadfile(this->Lua, filename) == 0) && this->Eval()) {
    return true;
  }
  std::cout << LUA_PREFIX << "Error in Lua-file '";
  std::cout << filename << "': ";
  std::cout << lua_tostring(this->Lua, -1);
  std::cout << "\n";

  return false;
}

bool LuaAdapter::Eval() {
  if (this->print == false)
    return ((this->Lua) && (lua_pcall(this->Lua, 0, 0, 0) == 0));

  if (!this->Lua) {
    std::cout << LUA_PREFIX << "Error: Lua-state invalid. Call init() first!\n";
    return false;
  }
  const int pcall{lua_pcall(this->Lua, 0, 0, 0)};
  if (pcall == 0)
    return true;
  std::cout << LUA_PREFIX << "Error: pcall failed. Code: ";
  std::cout << pcall;
  std::cout << ", '" << lua_tostring(this->Lua, -1) << "'\n";
  /* LUA_ERRRUN: a runtime error. (2)
  * LUA_ERRMEM: memory allocation error. (4)
  * LUA_ERRERR: error while running the error handler function. (5)
  */
  return false;
}

bool LuaAdapter::Load(const std::string &filename) {
  return this->Load(filename.c_str());
}

bool LuaAdapter::GetGlobal(const char *name) {
  if ((!this->Lua) || (!name))
    return false;

  lua_getglobal(this->Lua, name); // void
  return true;
}

bool LuaAdapter::Get(const char *name, int &result) {
  if (this->GetGlobal(name) == false)
    return false;

  if (lua_isnumber(this->Lua, -1) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tointeger(this->Lua, -1);
  if (this->print)
    std::cout << LUA_PREFIX << "get int '" << name << "' = '" << result
              << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaAdapter::Get(const char *name, std::string &result) {
  if (this->GetGlobal(name) == false)
    return false;

  if (lua_type(this->Lua, -1) != LUA_TSTRING) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tostring(this->Lua, -1);
  if (this->print)
    std::cout << LUA_PREFIX << "get string '" << name << "' = '" << result
              << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaAdapter::Get(const char *name, double &result) {
  if (this->GetGlobal(name) == false)
    return false;

  if (lua_isnumber(this->Lua, -1) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tonumber(this->Lua, -1);
  if (this->print)
    std::cout << LUA_PREFIX << "get double '" << name << "' = '" << result
              << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaAdapter::Get(const char *name, float &result) {
  double temp = 0.0;
  if (this->Get(name, temp) == false)
    return false;
  result = (float)temp;
  return true;
}

bool LuaAdapter::Get(const char *name, bool &result) {
  if (this->GetGlobal(name) == false)
    return false;

  if (lua_isboolean(this->Lua, -1) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_toboolean(this->Lua, -1);
  if (this->print)
    std::cout << LUA_PREFIX << "get boolean '" << name << "' = '" << result
              << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaAdapter::Flush() {
  if ((!this->Lua))
    return false;

  lua_settop(this->Lua, 0);
  return true;
}

bool LuaAdapter::Set(const char *name, const char *value) {
  if (!this->Lua)
    return false;
  lua_pushstring(this->Lua, value);
  lua_setglobal(this->Lua, name);
  if (this->print)
    std::cout << LUA_PREFIX << "set string '" << name << "' = '" << value
              << "' \n";
  return true;
}

bool LuaAdapter::Set(const char *name, int value) {
  if (!this->Lua) {
    return false;
  }
  lua_pushnumber(this->Lua, value);
  lua_setglobal(this->Lua, name);
  if (this->print)
    std::cout << LUA_PREFIX << "set int '" << name << "' = '" << value
              << "' \n";
  return true;
}

bool LuaAdapter::Set(const char *name, const double value) {
  if (!this->Lua)
    return false;
  lua_pushnumber(this->Lua, value);
  lua_setglobal(this->Lua, name);
  if (this->print)
    std::cout << LUA_PREFIX << "set double '" << name << "' = '" << value
              << "' \n";
  return true;
}

bool LuaAdapter::Set(const char *name, const bool value) {
  if (!this->Lua)
    return false;
  lua_pushboolean(this->Lua, value);
  lua_setglobal(this->Lua, name);
  if (this->print)
    std::cout << LUA_PREFIX << "set boolean '" << name << "' = '" << value
              << "' \n";
  return true;
}

bool LuaAdapter::Set(const char *name, const float value) {
  return this->Set(name, (double)value);
}

bool LuaAdapter::Push(double number) {
  if (!this->Lua)
    return false;

  lua_pushnumber(this->Lua, number);
  return true;
}

// TODO use pushlstring and define specific length
bool LuaAdapter::Push(const char *string) {
  if (!this->Lua)
    return false;

  lua_pushstring(this->Lua, string);
  return true;
}

void LuaAdapter::Close() {
  if ((!this->Lua) || (!this->single))
    return;
  // this->Flush();
  lua_close(this->Lua);

  this->Lua = nullptr;
}

LuaAdapter::~LuaAdapter() { this->Close(); }

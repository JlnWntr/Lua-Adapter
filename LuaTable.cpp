#ifndef LUA_TABLE_H
#include "LuaTable.hpp"
#endif
LuaTable::LuaTable(LuaAdapter &lua)
    : Lua{lua.GetLuaState()}, print{lua.IsDebugging()} {}

LuaTable::LuaTable(lua_State *const lua) : Lua{lua}, print{false} {}

LuaTable::~LuaTable() {}

bool LuaTable::Get(const char *name) {
  if ((!this->Lua) || (!name) || (!lua_istable(this->Lua, -1)))
    return false;

  lua_getfield(this->Lua, -1, name);
  return true;
}

bool LuaTable::Get(const char *name, std::string &result) {
  if (this->Get(name) == false)
    return false;

  if (lua_type(this->Lua, -1) != LUA_TSTRING) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tostring(this->Lua, -1);
  if (this->print)
    std::cout << "\t" << LUA_PREFIX << "get string-field '" << name << "' = '"
              << result << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaTable::Open(const char *name) {
  if (!name)
    return false;

  if (lua_istable(this->Lua, -1)) {
    return this->OpenNested(name);
  }
  lua_getglobal(this->Lua, name); // void

  if (lua_istable(this->Lua, -1)) {

    if (this->print)
      std::cout << LUA_PREFIX << "open table '" << name << "' \n";
    return true;
  }
  lua_pop(this->Lua, 1);
  return false;
}

unsigned short int LuaTable::Length() {
  unsigned short int result{0};
  if ((!this->Lua)) {
    return result;
  }
  if (lua_istable(this->Lua, -1) == false) {
    return result;
  }
  lua_len(this->Lua, -1);
  result = lua_tointeger(this->Lua, -1);
  lua_pop(this->Lua, 1);
  return result;
}

bool LuaTable::Get(const char *name, int &result) {
  if (this->Get(name) == false)
    return false;
  if (lua_isnumber(this->Lua, -1) != 1) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tointeger(this->Lua, -1);
  if (this->print)
    std::cout << "\t" << LUA_PREFIX << "get int-field '" << name << "' = '"
              << result << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaTable::Get(unsigned short int i, int &result) {
  if (this->GetI(i) == false)
    return false;

  if (lua_isnumber(this->Lua, -1) != 1) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tointeger(this->Lua, -1);
  if (this->print)
    std::cout << "\t" << LUA_PREFIX << "get int-field " << i << " = '" << result
              << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaTable::Get(unsigned short int i, double &result) {
  if (this->GetI(i) == false)
    return false;

  if (lua_isnumber(this->Lua, -1) != 1) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tonumber(this->Lua, -1);
  if (this->print)
    std::cout << "\t" << LUA_PREFIX << "get double-field " << i << " = '"
              << result << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaTable::Get(unsigned short int i, float &result) {
  if (this->GetI(i) == false)
    return false;

  if (lua_isnumber(this->Lua, -1) != 1) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = (float)lua_tonumber(this->Lua, -1);
  if (this->print)
    std::cout << "\t" << LUA_PREFIX << "get float-field " << i << " = '"
              << result << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaTable::Get(unsigned short int i, std::string &result) {
  if (this->GetI(i) == false)
    return false;
  if (lua_type(this->Lua, -1) != LUA_TSTRING) {
    lua_pop(this->Lua, 1);
    return false;
  }
  result = lua_tostring(this->Lua, -1);
  if (this->print)
    std::cout << "\t" << LUA_PREFIX << "get string-field " << i << " = '"
              << result << "' \n";
  lua_pop(this->Lua, 1);
  return true;
}

bool LuaTable::Get(unsigned short int j, unsigned short int i, double &result) {
  if (this->GetI(j) == false) {
    return false;
  }
  if (this->GetI(i) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  if (lua_isnumber(this->Lua, -1))
    result = lua_tonumber(this->Lua, -1);
  if (this->print)
    std::cout << "\t"
              << "\t" << LUA_PREFIX << "get nested double-field (" << j << "|"
              << i << ")' = '" << result << "' \n";
  lua_pop(this->Lua, 2);
  return true;
}

bool LuaTable::Get(unsigned short int j, unsigned short int i, float &result) {
  if (this->GetI(j) == false) {
    return false;
  }
  if (this->GetI(i) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  if (lua_isnumber(this->Lua, -1))
    result = (float)lua_tonumber(this->Lua, -1);
  if (this->print)
    std::cout << "\t"
              << "\t" << LUA_PREFIX << "get nested float-field (" << j << "|"
              << i << ")' = '" << result << "' \n";
  lua_pop(this->Lua, 2);
  return true;
}

bool LuaTable::Get(unsigned short int j, unsigned short int i, int &result) {
  if (this->GetI(j) == false) {
    return false;
  }
  if (this->GetI(i) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  if (lua_isnumber(this->Lua, -1))
    result = lua_tointeger(this->Lua, -1);
  if (this->print)
    std::cout << "\t"
              << "\t" << LUA_PREFIX << "get nested int-field (" << j << "|" << i
              << ")' = '" << result << "' \n";
  lua_pop(this->Lua, 2);
  return true;
}

bool LuaTable::Get(unsigned short int j, unsigned short int i,
                   std::string &result) {
  if (this->GetI(j) == false) {
    return false;
  }
  if (this->GetI(i) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  if (lua_isstring(this->Lua, -1))
    result = lua_tostring(this->Lua, -1);
  if (this->print)
    std::cout << "\t"
              << "\t" << LUA_PREFIX << "get nested string-field (" << j << "|"
              << i << ")' = '" << result << "' \n";
  lua_pop(this->Lua, 2);
  return true;
}

bool LuaTable::Get(unsigned short int k, unsigned short int j,
                   unsigned short int i, int &result) {
  if (this->GetI(k) == false) {
    return false;
  }
  if (this->GetI(j) == false) {
    return false;
  }
  if (this->GetI(i) == false) {
    lua_pop(this->Lua, 1);
    return false;
  }
  if (lua_isnumber(this->Lua, -1))
    result = lua_tointeger(this->Lua, -1);
  if (this->print)
    std::cout << "\t"
              << "\t" << LUA_PREFIX << "get nested int-field (" << j << "|" << i
              << ")' = '" << result << "' \n";
  lua_pop(this->Lua, 3);
  return true;
}

bool LuaTable::GetI(unsigned short int i) {
  if ((!this->Lua) || (!lua_istable(this->Lua, -1)) || (i < 1))
    return false;

  lua_rawgeti(this->Lua, -1, i);
  return true;
}

bool LuaTable::OpenNested(const char *name) {
  if ((!this->Lua)) {
    return false;
  }
  if (this->print)
    std::cout << "\t" << LUA_PREFIX << "opening nested table '" << name
              << "' ... ";

  if (lua_istable(this->Lua, -1) == false) {
    return false;
  }

  lua_getfield(this->Lua, -1, name);

  if (lua_isnil(this->Lua, -1)) {
    lua_pop(this->Lua, 1);
    return false;
  }

  if (lua_istable(this->Lua, -1)) {
    if (this->print)
      std::cout << "ok! \n";
    return true;
  }
  lua_pop(this->Lua, 1);
  return false;
}
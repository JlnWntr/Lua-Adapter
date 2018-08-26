/*
*Copyright (c) 2015-2018 JlnWntr (jlnwntr@gmail.com)
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all
*copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*SOFTWARE.
*/

#ifndef LUA_TABLE_H
#define LUA_TABLE_H

#include <vector>

#ifndef LUA_ADAPTER_H
#include "LuaAdapter.hpp"
#endif

class LuaTable {

public:
  /**
  * Constructor
  * @param lua uses an existing lua_state
  */
  LuaTable(LuaAdapter &lua) : Lua{lua.GetLuaState()}, debug{lua.GetDebug()} {}

  /**
  * Default constructor
  * @param lua uses an existing lua_state
  */
  LuaTable(lua_State *const lua = nullptr) : Lua{lua}, debug{false} {}

  /**
    * Opens a lua table
    * @param name Name of the table inside loaded lua state
    * @return true on success, false on error
    */
  bool Open(const std::string &name) { return this->Open(name.c_str()); }
  bool Open(const char *name) {
    if (!name)
      return false;

    if (lua_istable(this->Lua, -1)) {
      if (this->debug)
        std::cout << "\t" << LUA_PREFIX << "opening nested table '" << name
                  << "' ... ";
      lua_getfield(this->Lua, -1, name);

      if (lua_isnil(this->Lua, -1)) {
        lua_pop(this->Lua, 1);
        return false;
      }

      if (lua_istable(this->Lua, -1)) {
        if (this->debug)
          std::cout << "ok! \n";
        return true;
      }
      return false;
    }

    lua_getglobal(this->Lua, name); // void

    if (lua_istable(this->Lua, -1)) {

      if (this->debug)
        std::cout << LUA_PREFIX << "open table '" << name << "' \n";
      return true;
    }

    lua_pop(this->Lua, 1);
    return false;
  }

  /**
    * Get the length of the current (opened) table
    * (tables with integer keys and without 'nil-holes')
    * @return the length of the table
    */
  unsigned short int Length() {
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

  /**
  * Destructor
  */
  ~LuaTable(){}

  /**
   * (Re-)Sets the lua state
   * @param lua lua_state
   * @return true on success, false on error
  */
  bool SetLuaState(lua_State *const lua) {
    if (!lua)
      return false;
    this->Lua = lua;
    return true;
  }

/**
  * Gets a field from an opened table
  * @param name of the field
  * @param r value of the field
  * @return true on success, false on error
  */
  template <typename R> bool Get(const char *name, R &r) {
    if (!this->Lua || !name || !lua_istable(this->Lua, -1))
      return false;

    lua_getfield(this->Lua, -1, name);

    if (this->convert(r) == false) {
      lua_pop(this->Lua, 1);
      return false;
    }
    if (this->debug)
      std::cout << "\t" << LUA_PREFIX << "got field '" << name << "' = '" << r
                << "' \n";
    lua_pop(this->Lua, 1);
    return true;
  }

  /**
  * Gets a field from an opened table
  * @param i i-th field (starting at 1)
  * @param r value of the field
  * @return true on success, false on error
  */
  template <typename R> bool Get(unsigned short int i, R &r) {
    if ((!this->Lua) || (!lua_istable(this->Lua, -1)) || (i < 1))
      return false;

    lua_rawgeti(this->Lua, -1, i);

    if (this->convert(r) == false) {
      lua_pop(this->Lua, 1);
      return false;
    }

    if (this->debug)
      std::cout << "\t" << LUA_PREFIX << "got " << i << "th field "
                                                        " = '"
                << r << "' \n";
    lua_pop(this->Lua, 1);
    return true;
  }
  /**
  * Gets a field value from an opened n-dimensional table.
  * Example: lua_identity ={
  *               {1, 0, 0},
  *               {0, 1, 0},
  *               {0, 0, 1},
  *           }  *
  * @param m table-indeces. e.g. {1,2} for first 0 from example table above.
  * @param r value of the field
  * @return true on success, false on error
  */
  template <typename R>
  bool Get(const std::vector<unsigned short int> &m, R &r) {
    if (!this->Lua)
      return false;
    for (auto &v : m)
      if (lua_istable(this->Lua, -1))
        lua_rawgeti(this->Lua, -1, v);

    const bool Result{this->convert(r)};

    if (this->debug && Result)
      std::cout << "\t" << LUA_PREFIX << "got field "
                                         " = '"
                << r << "' \n";
    while (lua_gettop(this->Lua) > 1)
      lua_pop(this->Lua, 1);
    return Result;
  }

  /**
  *Closes a table
  *(This is important to prevents "stack-smashing".)
  */
  void Close() { lua_pop(this->Lua, 1); }

private:
  template <typename R> bool convert(R &r) {
    if (lua_isnumber(this->Lua, -1) && !lua_isinteger(this->Lua, -1)) {
      if
        constexpr(std::is_same_v<double, R> || std::is_same_v<R, float>) r =
            lua_tonumber(this->Lua, -1);
    } else if (lua_isinteger(this->Lua, -1)) {
      if
        constexpr(std::is_same_v<R, int>) r = lua_tointeger(this->Lua, -1);
    } else if (lua_isboolean(this->Lua, -1)) {
      if
        constexpr(std::is_same_v<R, bool>) r = lua_toboolean(this->Lua, -1);
    } else if (lua_isstring(this->Lua, -1)) {
      if
        constexpr(std::is_same_v<R, std::string>) r =
            lua_tostring(this->Lua, -1);
    } else
      return false;

    return true;
  }

  lua_State *Lua;
  const bool debug;
};
#endif
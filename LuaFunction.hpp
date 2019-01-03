/*
* Copyright (c) 2015-2019 JlnWntr (jlnwntr@gmail.com)
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

#ifndef LUA_FUNCTION_H
#define LUA_FUNCTION_H

#ifndef LUA_ADAPTER_H
#include "LuaAdapter.hpp"
#endif

static int LUA_ADAPTER_NULL{};
typedef int (*Lua_callback_function)(lua_State *L);

class LuaFunction {

public:
  /**
  *Constructor
  *@param lua uses an existing lua_state
  */
  LuaFunction(LuaAdapter &lua) : Lua{lua.GetLuaState()} {}

  /**
  *Default constructor
  *@param lua uses an existing lua_state
  */
  LuaFunction(lua_State *const lua = nullptr) : Lua{lua} {}

  /**
  * Destructor
  */
  ~LuaFunction() {}

  /**
   * Calls a lua-function
   * @param f name of the lua-function
   * @param c number of arguments passed to the lua function
   * @param a function arguments
   * @param r new value returned from the called lua function
   * @return true on success, false on error
   */
  template <typename A, typename R>
  bool Call(const char *f, const auto c, const A *const a, R &r) {
    if ((!this->Lua) || (!f) || (lua_getglobal(this->Lua, f) != LUA_TFUNCTION))
      return false;

    for (auto i = 0; i < c; i++) {
      if (!(a + i))
        break;
      if
        constexpr(std::is_same_v<double, A> || std::is_same_v<A, float>)
            lua_pushnumber(this->Lua, a[i]);
      else if
        constexpr(
            std::is_same_v<A, int>) {
          lua_pushinteger(this->Lua, a[i]);
        }
      else if
        constexpr(std::is_same_v<A, std::string>)
            lua_pushlstring(this->Lua, a[i].c_str(), a[i].length());
      else if
        constexpr(std::is_same_v<A, bool>) lua_pushboolean(this->Lua, a[i]);
    }
    if (this->Pcall(c, 1, 0) == false)
      return false;
    if
      constexpr(std::is_same_v<double, R> || std::is_same_v<R, float>) {
        if (lua_isnumber(this->Lua, -1))
          r = lua_tonumber(this->Lua, -1);
      }
    else if
      constexpr(std::is_same_v<R, int>) {
        if (lua_isinteger(this->Lua, -1))
          r = lua_tointeger(this->Lua, -1);
      }
    else if
      constexpr(std::is_same_v<R, bool>) {
        if (lua_isboolean(this->Lua, -1))
          r = lua_toboolean(this->Lua, -1);
      }
    else if
      constexpr(std::is_same_v<R, std::string>) {
        if (lua_isstring(this->Lua, -1))
          r = lua_tostring(this->Lua, -1);
      }
    lua_pop(this->Lua, 1);
    return true;
  }
  template <typename A, typename R>
  bool Call(const std::string f, const auto c, const A *const a, R &r) {
      return this->Call(f.c_str(), c, a, r);
  }

 /**
   * Calls a lua-function
   * @param f name of the lua-function
   * @param c number of arguments passed to the lua function
   * @param a function arguments
   * @return true on success, false on error
   */
  template <typename A>
  bool Call(const char *f, const auto c, const A *const a) {
    if ((!this->Lua) || (!f) || (lua_getglobal(this->Lua, f) != LUA_TFUNCTION))
      return false;

    for (auto i = 0; i < c; i++) {
      if (!(a + i))
        break;
      if
        constexpr(std::is_same_v<double, A> || std::is_same_v<A, float>)
            lua_pushnumber(this->Lua, a[i]);
      else if
        constexpr(std::is_same_v<A, int>) lua_pushinteger(this->Lua, a[i]);
      else if
        constexpr(std::is_same_v<A, std::string>)
            lua_pushlstring(this->Lua, a[i].c_str(), a[i].length());
      else if
        constexpr(std::is_same_v<A, bool>) lua_pushboolean(this->Lua, a[i]);
    }
    return this->Pcall(c, 0, 0);
  }
  template <typename A>
  bool Call(const std::string f, const auto c, const A *const a) {
      return this->Call(f.c_str(), c, a);
  }

 /**
   * Calls a lua-function
   * @param f name of the lua function
   * @param a function argument
   * @param r new value returned from the called lua function
   * @return true on success, false on error
   */
  template <typename A, typename R>
  bool Call(const char *f, const A a, R &r = LUA_ADAPTER_NULL) {
    return this->Call(f, 1, (const A *)&a, r);
  }
  template <typename A, typename R>
  bool Call(const std::string f, const A a, R &r = LUA_ADAPTER_NULL) {
    return this->Call(f.c_str(), 1, (const A *)&a, r);
  }

  /**
   * Calls a lua-function
   * @param f name of the lua-function
   * @param a function argument
   * @return true on success, false on error
   */
  template <typename A> bool Call(const char *f, const A a) {
    return this->Call(f, 1, (const A *)&a);
  }
  template <typename A> bool Call(const std::string f, const A a) {
    return this->Call(f.c_str(), 1, (const A *)&a);
  }

  /**
   * Calls a lua-function
   * @param f name of the lua-function
   * @return true on success, false on error
   */
  bool Call(const char *f) {
    if ((!this->Lua) || (!f) || (lua_getglobal(this->Lua, f) != LUA_TFUNCTION))
      return false;
    return this->Pcall(0, 0, 0);
  }
  bool Call(const std::string f) {
      return this->Call(f.c_str());
  }

  /**
    * Makes a C-/C++-function call available for lua
    * (it's called pushFunction(), but you're not 'incrementing' the stack)
    * @param function C-/C++-function
    * @param name of the function
    * @return true on success, false on error
    */
  bool Push(Lua_callback_function function, const char *name) {
    if (!this->Lua)
      return false;
    lua_pushcfunction(this->Lua, function);
    lua_setglobal(this->Lua, name);
    return true;
  }
  bool Push(Lua_callback_function function, const std::string name) {
    return Push(function, name.c_str());
  }

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

private:
  lua_State *Lua;

  /**
  * Calls lua's pcall and outputs errors.
  * @param na number of a
  * @param nresults number of results
  * @param int msgh see https://www.lua.org/manual/5.3/manual.html#lua_pcall
  * @return true on success, false on error
  */
  bool Pcall(int na, int nresults, int msgh) {
    const int call{lua_pcall(this->Lua, na, nresults, msgh)};
    if (call == LUA_OK) {
      return true;
    }
    std::cerr << LUA_PREFIX << "Error: pcall failed. Code: ";
    std::cerr << call;
    std::cerr << ", '" << lua_tostring(this->Lua, -1) << "'\n";
    lua_pop(this->Lua, 1);
    return false;
  }
};
#endif
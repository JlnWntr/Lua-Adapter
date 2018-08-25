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
#define LUA_ADAPTER_H

#include <iostream>
#include <lua.hpp>

#define LUA_PREFIX "Lua > "
class LuaTable;

class LuaAdapter {

public:
  /**
  * Default constructor
  * Hint: Calls init()!
  * @param lua (re-)uses an existing lua_state. See example in test.cpp at
  * testCFunction()
  */
  LuaAdapter(lua_State *const lua = nullptr)
      : Lua{lua}, debug{false}, single{(lua) ? false : true} {
    this->Init();
  }

  /**
  * This constructor inits lua and loads a .lua-file directly.
  * @param filename .lua-file to load
  */
  LuaAdapter(const std::string &filename)
      : Lua{nullptr}, debug{false}, single{true} {
    this->Load(filename);
  }

  /**
  * Destructor
  */
  ~LuaAdapter() { this->Close(); }

  /**
  * Initialize Lua and all its libs
  * Does NOT load any lua-sourcefile
  * LuaAdapter calls this internally single-handed.
  * @return true on success, false on error
  */
  bool Init() {
    if (this->Lua) {
      return true;
    }
    this->Lua = luaL_newstate();
    if (!this->Lua) {
      return false;
    }
    luaL_openlibs(this->Lua);
    return true;
  }

  /**
  * Calls lua_pcall(Lua, 0, 0, 0)
  * @return true on success, false on error
  */
  bool Eval() {
    if (this->debug == false)
      return ((this->Lua) && (lua_pcall(this->Lua, 0, 0, 0) == 0));

    if (!this->Lua) {
      std::cerr << LUA_PREFIX
                << "Error: Lua-state invalid. Call init() first!\n";
      return false;
    }
    const int pcall{lua_pcall(this->Lua, 0, 0, 0)};
    if (pcall == 0)
      return true;
    std::cerr << LUA_PREFIX << "Error: pcall failed. Code: ";
    std::cerr << pcall;
    std::cerr << ", '" << lua_tostring(this->Lua, -1) << "'\n";
    this->Pop();
    return false;
  }

  /**
  * Loads a *.lua-sourcefile
  * Set your default (local) lua-vars before you call this function!
  * @param filename lua-file to load
  * @return true on success, false on error
  */

  bool Load(const char *filename) {
    this->Init();

    if ((luaL_loadfile(this->Lua, filename) == 0) && this->Eval()) {
      return true;
    }
    std::cerr << LUA_PREFIX << "Error in Lua-file '";
    std::cerr << filename << "': ";
    std::cerr << lua_tostring(this->Lua, -1);
    std::cerr << "\n";

    return false;
  }

  bool Load(const std::string &filename) {
    return this->Load(filename.c_str());
  }

  /**
  * Gets the value of a variable.
  * @param name of the variable inside loaded lua state
  * @param result value is saved in this variable
  * @return true on success, false on error
  */
  template <typename R> bool Get(const char *name, R &r) {

    if (!this->Lua || !name)
      return false;

    switch (lua_getglobal(this->Lua, name)) {

    case LUA_TNUMBER:

      if (lua_isinteger(this->Lua, -1)) {
        if
          constexpr(std::is_same_v<R, int>) r = lua_tointeger(this->Lua, -1);
      } else if
        constexpr(std::is_same_v<double, R> || std::is_same_v<R, float>) r =
            lua_tonumber(this->Lua, -1);
      break;
    case LUA_TBOOLEAN:
      if
        constexpr(std::is_same_v<R, bool>) r = lua_toboolean(this->Lua, -1);
      break;
    case LUA_TSTRING:

      if
        constexpr(std::is_same_v<R, std::string>) r =
            lua_tostring(this->Lua, -1);

      break;
    default:
      return false;
      break;
    }
    if (this->debug)
      std::cout << LUA_PREFIX << "got '" << name << "' = '" << r << "' \n";
    lua_pop(this->Lua, 1);
    return true;
  }

  /**
  * Sets the value of global lua-var.
  * @param name of the variable
  * @param value the var's value
  * @return true on success, false on error
  */

  template <typename A> bool Set(const char *name, const A a) {

    if (!this->Lua || !name)
      return false;

    if (this->Push(a) == true) {

      lua_setglobal(this->Lua, name);

      if (this->debug)
        std::cout << LUA_PREFIX << "set '" << name << "' = '" << a << "' \n";
      return true;
    }
    return false;
  }

  /**
      * Execute any string
  * @param string to execute, for example "test = io.read()"
      */
  bool DoString(const char *string) { return luaL_dostring(this->Lua, string); }

  /**
  * Push data on the lua-stack. (Mind the stack!)
  * @param number number to push
  * @param string string to push
  * @return true on success, false on error
  */
  template <typename A> bool Push(A a) {
    if (!this->Lua)
      return false;

    if
      constexpr(std::is_same_v<A, int>) lua_pushinteger(this->Lua, a);

    else if
      constexpr(std::is_same_v<A, float> || std::is_same_v<A, double>)
          lua_pushnumber(this->Lua, a);

    else if
      constexpr(std::is_same_v<A, bool>) lua_pushboolean(this->Lua, a);

    else if
      constexpr(std::is_same_v<A, std::string>)
          lua_pushlstring(this->Lua, a.c_str(), a.length());
    else
      return false;
    return true;
  }

  /**
  * Resets Lua's internal stack
  * @return true on success, false on error
  */
  bool Flush() {
    if ((!this->Lua))
      return false;

    lua_settop(this->Lua, 0);
    return true;
  }

  /**
  * After calling this function,
  * lua-adapter will print out debug-information for each following
  * Lua-Adapter function call.
  * @param on true to activate debug-mode
  */
  void Debug(bool on = true) { this->debug = on; }

  /**
  * Return the current debugging-state
  * @return true if debug-mode is activated, false if not
  */
  bool GetDebug() const { return this->debug; }

  /**
  * Pops i entries from Lua's internal stack
  * @param i number of entries
  */
  void Pop(signed short int i = 1) { lua_pop(this->Lua, i); }

  /**
  * Gets the stack position
  * @return the stack position
  */
  int GetTop() const { return lua_gettop(this->Lua); }

  /**
  * Gets the value-type of the current stack position
  * @return the type
  */
  int GetType() const { return lua_type(this->Lua, 0); }

  /**
  * Returns the current lua_State which is used in this adapter
  * This is necessary, because you need this state for LuaFunctions or
  * LuaTables!
  * @return the current lua_State
  */
  lua_State *const GetLuaState() const { return this->Lua; }

  /**
  * "Closes" the loaded *.lua-sourcefile
  * This function is called by the destructor of this class!
  * */
  void Close() {
    if ((!this->Lua) || (!this->single))
      return;
    // this->Flush();
    lua_close(this->Lua);

    this->Lua = nullptr;
  }

private:
  lua_State *Lua;
  bool debug;
  bool single;
};
#endif

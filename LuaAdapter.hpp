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
#include <string>

#define LUA_PREFIX "Lua > "
class LuaAdapter {

public:
  /**
  * Default constructor
  * Hint: Calls init()!
  * @param lua (re-)uses an existing lua_state. See example in test.cpp at
  * testCFunction()
  */
  LuaAdapter(lua_State *const lua = nullptr);

  /**
  * This constructor inits lua and loads a .lua-file directly.
  * @param filename .lua-file to load
  */
  LuaAdapter(const std::string &filename);

  /**
  * Destructor
  */
  ~LuaAdapter();

  /**
  * Initialize Lua and all its libs
  * Does NOT load any lua-sourcefile
  * LuaAdapter calls this internally single-handed.
  */
  void Init();

  /**
  * Calls lua_pcall(Lua, 0, 0, 0)
  * @return true on success, false on error
  */
  bool Eval();

  /**
  * Loads a *.lua-sourcefile
  * Set your default (local) lua-vars before you call this function!
  * @param filename lua-file to load
  * @return true on success, false on error
  */
  bool Load(const char *filename);
  bool Load(const std::string &filename);

  /**
  * Gets the value of a variable.
  * @param name of the variable inside loaded lua state
  * @param result value is saved in this variable
  * @return true on success, false on error
  */
  bool Get(const char *name, int &result);
  bool Get(const char *name, std::string &result);
  bool Get(const char *name, double &result);
  bool Get(const char *name, float &result);
  bool Get(const char *name, bool &result);

  /**
  * Sets the value of global lua-var.
  * @param name of the variable
  * @param value the var's value
  * @return true on success, false on error
  */
  bool Set(const char *name, const char *value);
  bool Set(const char *name, int value);
  bool Set(const char *name, const double value);
  bool Set(const char *name, const float value);
  bool Set(const char *name, const bool value);

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
  bool Push(double number);
  bool Push(const char *string);
  bool Push(const std::string &string) { return this->Push(string.c_str()); }

  /**
  * Resets Lua's internal stack
  * @return true on success, false on error
  */
  bool Flush();

  /**
  * After calling debug(),
  * lua-adapter will print out debug-information for each following
  * function-call.
  */
  void Debug() { this->print = true; }
  /**
  * Set debug output on or off
  * @param mode if mode==true, see debug()
  */

  bool IsDebugging() const { return this->print; }

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

  lua_State *const GetLuaState() const { return this->Lua; }

  /**
  * "Closes" the loaded *.lua-sourcefile
  * This function is called by the destructor of this class!
  * */
  void Close();

private:
  lua_State *Lua;
  bool print;
  bool single;

  /**
  * Gets the value of a globally loaded lua-variable
  * @param name Name of the vairable
  * @return true on success, false on error
  */
  bool GetGlobal(const char *name);
};
#endif

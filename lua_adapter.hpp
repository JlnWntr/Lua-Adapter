/*
 * Copyright (c) 2015-2017 JlnWntr (jlnwntr@gmail.com)
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
#include <lua5.3/lauxlib.h>
#include <lua5.3/lua.hpp>
#include <lua5.3/lualib.h>

typedef int (*Lua_callback_function)(lua_State *L);

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
  LuaAdapter(const std::string filename);

  /**
  * Destructor
  */
  ~LuaAdapter();

  /**
  * Initialize Lua and all its libs
  * Does NOT load any lua-sourcefile
  * LuaAdapter calls this internally single-handed.
  */
  void init();

  /**
  * Calls lua_pcall(Lua, 0, 0, 0)
  * @return true on success, false on error
  */
  bool eval();

  /**
  * Loads a *.lua-sourcefile
  * Set your default (local) lua-vars before you call this function!
  * @param filename lua-file to load
  * @return true on success, false on error
  */
  bool load(const char *filename);
  bool load(const std::string filename);

  /**
  * @param name Name of the variable inside loaded .lua-file
  * @param result value is saved in this variable
  * @return true on success, false on error
  */
  bool get(const char *name, int &result);
  bool get(const char *name, std::string &result);
  bool get(const char *name, double &result);
  bool get(const char *name, float &result);
  bool get(const char *name, bool &result);

  /**
  * Opens a lua-table
  * @param name Name of the table inside loaded .lua-file
  * @return true on success, false on error
  */
  bool openTable(const char *name);
  bool openTable(std::string name) { return this->openTable(name.c_str()); }

  /**
  * Opens a table(-field) inside a table
  * @param name Name of the table inside the opened table
  * @return true on success, false on error
  */
  bool openNestedTable(const char *name);
  bool openNestedTable(std::string name) {
    return this->openNestedTable(name.c_str());
  }

  /**
  * Get the length of the current (opened) table
  * [tables with integer keys and without 'nil-holes']
  * @return the length of the table
  */
  unsigned short int getTableLength();

  /**
  * Gets a field from an opened table
  * @param name Name of the field
  * @param result value of the field
  * @return true on success, false on error
  */
  bool getField(const char *name, std::string &result);
  bool getField(const char *name, int &result);

  /**
  * Gets a field from an opened table
  * @param i i-th field (starting at 1)
  * @param result value of the field
  * @return true on success, false on error
  */
  bool getField(unsigned short int i, int &result);
  bool getField(unsigned short int i, double &result);
  bool getField(unsigned short int i, float &result);

  /**
  * Gets a ("2D"-)field value from an opened table.
  * NOTE: DO NOT CALL openNestedTable(name) for this!
  * Example: identity ={
  *               {1, 0, 0},
  *               {0, 1, 0},
  *               {0, 0, 1},
  *           }
  * @param j row
  * @param i col
  * @param result value of the field
  * @return true on success, false on error
  */
  bool getNestedField(unsigned short int j, unsigned short int i,
                      double &result);
  bool getNestedField(unsigned short int j, unsigned short int i,
                      float &result);
  bool getNestedField(unsigned short int j, unsigned short int i, int &result);
  bool getNestedField(unsigned short int j, unsigned short int i,
                      std::string &result);

  /**
  * Sets the value of global lua-var.
  * @param name of the variable
  * @param value the var's value
  * @return true on success, false on error
  */
  bool set(const char *name, const char *value);
  bool set(const char *name, int value);
  bool set(const char *name, const double value);
  bool set(const char *name, const float value);
  bool set(const char *name, const bool value);

  /**
  * Closes a table
  * Call this function after every opening (and use) of a table!
  * (Prevents "stack-smashing".)
  */
  void closeTable() { return this->pop(1); }

  /**
  * Calls a lua-function
  * @param name of the function
  * @param argc number of arguments passed to the function
  * @param args arguments
  * @param result returned value of the function
  * @return true on success, false on error
  */
  bool callFunction(const char *functionName, const unsigned short int argc,
                    int args[], int &result);

  /**
  * Calls a lua-function
  * @param name of the function
  * @param result returned value of the function
  * @return true on success, false on error
  */
  bool callFunction(const char *functionName, double &result);

  /**
  * Makes a C-/C++-function-call available for lua
  * (it's called pushFunction(), but you're not 'incrementing' the stack)
  * @param function C-/C++-function
  * @param functionName name of the function
  * @return true on success, false on error
  */
  bool pushFunction(Lua_callback_function function, const char *functionName);

  /**
  * Push data on the lua-stack. (Mind the stack!)
  * @param number number to push
  * @param string string to push
  * @return true on success, false on error
  */
  bool push(double number);
  bool push(const char *string);
  bool push(std::string string) { return this->push(string.c_str()); }

  /**
  * Resets Lua's internal stack
  * @return true on success, false on error
  */
  bool flush();

  /**
  * After calling debug(),
  * lua-adapter will print out debug-information for each following
  * function-call.
  */
  void debug() { this->setLogMode(true); }
  /**
  * Set debug output on or off
  * @param mode if mode==true, see debug()
  */
  void setLogMode(bool mode);

  /**
  * Pops i entries from Lua's internal stack
  * @param i number of entries
  */
  void pop(signed short int i = 1) { lua_pop(this->Lua, i); }

  /**
  * Gets the stack position
  * @return the stack position
  */
  int getTop() const { return lua_gettop(this->Lua); }

  /**
  * Gets the value-type of the current stack position
  * @return the type
  */
  int getType() const { return lua_type(this->Lua, 0); }

  /**
  * "Closes" the loaded *.lua-sourcefile
  * This function is called by the destructor of this class!
  * */
  void close();

private:
  lua_State *Lua;
  bool loaded;
  bool print;
  const std::string outputPrefix;

  /**
  * Gets a field from an opened table and puts its value on the stack
  * @param name Name of the field
  * @return true on success, false on error
  */
  bool getField(const char *name);

  /**
  * Gets the i-th (nested) field from an opened table
  * @param i the i-th field
  * @return true on success, false on error
  */
  bool getI(unsigned short int i);

  /**
  * Gets the value of a globally loaded lua-variable
  * @param name Name of the vairable
  * @return true on success, false on error
  */
  bool getGlobal(const char *name);
};
#endif

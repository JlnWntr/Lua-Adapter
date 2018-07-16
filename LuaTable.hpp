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

#ifndef LUA_ADAPTER_H
#include "LuaAdapter.hpp"
#endif

class LuaTable {

public:
  /**
  *Default constructor
  *@param lua uses an existing lua_state
  */
  LuaTable(LuaAdapter &lua);
  LuaTable(lua_State *const lua = nullptr);

  /**
  * Destructor
  */
  ~LuaTable();

  /**
   * (Re-)Sets the lua state
   * @param lua lua_state
  */
  void SetLuaState(lua_State *const lua){
    this->Lua = lua;
  }

  /**
  * Gets a field from an opened table and puts its value on the stack
  * @param name Name of the field
  * @return true on success, false on error
  */
  bool Get(const char *name);

  /**
   * Opens a lua-table
   * @param name Name of the table inside loaded lua state
   * @return true on success, false on error
   */
  bool Open(const char *name);
  bool Open(const std::string &name) { return this->Open(name.c_str()); }

  /**
  * Get the length of the current (opened) table
  * [tables with integer keys and without 'nil-holes']
  * @return the length of the table
  */
  unsigned short int Length();

  /**
  * Gets a field from an opened table
  * @param name Name of the field
  * @param result value of the field
  * @return true on success, false on error
  */
  bool Get(const char *name, std::string &result);
  bool Get(const char *name, int &result);

  /**
  * Gets a field from an opened table
  * @param i i-th field (starting at 1)
  * @param result value of the field
  * @return true on success, false on error
  */
  bool Get(unsigned short int i, int &result);
  bool Get(unsigned short int i, double &result);
  bool Get(unsigned short int i, float &result);
  bool Get(unsigned short int i, std::string &result);

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
  bool Get(unsigned short int j, unsigned short int i, double &result);
  bool Get(unsigned short int j, unsigned short int i, float &result);
  bool Get(unsigned short int j, unsigned short int i, int &result);
  bool Get(unsigned short int j, unsigned short int i, std::string &result);
  /**
  * Like above but gets a ("3D"-)field value.
  * @param k row
  * @param j col
  * @param i inner table
  * @param result value
  * @return true on success, false on error
  */
  bool Get(unsigned short int k, unsigned short int j, unsigned short int i,
           int &result);

  /**
  *Closes a table
  *Call this function after every opening (and use) of a table!
  *(Prevents "stack-smashing".)
  */
  void Close() { lua_pop(this->Lua, 1); }

private:
  /**
  * Opens a table(-field) inside a table
  * @param name Name of the table inside the opened table
  * @return true on success, false on error
  */
  bool OpenNested(const char *name);
  bool OpenNested(const std::string &name) {
    return this->OpenNested(name.c_str());
  }

  /**
  *Gets the i-th (nested) field from an opened table
  *@param i the i-th field
  *@return true on success, false on error
  */
  bool GetI(unsigned short int i);

  lua_State *Lua;
  const bool print;
};
#endif
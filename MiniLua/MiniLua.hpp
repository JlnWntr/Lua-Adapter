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
#ifndef MINI_LUA_H
#define MINI_LUA_H

#include <iostream>
#include <lua.hpp>

class MiniLua {

public:
  /**
  * Default constructor
  * @param filename .lua-file to load
  */
  MiniLua(const std::string &filename = "");

  /**
  * Gets the value of a variable.
  * @param name of the variable inside loaded .lua-file
  * @param result value is saved in this variable
  * @return true on success, false on error
  */
  bool Get(const char *name, int &result);
  bool Get(const char *name, std::string &result);
  bool Get(const char *name, double &result);
  bool Get(const char *name, float &result);
  bool Get(const char *name, bool &result);

  /**
  * Closes lua state
  */
  void Close();

  /**
  * Destructor
  * calls Close()
  */
  ~MiniLua();

private:
  /**
  * Gets the value of a globally loaded lua-variable
  * @param name Name of the variable
  * @return true on success, false on error
  */
  bool GetGlobal(const char *name);

  lua_State *Lua;
};
#endif

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

#ifndef LUA_FUNCTION_H
#define LUA_FUNCTION_H

#ifndef LUA_ADAPTER_H
#include "LuaAdapter.hpp"
#endif

typedef int (*Lua_callback_function)(lua_State *L);

class LuaFunction {

public:
  /**
  *Default constructor
  *@param lua uses an existing lua_state
  */
  LuaFunction(LuaAdapter &lua);
  LuaFunction(lua_State *const lua = nullptr);



  /**
  * Destructor
  */
  ~LuaFunction();

  /**
   * (Re-)Sets the lua state
   * @param lua lua_state
  */
  void SetLuaState(lua_State *const lua){
    this->Lua = lua;
  }

  /**
   * Calls a lua-function
   * @param name of the lua-function
   * @param argc number of arguments passed to the lua-function
   * @param args function-arguments
   * @param result new value from the lua-function
   * @return true on success, false on error
   */
  bool Call(const char *function_name, const unsigned short int argc,
            const int args[], int &result);
  /**
  * Calls a lua-function
  * @param function_name of the lua-function
  * @param string a string-argument
  * @param length of this string
  * @return true on success, false on error
  */
  bool Call(const char *function_name, const char *const string,
            const size_t length);

  /**
  * Calls a lua-function
  * @param function_name of the lua-function
  * @return true on success, false on error
  */
  bool Call(const char *function_name);

  /**
  * Calls a lua-function
  * @param function_name of the lua-function
  * @param string a string-argument
  * @param length of this string
  * @param result new value (string) from the lua-function
  * @return true on success, false on error
  */
  bool Call(const char *function_name, const char *const string, size_t &length,
            std::string &result);

  /**
  * Calls a lua-function
  * @param name of the lua-function
  * @param result new value from the lua-function
  * @return true on success, false on error
  */
  bool Call(const char *function_name, double &result);
  bool Call(const char *function_name, int &result);

  /**
  * Calls a lua-function
  * @param name of the lua-function
  * @param integer argument
  * @param result new value from the lua-function
  * @return true on success, false on error
  */
    bool Call(const char *function_name, const int arg, int &result);

  /**
  * Calls a lua-function
  * @param name of the lua-function
  * @param arg argument of the lua-function
  * @param result new value (string) from the lua-function
  * @return true on success, false on error
  */
  bool Call(const char *function_name, const std::string arg,
            std::string &result);

  /**
  * Makes a C-/C++-function-call available for lua
  * (it's called pushFunction(), but you're not 'incrementing' the stack)
  * @param function C-/C++-function
  * @param function_name name of the function
  * @return true on success, false on error
  */
  bool Push(Lua_callback_function function, const char *function_name);



private:
  lua_State *Lua;

  /**
  * Calls lua's pcall and outputs errors.
  * @param nargs number of args
  * @param nresults number of results
  * @param int msgh see https://www.lua.org/manual/5.3/manual.html#lua_pcall
  * @return true on success, false on error
  */
  bool Pcall(int nargs, int nresults, int msgh);
};
#endif
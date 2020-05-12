/*
* Copyright (c) 2015-2020 JlnWntr (jlnwntr@gmail.com)
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

#include <lua.hpp>
#include <string>

typedef int (*Lua_callback_function)(lua_State *L);
static int LUA_ADAPTER_NULL{};

class MiniLua {
public:
    /**
    * Default constructor, opens lua libs
    */
    MiniLua();

    /**
    * This constructor open all the lua libs and a lua-file
    * @param filename .lua-file to load
    */
    MiniLua(const std::string &filename);

    /**
    * Loads (and runs) lua-file.
    * @param filename .lua-file to load
    * @return true on success, false on error
    */
    bool Load(const std::string &filename);

    /**
    * Runs precompiled lua-bytecode.
    * @param bytecode luac output
    * @param length number of bytes
    * @return true on success, false on error
    */
    bool Load(const char *bytecode, const size_t length);

    /**
    * Gets the value of a lua-variable.
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
     * Calls a Lua-function
     * @param f name of the Lua-function
     * @param c number of arguments passed to the Lua-function
     * @param a function arguments
     * @param r new value returned from the called Lua-function
     * @return true on success, false on error
     */
    bool Call(const char *f, const unsigned short int c, const int *a, int &r = LUA_ADAPTER_NULL);
    bool Call(const char *f, const unsigned short int c, const float *a, float &r);
    bool Call(const char *f, const int a);
    bool Call(const char *f);

    /**
     * Makes a C-/C++-function available for Lua
     * (It's called pushFunction(), but you're not 'incrementing' the stack)
     * @param function C-/C++-function
     * @param name of the function
     * @return true on success, false on error
     */
    bool Push(Lua_callback_function function, const char *name);

    /**
    * Closes the lua state
    */
    void Close();

    /**
     * Close()s the old lua state and creates a new one.
     * This should be handy, if you want to re-use a class-instance.
     * You do NOT have to call this function for a newly constructed instance of MiniLua.
     * @return true if an internal lua_State is available, false on error
     */
    bool New();

    /**
     * Returns recent lua state
     * @return lua_State
     */
    lua_State *const GetState() const {
        return this->Lua;
    }


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

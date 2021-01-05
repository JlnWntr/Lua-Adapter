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
#ifndef LUA_ADAPTER_H
#define LUA_ADAPTER_H

#ifdef LUA_ADAPTER_DEBUG
#include <iostream>
#define LUA_ADAPTER_PREFIX "Lua > "
// #warning Debug-information will be displayed during execution!
#endif

#include <lua.hpp>
#include <memory>
#include <string>
#include <cstring>

class LuaTable;

class LuaState
{
private:
    lua_State *const lua;

public:
    LuaState(lua_State *const l) : lua{l} {}
    LuaState() : lua{luaL_newstate()}
    {
        luaL_openlibs(this->lua);
    }
    ~LuaState()
    {
        lua_close(this->lua);
    }
    LuaState &operator=(const LuaState &) = delete;
    LuaState(const LuaState &) = delete;
    lua_State *const Lua() const
    {
        return this->lua;
    };
};

class LuaAdapter
{
public:
    /**
     * Default-Constructor
     */
    LuaAdapter() : Lua{std::make_shared<LuaState>()} {}

    /**
     * Constructor
     * @param lua uses an pre-existing lua_state.
     * (See for example testCFunction() in test.cpp)
     */
    LuaAdapter(lua_State *const lua) : Lua{std::make_shared<LuaState>(lua)} {}
    LuaAdapter(LuaAdapter &lua) : Lua{lua.GetLuaState()} {}
    LuaAdapter(const LuaAdapter &lua) : Lua{lua.GetLuaState()} {}

    /**
     * This constructor inits Lua and loads a .Lua-file.
     * @param filename .Lua-file to load
     */
    LuaAdapter(const std::string &filename) : Lua{std::make_shared<LuaState>()}
    {
        this->Load(filename);
    }

    /**
     * Destructor
     */
    ~LuaAdapter() {}

    /**
     * Loads and interprets Lua-code.
     * If length is given, then LuaAdapter will regard the given bytestring (code)
     * as 'precompiled' Lua-code.*
     * @param code Lua-Code
     * @param length amounts of bytes
     * @return true on success, false on error
     */
    bool Load(const std::string &filename)
    {
        return Load(filename.c_str());
    }
    bool Load(const char *code, const size_t length = 0)
    {
        if (not this->Lua.get()
                or  not this->Lua.get()->Lua()
                or  not code
                or  ( (length == 0) and
                      (luaL_loadfile(this->Lua.get()->Lua(), code) != 0))
                or  ( not(length == 0) and
                      not(luaL_loadbuffer(this->Lua.get()->Lua(), code, length, nullptr) == 0))
           )
        {
#ifdef LUA_ADAPTER_DEBUG
            std::cerr << LUA_ADAPTER_PREFIX << "Error. Could not load '";
            std::cerr << code << "'" << std::endl;
            std::cerr << lua_tostring(this->Lua.get()->Lua(), -1);
            std::cerr << std::endl;
#endif
            return false;
        }
        if (lua_pcall(this->Lua.get()->Lua(), 0, 0, 0) == 0) return true;
#ifdef LUA_ADAPTER_DEBUG
        std::cerr << LUA_ADAPTER_PREFIX << "Error in Lua-file ";
        std::cerr << lua_tostring(this->Lua.get()->Lua(), -1);
        std::cerr << std::endl;
#endif
        return false;
    }

    /**
     * Gets the value of a global variable.
     * @param name of the variable inside loaded Lua-state
     * @param r value is saved in this variable
     * @return true on success, false on error
     */
    template <typename R>
    bool Get(const char *name, R &r)
    {
        if (not this->Lua.get()
                or  not this->Lua.get()->Lua()
                or  not name) return false;

        switch (lua_getglobal(this->Lua.get()->Lua(), name))
        {
        case LUA_TNUMBER:
            if (lua_isinteger(this->Lua.get()->Lua(), -1))
            {
                if constexpr (std::is_same_v<R, int>)
                    r = lua_tointeger(this->Lua.get()->Lua(), -1);
            }
            else if constexpr (std::is_same_v<double, R> or
                               std::is_same_v<R, float>)
                r = lua_tonumber(this->Lua.get()->Lua(), -1);
            break;
        case LUA_TBOOLEAN:
            if constexpr (std::is_same_v<R, bool>)
                r = lua_toboolean(this->Lua.get()->Lua(), -1);
            break;
        case LUA_TSTRING:
            if constexpr (std::is_same_v<R, std::string>)
                r = lua_tostring(this->Lua.get()->Lua(), -1);
            break;
        default:
            return false;
            break;
        }
#ifdef LUA_ADAPTER_DEBUG
        std::cout << LUA_ADAPTER_PREFIX << "got '" << name << "' = '" << r << "'"
                  << std::endl;
#endif
        lua_pop(this->Lua.get()->Lua(), 1);
        return true;
    }

    /**
     * Sets the value of a global Lua-variable.
     * @param name of the variable
     * @param a the value of the variable
     * @return true on success, false on error
     */
    template <typename A>
    bool Set(const char *name, const A &a)
    {
        if (not this->Lua.get()
                or  not this->Lua.get()->Lua()
                or  not name)
            return false;

        if (this->Push(a) == true)
        {
            lua_setglobal(this->Lua.get()->Lua(), name);

#ifdef LUA_ADAPTER_DEBUG
            std::cout << LUA_ADAPTER_PREFIX << "set '" << name << "' = '" << a << "'"
                      << std::endl;
#endif
            return true;
        }
        return false;
    }

    /**
     * Execute any string
     * @param string to execute, for example "test = io.read()"
     * @return true on success, false on error
     */
    bool DoString(const char *string)
    {
        if (not this->Lua.get() or not this->Lua.get()->Lua()) return false;
        return luaL_dostring(this->Lua.get()->Lua(), string);
    }

    /**
     * Push data on the lua stack.
     * @param a variable to push
     * @return true on success, false on error
     */
    template <typename A>
    bool Push(A a)
    {
        if (not this->Lua.get()
                or  not this->Lua.get()->Lua()) return false;
        if constexpr (std::is_same_v<A, int>)
            lua_pushinteger(this->Lua.get()->Lua(), a);
        else if constexpr (std::is_same_v<A, float> or std::is_same_v<A, double>)
            lua_pushnumber(this->Lua.get()->Lua(), a);
        else if constexpr (std::is_same_v<A, bool>)
            lua_pushboolean(this->Lua.get()->Lua(), a);
        else if constexpr (std::is_same_v<A, std::string>)
            lua_pushlstring(this->Lua.get()->Lua(), a.c_str(), a.length());
        else if constexpr (std::is_same_v<A, const char*>)
            lua_pushlstring(this->Lua.get()->Lua(), a, strlen(a));
        else
            return false;
        return true;
    }

    /**
     * Resets Lua's internal stack
     * @return true on success, false on error
     */
    bool Flush()
    {
        if (not this->Lua.get()
                or  not this->Lua.get()->Lua()) return false;
        lua_settop(this->Lua.get()->Lua(), 0);
        return true;
    }

    /**
     * Pops i entries from Lua's internal stack
     * @param i number of entries
     */
    void Pop(int i = 1)
    {
        if (this->Lua.get()
                and this->Lua.get()->Lua())
            lua_pop(this->Lua.get()->Lua(), i);
    }

    /**
     * Gets the stack position
     * @return the stack position
     */
    int GetTop() const
    {
        if (not this->Lua.get()
                or  not this->Lua.get()->Lua()) return false;
        return lua_gettop(this->Lua.get()->Lua());
    }

    /**
     * Gets the value type of the current stack position
     * (LUA_TNIL (0), LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE,
     *  LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.)
     * [https://www.lua.org/manual/5.3/manual.html#lua_type]
     * @return the type
     */
    int GetType() const
    {
        if (not this->Lua.get()
                or  not this->Lua.get()->Lua()) return false;
        return lua_type(this->Lua.get()->Lua(), 0);
    }

    /**
     * Returns the current LuaState which is used
     * This is necessary,
     * because you need this state for LuaFunctions or LuaTables.
     * @return the current LuaState
     */
    std::shared_ptr<LuaState> GetLuaState() const
    {
        return this->Lua;
    }

private:
    std::shared_ptr<LuaState> Lua;
};
#endif

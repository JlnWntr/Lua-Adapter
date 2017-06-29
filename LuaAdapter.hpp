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
#include <lua5.3/lua.hpp>


typedef int(*Lua_callback_function)(lua_State *L);

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
	LuaAdapter(const std::string& filename);

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
	bool Load(const std::string& filename);

	/**
	* @param name Name of the variable inside loaded .lua-file
	* @param result value is saved in this variable
	* @return true on success, false on error
	*/
	bool Get(const char *name, int &result);
	bool Get(const char *name, std::string &result);
	bool Get(const char *name, double &result);
	bool Get(const char *name, float &result);
	bool Get(const char *name, bool &result);

	/**
	* Opens a lua-table
	* @param name Name of the table inside loaded .lua-file
	* @return true on success, false on error
	*/
	bool OpenTable(const char *name);
	bool OpenTable(const std::string& name) { return this->OpenTable(name.c_str()); }

	/**
	* Opens a table(-field) inside a table
	* @param name Name of the table inside the opened table
	* @return true on success, false on error
	*/
	bool OpenNestedTable(const char *name);
	bool OpenNestedTable(const std::string& name) {
		return this->OpenNestedTable(name.c_str());
	}

	/**
	* Get the length of the current (opened) table
	* [tables with integer keys and without 'nil-holes']
	* @return the length of the table
	*/
	unsigned short int GetTableLength();

	/**
	* Gets a field from an opened table
	* @param name Name of the field
	* @param result value of the field
	* @return true on success, false on error
	*/
	bool GetField(const char *name, std::string &result);
	bool GetField(const char *name, int &result);

	/**
	* Gets a field from an opened table
	* @param i i-th field (starting at 1)
	* @param result value of the field
	* @return true on success, false on error
	*/
	bool GetField(unsigned short int i, int &result);
	bool GetField(unsigned short int i, double &result);
	bool GetField(unsigned short int i, float &result);

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
	bool GetNestedField(unsigned short int j, unsigned short int i,
		double &result);
	bool GetNestedField(unsigned short int j, unsigned short int i,
		float &result);
	bool GetNestedField(unsigned short int j, unsigned short int i, int &result);
	bool GetNestedField(unsigned short int j, unsigned short int i,
		std::string &result);

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
	* Closes a table
	* Call this function after every opening (and use) of a table!
	* (Prevents "stack-smashing".)
	*/
	void CloseTable() { return this->Pop(1); }
        
    /**
	* Execute any string	
    * @param string to execute, for example "test = io.read()"
	*/
    bool DoString(const char *string){
        return luaL_dostring(this->Lua, string);      
    }

	/**
	* Calls a lua-function
	* @param name of the function
	* @param argc number of arguments passed to the function
	* @param args arguments
	* @param result returned value of the function
	* @return true on success, false on error
	*/
	bool CallFunction(const char *functionName, const unsigned short int argc,
		int args[], int &result);

	/**
	* Calls a lua-function
	* @param name of the function
	* @param result returned value of the function
	* @return true on success, false on error
	*/
	bool CallFunction(const char *functionName, double &result);

	/**
	* Makes a C-/C++-function-call available for lua
	* (it's called pushFunction(), but you're not 'incrementing' the stack)
	* @param function C-/C++-function
	* @param functionName name of the function
	* @return true on success, false on error
	*/
	bool PushFunction(Lua_callback_function function, const char *functionName);

	/**
	* Push data on the lua-stack. (Mind the stack!)
	* @param number number to push
	* @param string string to push
	* @return true on success, false on error
	*/
	bool Push(double number);
	bool Push(const char *string);
	bool Push(const std::string& string) { return this->Push(string.c_str()); }

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
	void Debug() { this->SetLogMode(true); }
	/**
	* Set debug output on or off
	* @param mode if mode==true, see debug()
	*/
	void SetLogMode(bool mode);

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
	* "Closes" the loaded *.lua-sourcefile
	* This function is called by the destructor of this class!
	* */
	void Close();

private:
	lua_State *Lua;	
	bool print;
    bool single;
	const std::string outputPrefix;

	/**
	* Gets a field from an opened table and puts its value on the stack
	* @param name Name of the field
	* @return true on success, false on error
	*/
	bool GetField(const char *name);

	/**
	* Gets the i-th (nested) field from an opened table
	* @param i the i-th field
	* @return true on success, false on error
	*/
	bool GetI(unsigned short int i);

	/**
	* Gets the value of a globally loaded lua-variable
	* @param name Name of the vairable
	* @return true on success, false on error
	*/
	bool GetGlobal(const char *name);
};
#endif

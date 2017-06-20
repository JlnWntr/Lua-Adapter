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
#include "LuaAdapter.hpp"
#endif


LuaAdapter::LuaAdapter(lua_State *const lua)
	: Lua{ lua }, loaded{ false }, print{ false }, outputPrefix{ "Lua > " } {
	this->Init();
}

LuaAdapter::LuaAdapter(const std::string& filename)
	: Lua{ nullptr }, loaded{ false }, print{ false }, outputPrefix{ "Lua > " } {
	this->Init();
	this->Load(filename);
}

void LuaAdapter::Init() {
	if (this->Lua)
		return;
	this->Lua = luaL_newstate();
	luaL_openlibs(this->Lua);
}

bool LuaAdapter::Load(const char *filename) {
	this->Init();

	if ((luaL_loadfile(this->Lua, filename) == 0) && this->Eval()) {
		this->loaded = true;
		return true;
	}
	std::cout << this->outputPrefix << "Error in Lua-file '";
	std::cout << filename << "': ";
	std::cout << lua_tostring(this->Lua, -1);
	std::cout << "\n";

	this->loaded = false;
	return false;
}

bool LuaAdapter::Eval() {
	if (this->print == false)
		return ((this->Lua) && (lua_pcall(this->Lua, 0, 0, 0) == 0));

	if (!this->Lua) {
		std::cout << this->outputPrefix
			<< "Error: Lua-state invalid. Call init() first!\n";
		return false;
	}
	const int pcall{ lua_pcall(this->Lua, 0, 0, 0) };
	if (pcall == 0)
		return true;
	std::cout << this->outputPrefix << "Error: pcall failed. Code: ";
	std::cout << pcall;
	std::cout << ", '" << lua_tostring(this->Lua, -1) << "'\n";
	/* LUA_ERRRUN: a runtime error. (2)
	* LUA_ERRMEM: memory allocation error. (4)
	* LUA_ERRERR: error while running the error handler function. (5)
	*/
	return false;
}

bool LuaAdapter::Load(const std::string& filename) {
	return this->Load(filename.c_str());
}

bool LuaAdapter::GetField(const char *name) {
	if ((!this->Lua) || (!this->loaded) || (!name) ||
		(!lua_istable(this->Lua, -1)))
		return false;

	lua_getfield(this->Lua, -1, name);
	return true;
}

bool LuaAdapter::GetI(unsigned short int i) {
	if ((!this->Lua) || (!this->loaded) || (!lua_istable(this->Lua, -1)) ||
		(i < 1))
		return false;

	lua_rawgeti(this->Lua, -1, i);
	return true;
}

bool LuaAdapter::GetGlobal(const char *name) {
	if ((!this->Lua)
		//|| (!this->loaded)
		|| (!name))
		return false;

	lua_getglobal(this->Lua, name); // void
	return true;
}

bool LuaAdapter::Get(const char *name, int &result) {
	if (this->GetGlobal(name) == false)
		return false;

	if (lua_isnumber(this->Lua, -1) == false) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tointeger(this->Lua, -1);
	if (this->print)
		std::cout << this->outputPrefix << "get int '" << name << "' = '" << result
		<< "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::Get(const char *name, std::string &result) {
	if (this->GetGlobal(name) == false)
		return false;

	if (lua_type(this->Lua, -1) != LUA_TSTRING) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tostring(this->Lua, -1);
	if (this->print)
		std::cout << this->outputPrefix << "get string '" << name << "' = '"
		<< result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::Get(const char *name, double &result) {
	if (this->GetGlobal(name) == false)
		return false;

	if (lua_isnumber(this->Lua, -1) == false) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tonumber(this->Lua, -1);
	if (this->print)
		std::cout << this->outputPrefix << "get double '" << name << "' = '"
		<< result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::Get(const char *name, float &result) {
	double temp = 0.0;
	if (this->Get(name, temp) == false)
		return false;
	result = (float)temp;
	return true;
}

bool LuaAdapter::Get(const char *name, bool &result) {
	if (this->GetGlobal(name) == false)
		return false;

	if (lua_isboolean(this->Lua, -1) == false) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_toboolean(this->Lua, -1);
	if (this->print)
		std::cout << this->outputPrefix << "get boolean '" << name << "' = '"
		<< result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::OpenTable(const char *name) {
	if (this->GetGlobal(name) && lua_istable(this->Lua, -1)) {
		if (this->print)
			std::cout << this->outputPrefix << "open table '" << name << "' \n";
		return true;
	}
	this->Pop();
	return false;
}

bool LuaAdapter::OpenNestedTable(const char *name) {
	if ((!this->Lua) || (!this->loaded)) {
		return false;
	}
	if (this->print)
		std::cout << "\t" << this->outputPrefix << "opening nested table '" << name
		<< "' ... ";

	if (lua_istable(this->Lua, -1) == false) {
		return false;
	}

	lua_getfield(this->Lua, -1, name);

	if (lua_isnil(this->Lua, -1)) {
		this->Pop();
		return false;
	}

	if (lua_istable(this->Lua, -1)) {
		if (this->print)
			std::cout << "ok! \n";
		return true;
	}
	this->Pop();
	return false;
}

unsigned short int LuaAdapter::GetTableLength() {
	unsigned short int result{ 0 };
	if ((!this->Lua) || (!this->loaded)) {
		return result;
	}
	if (lua_istable(this->Lua, -1) == false) {
		return result;
	}
	lua_len(this->Lua, -1);
	result = lua_tointeger(this->Lua, -1);
	this->Pop();
	return result;
}

bool LuaAdapter::Flush() {
	if ((!this->Lua) || (!this->loaded))
		return false;

	lua_settop(this->Lua, 0);
	return true;
}

void LuaAdapter::SetLogMode(bool mode) { this->print = mode; }

bool LuaAdapter::GetField(const char *name, std::string &result) {
	if (this->GetField(name) == false)
		return false;

	if (lua_type(this->Lua, -1) != LUA_TSTRING) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tostring(this->Lua, -1);
	if (this->print)
		std::cout << "\t" << this->outputPrefix << "get string-field '" << name
		<< "' = '" << result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::GetField(const char *name, int &result) {
	if (this->GetField(name) == false)
		return false;
	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tointeger(this->Lua, -1);
	if (this->print)
		std::cout << "\t" << this->outputPrefix << "get int-field '" << name
		<< "' = '" << result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::GetField(unsigned short int i, int &result) {
	if (this->GetI(i) == false)
		return false;

	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tointeger(this->Lua, -1);
	if (this->print)
		std::cout << "\t" << this->outputPrefix << "get int-field " << i << " = '"
		<< result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::GetField(unsigned short int i, double &result) {
	if (this->GetI(i) == false)
		return false;

	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tonumber(this->Lua, -1);
	if (this->print)
		std::cout << "\t" << this->outputPrefix << "get double-field " << i
		<< " = '" << result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::GetField(unsigned short int i, float &result) {
	if (this->GetI(i) == false)
		return false;

	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = (float)lua_tonumber(this->Lua, -1);
	if (this->print)
		std::cout << "\t" << this->outputPrefix << "get float-field " << i << " = '"
		<< result << "' \n";
	lua_pop(this->Lua, 1);
	return true;
}

bool LuaAdapter::Set(const char *name, const char *value) {
	if (!this->Lua)
		return false;
	lua_pushstring(this->Lua, value);
	lua_setglobal(this->Lua, name);
	if (this->print)
		std::cout << this->outputPrefix << "set string '" << name << "' = '"
		<< value << "' \n";
	return true;
}

bool LuaAdapter::Set(const char *name, int value) {
	if (!this->Lua) {
		return false;
	}
	lua_pushnumber(this->Lua, value);
	lua_setglobal(this->Lua, name);
	if (this->print)
		std::cout << this->outputPrefix << "set int '" << name << "' = '" << value
		<< "' \n";
	return true;
}

bool LuaAdapter::Set(const char *name, const double value) {
	if (!this->Lua)
		return false;
	lua_pushnumber(this->Lua, value);
	lua_setglobal(this->Lua, name);
	if (this->print)
		std::cout << this->outputPrefix << "set double '" << name << "' = '"
		<< value << "' \n";
	return true;
}

bool LuaAdapter::Set(const char *name, const bool value) {
	if (!this->Lua)
		return false;
	lua_pushboolean(this->Lua, value);
	lua_setglobal(this->Lua, name);
	if (this->print)
		std::cout << this->outputPrefix << "set boolean '" << name << "' = '"
		<< value << "' \n";
	return true;
}

bool LuaAdapter::Set(const char *name, const float value) {
	return this->Set(name, (double)value);
}

bool LuaAdapter::GetNestedField(unsigned short int j, unsigned short int i,
	double &result) {
	if (this->GetI(j) == false) {
		return false;
	}
	if (this->GetI(i) == false) {
		lua_pop(this->Lua, 1);
		return false;
	}
	if (lua_isnumber(this->Lua, -1))
		result = lua_tonumber(this->Lua, -1);
	if (this->print)
		std::cout << "\t"
		<< "\t" << this->outputPrefix << "get nested double-field (" << j
		<< "|" << i << ")' = '" << result << "' \n";
	lua_pop(this->Lua, 2);
	return true;
}

bool LuaAdapter::GetNestedField(unsigned short int j, unsigned short int i,
	float &result) {
	if (this->GetI(j) == false) {
		return false;
	}
	if (this->GetI(i) == false) {
		lua_pop(this->Lua, 1);
		return false;
	}
	if (lua_isnumber(this->Lua, -1))
		result = (float)lua_tonumber(this->Lua, -1);
	if (this->print)
		std::cout << "\t"
		<< "\t" << this->outputPrefix << "get nested float-field (" << j
		<< "|" << i << ")' = '" << result << "' \n";
	lua_pop(this->Lua, 2);
	return true;
}

bool LuaAdapter::GetNestedField(unsigned short int j, unsigned short int i,
	int &result) {
	if (this->GetI(j) == false) {
		return false;
	}
	if (this->GetI(i) == false) {
		lua_pop(this->Lua, 1);
		return false;
	}
	if (lua_isnumber(this->Lua, -1))
		result = lua_tointeger(this->Lua, -1);
	if (this->print)
		std::cout << "\t"
		<< "\t" << this->outputPrefix << "get nested int-field (" << j
		<< "|" << i << ")' = '" << result << "' \n";
	lua_pop(this->Lua, 2);
	return true;
}

bool LuaAdapter::GetNestedField(unsigned short int j, unsigned short int i,
	std::string &result) {
	if (this->GetI(j) == false) {
		return false;
	}
	if (this->GetI(i) == false) {
		lua_pop(this->Lua, 1);
		return false;
	}
	if (lua_isstring(this->Lua, -1))
		result = lua_tostring(this->Lua, -1);
	if (this->print)
		std::cout << "\t"
		<< "\t" << this->outputPrefix << "get nested string-field (" << j
		<< "|" << i << ")' = '" << result << "' \n";
	lua_pop(this->Lua, 2);
	return true;
}

bool LuaAdapter::CallFunction(const char *functionName,
	const unsigned short int argc, int args[],
	int &result) {
	if (!this->loaded) {
		return false;
	}
	lua_getglobal(this->Lua, functionName);

	for (unsigned char i = 0; i < argc; i++)
		if (args + i)
			lua_pushnumber(this->Lua, args[i]);

	if ((lua_pcall(this->Lua, argc, 1, 0) != LUA_OK) ||
		(lua_isnumber(this->Lua, -1) == false)) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tointeger(this->Lua, -1);
	lua_pop(this->Lua, 1);
	return false;
}

bool LuaAdapter::CallFunction(const char *functionName, double &result) {
	if (!this->loaded) {
		return false;
	}
	lua_getglobal(this->Lua, functionName);

	if ((lua_pcall(this->Lua, 0, 1, 0) != LUA_OK) ||
		(lua_isnumber(this->Lua, -1) == false)) {
		lua_pop(this->Lua, 1);
		return false;
	}
	result = lua_tonumber(this->Lua, -1);
	lua_pop(this->Lua, 1);
	return false;
}

bool LuaAdapter::PushFunction(Lua_callback_function function,
	const char *functionName) {
	if (!this->Lua)
		return false;

	lua_pushcfunction(this->Lua, function);
	lua_setglobal(this->Lua, functionName);
	return true;
}

bool LuaAdapter::Push(double number) {
	if (!this->Lua)
		return false;

	lua_pushnumber(this->Lua, number);
	return true;
}

bool LuaAdapter::Push(const char *string) {
	if (!this->Lua)
		return false;

	lua_pushstring(this->Lua, string);
	return true;
}

void LuaAdapter::Close() {
	if ((!this->Lua) || (!this->loaded))
		return;
	this->Flush();
	lua_close(this->Lua);

	this->loaded = false;
	this->Lua = nullptr;
}

LuaAdapter::~LuaAdapter() { this->Close(); }

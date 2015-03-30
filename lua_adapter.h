#ifndef LUA_ADAPTER_H
	#define LUA_ADAPTER_H
	
#include <iostream>

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>



typedef int (*Lua_callback_function)(lua_State *L); 

class LuaAdapter {
	
	private:
		lua_State *Lua;
		bool loaded;	
	
	public:	
	
		LuaAdapter();		
		
		bool load(const char*);			
		void pushFunction(Lua_callback_function, const char*) ;
		
		int getInteger(const char*);		
		std::string getString(const char*);
   
   
   		int callFunction(const char*, const unsigned char, int []);		
		
		void close();
		~LuaAdapter();
};

#endif

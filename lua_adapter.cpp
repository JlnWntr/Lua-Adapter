#ifndef LUA_ADAPTER_H
	#include "lua_adapter.h"
#endif

		LuaAdapter::LuaAdapter()
			:Lua{nullptr}
			,loaded{false}				
		{
			this->Lua = luaL_newstate();
   			luaL_openlibs(this->Lua);   			 							
		}
		
		
		bool LuaAdapter::load(const char *pFileName){
			if ( luaL_loadfile(this->Lua, pFileName) || lua_pcall(this->Lua, 0, 0, 0) ){
				 std::cout << ( "Error in Lua-Script: ");
				 std::cout << ( lua_tostring(this->Lua, -1)  );
				 std::cout << "\n";
				  this->loaded=false;
				  return false;
			 }	
			this->loaded = true;
			return true;
		}
		
		
		void LuaAdapter::pushFunction(Lua_callback_function pFunction, const char *pFunctionName) {			
			lua_pushcfunction(this->Lua, pFunction); 
  			lua_setglobal(this->Lua, pFunctionName);  
		}
		
		
		int LuaAdapter::getInteger(const char *pVarName) {
		 	if(!this->loaded)
		 		return -1;
		 		
		 	lua_getglobal(this->Lua, pVarName); 
		 	
			if ( lua_isnumber(this->Lua, -1)) {		   
				const int Result = {lua_tointeger(this->Lua, -1)};
				lua_pop(this->Lua, 1);
				return Result;
			}
			return -1;  
		} 
		
		
		std::string LuaAdapter::getString(const char *pVarName) {
		 	if(!this->loaded)
		 		return "";
		 		
		 	lua_getglobal(this->Lua, pVarName); 
		 	
			if ( lua_isstring(this->Lua, -1)) {		   
				const std::string Result = {lua_tostring(this->Lua, -1)};
				lua_pop(this->Lua, 1);
				return Result;
			}
			return "";  
		} 
   
   
   		int LuaAdapter::callFunction(const char *pFunctionName, const unsigned char argc, int args[]){
   			if(!this->loaded)
		 		return -1;
	   		lua_getglobal(this->Lua, pFunctionName);
	   		
	   		for(unsigned char i=0; i<argc; i++)
	   			lua_pushnumber(this->Lua, args[i]);

	   		if (lua_pcall(this->Lua, argc, 1, 0))
		  		return -1;

			if ( lua_isnumber(this->Lua, -1)) {		   
				const int Result = {lua_tointeger(this->Lua, -1)};
				lua_pop(this->Lua, 1);
				return Result;
			}
			return -1;  
   
		}
		
		
		void LuaAdapter::close(){
			lua_close(this->Lua); 
		}


		LuaAdapter::~LuaAdapter(){
			this->close();
		}	
		

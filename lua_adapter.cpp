#ifndef LUA_ADAPTER_H
	#include "lua_adapter.hpp"
#endif

LuaAdapter::LuaAdapter()
	:Lua{nullptr}
	,loaded{false}		
	//,error{false}		
{
	this->init();  			 							
}		

void LuaAdapter::init(){
	if(!this->Lua) {
		this->Lua = luaL_newstate();
	   	luaL_openlibs(this->Lua);
	}				
}


bool LuaAdapter::load(const char* name){
	this->init();
	
	if ( luaL_loadfile(this->Lua, name) || lua_pcall(this->Lua, 0, 0, 0) ){
		 std::cout << "Error in Lua-file '" << name << "': ";
		 std::cout << lua_tostring(this->Lua, -1);
		 std::cout << "\n";
		  this->loaded=false;
		  return false;
	 }	
	this->loaded = true;
	return true;
}


bool LuaAdapter::getField(const char* name){
	if((!this->Lua)
	|| (!this->loaded)
	|| (!name)
	|| (!lua_istable(this->Lua, -1))
	)		 		
	 	return false;		 	
	 
	lua_getfield(this->Lua, -1, name);
	return true;		
}


bool LuaAdapter::getI(unsigned short int i){
	if((!this->Lua)
	|| (!this->loaded)            
	|| (!lua_istable(this->Lua, -1))
	)		 		
	 	return false;		 	
	 
	lua_rawgeti(this->Lua, -1, i);
	return true;		
}

bool LuaAdapter::getGlobal(const char* name){
	if((!this->Lua)
	|| (!this->loaded)
	|| (!name)
	)		 		
		return false;		 	
	 	
	 lua_getglobal(this->Lua, name); 
	return true;		
}


bool LuaAdapter::get(const char *name, int &result){
	if(this->getGlobal(name)==false)
		return false;
	 
	if ( lua_isnumber(this->Lua, -1)==false) {
		lua_pop(this->Lua, 1);			
		return false;   						
	}
	result = lua_tointeger(this->Lua, -1);            
	lua_pop(this->Lua, 1);
	return true;	
}


bool LuaAdapter::get(const char *name, std::string &result) {
	 if(this->getGlobal(name)==false)
		return false;
	 
	if ( lua_isstring(this->Lua, -1)==false) {
		lua_pop(this->Lua, 1);			
		return false;   						
	}
	result = lua_tostring(this->Lua, -1);            
	lua_pop(this->Lua, 1);
	return true;
}


bool LuaAdapter::get(const char *name, double &result) {
	if(this->getGlobal(name)==false)
		return false;
	 
	if ( lua_isnumber(this->Lua, -1)==false) {
		lua_pop(this->Lua, 1);			
		return false;   						
	}
	result = lua_tonumber(this->Lua, -1);            
	lua_pop(this->Lua, 1);
	return true;		
} 


bool LuaAdapter::get(const char *name, float &result) {
	double temp = 0.0;
	if(this->get(name, temp)==false)
		return false;
	result = (float)temp;           
	return true;		
} 


bool LuaAdapter::openTable(const char *name) {		 		 	
	return this->getGlobal(name) && lua_istable(this->Lua, -1);
}


bool LuaAdapter::openNestedTable(const char *name){			
	if((!this->Lua) || (!this->loaded) ){		 		
 		return false;
 	}

	if(lua_istable(this->Lua, -1) == false) {				
		return false;
	}		
	 	
	lua_getfield(this->Lua, -1, name);
	
	if (lua_isnil(this->Lua, -1)) {
		lua_pop(this->Lua, 1);
		return false;
	}
	return lua_istable(this->Lua, -1);
}


bool LuaAdapter::flush(){
	if((!this->Lua) || (!this->loaded) )
	 	return false;
		 	
	lua_settop(this->Lua, 0);
	return true;
}
	

bool LuaAdapter::getField(const char *name, std::string &result) {
	if(this->getField(name)==false)            	 
		return false;		 		
	
	if (lua_isstring(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);	
		return false;
	}
	result = lua_tostring(this->Lua, -1);
	lua_pop(this->Lua, 1);			
	return true;
}


bool LuaAdapter::getField(const char *name, int &result) {
	if(this->getField(name)==false)            	 
	return false;            
	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);	
		return false;
	}            
	result = lua_tointeger(this->Lua, -1);
	lua_pop(this->Lua, 1);			
	return true;
}


bool LuaAdapter::getField(unsigned short int i, int &result){	
	if(this->getI(i)==false)            	 
		return false;
	
	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);	
		return false;
	}
	result = lua_tointeger(this->Lua, -1);
	lua_pop(this->Lua, 1);            
	return true;
}


bool LuaAdapter::getField(unsigned short int i, double &result){	
	if(this->getI(i)==false)            	 
		return false;
	
	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);	
		return false;
	}
	result = lua_tonumber(this->Lua, -1);
	lua_pop(this->Lua, 1);            
	return true;
}


bool LuaAdapter::getField(unsigned short int i, float &result){	
	if(this->getI(i)==false)            	 
		return false;
	
	if (lua_isnumber(this->Lua, -1) != 1) {
		lua_pop(this->Lua, 1);	
		return false;
	}
	result = (float)lua_tonumber(this->Lua, -1);
	lua_pop(this->Lua, 1);            
	return true;
}


bool LuaAdapter::set(const char *name, const char *value){
	if(!this->Lua){		 		
	 	return false;
	 }
	lua_pushstring(this->Lua, value);
	lua_setglobal(this->Lua, name);			
	return true;
}	


bool LuaAdapter::set(const char *name, int value){
	if(!this->Lua){		 		
	 	return false;
	 }
	lua_pushnumber(this->Lua, value);
	lua_setglobal(this->Lua, name);			
	return true;
}


bool LuaAdapter::set(const char *name, const double value){
	if(!this->Lua){		 		
	 	return false;
	 }
	lua_pushnumber(this->Lua, value);
	lua_setglobal(this->Lua, name);			
	return true;
}	


bool LuaAdapter::set(const char *name, const float value){
	return this->set(name, (double)value);
}


bool LuaAdapter::getNestedField(unsigned short int j, unsigned short int i, double &result){	
	if(this->getI(j)==false) {
	lua_pop(this->Lua, 1);
		return false;
	}            
	if(this->getI(i)==false) {
	lua_pop(this->Lua, 2);
	return false;
}

    result = lua_tonumber(this->Lua, -1);            
    lua_pop(this->Lua, 2);	
    return true;
}


bool LuaAdapter::getNestedField(unsigned short int j, unsigned short int i, float &result){	
	if(this->getI(j)==false) {
		lua_pop(this->Lua, 1);
		return false;
	}            
	if(this->getI(i)==false) {
		lua_pop(this->Lua, 2);
	return false;
	}
	
	result = (float)lua_tonumber(this->Lua, -1);            
	lua_pop(this->Lua, 2);	
	return true;
}


bool LuaAdapter::getNestedField(unsigned short int j, unsigned short int i, int &result){	
	if(this->getI(j)==false) {
		lua_pop(this->Lua, 1);
		return false;
	}            
	if(this->getI(i)==false) {
		lua_pop(this->Lua, 2);
		return false;
	}
	result = lua_tointeger(this->Lua, -1);            
	lua_pop(this->Lua, 2);	
	return true;
}


bool LuaAdapter::callFunction(const char *pFunctionName, const unsigned short int argc, int args[], int &result){
	if(!this->loaded) {				
		return false;
	}		
	lua_getglobal(this->Lua, pFunctionName);
	
	for(unsigned char i=0; i<argc; i++)
	   if(args + i)
	   	lua_pushnumber(this->Lua, args[i]);
	
	if (
		(lua_pcall(this->Lua, argc, 1, 0) != LUA_OK)
	||  	(lua_isnumber(this->Lua, -1) == false)
	){
		lua_pop(this->Lua, 1);				
		return false;
	}	            
	result = lua_tointeger(this->Lua, -1);
	lua_pop(this->Lua, 1);
	return false;
}


void LuaAdapter::pushFunction(Lua_callback_function pFunction, const char *pFunctionName) {			
	lua_pushcfunction(this->Lua, pFunction); 
	lua_setglobal(this->Lua, pFunctionName);  
}


void LuaAdapter::close(){
	if((!this->Lua) || (!this->loaded))
		return;
	this->flush();
	lua_close(this->Lua); 
	
	this->loaded = false;
	this->Lua = nullptr;
}


LuaAdapter::~LuaAdapter(){
	this->close();
}	


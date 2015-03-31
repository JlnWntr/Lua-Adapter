#ifndef LUA_H
	#include "lua_adapter.h"
#endif


static int functionInLua (lua_State *L){  // example function-call
		
	int n = lua_tointeger(L, 1); 
	int m = lua_tointeger(L, 2);  
	std::cout <<  "FunctionCall: " << n + m << "\n";
	return 0;	
}   		


int main (int argc, char *argv[])
{  	
 	//------------------------------------------//
 	// Usage / Test:
 	LuaAdapter lua;
 	
 	
 	// Let Lua call our C++ function later
 	lua.pushFunction(functionInLua, "functionInLua"); // see line 6
 	
 	lua.load("test.lua");	// THEN load the test script! 	
 	
 	// Using Lua for configuration:
 	std::cout << "Number: "<< lua.getInteger("number") << "\n"; 	
 	std::cout << "width: " << lua.getInteger("width") << "\n"; 	
 	std::cout << "title: " << lua.getString("title") << "\n";
 	
 	// read a table
 	if(lua.openTable("table")) {
 		std::cout << "Table [0]: " << "\n";
	 	std::cout << lua.getIntegerField("three") << "\n";
	 	std::cout << lua.getStringField("one") << "\n";
	 	std::cout << lua.getStringField("two") << "\n";
 	}
 	
 	std::cout << ("...") << lua.getString("more_text") << "\n"; 	
 	
 	// Calling a function from test.lua to compute stuff:
 	int test[] = {36, 24};
 	std::cout << "ggt1: " << lua.callFunction("ggT", 2, test) << "\n"; 	
 	
 	int test2[] = {98, 45};
 	std::cout << "ggt2: " << lua.callFunction("ggT", 2, test2) << "\n";
 
 	// Using Lua (again) for configuration:
	std::cout << "height: " << lua.getInteger("height") << "\n";
	
   	return 0;
}

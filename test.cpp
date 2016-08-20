#ifndef LUA_H
	#include "lua_adapter.hpp"
#endif

/*
static int functionInLua (lua_State *L){  // example function-call
		
	int n = lua_tointeger(L, 1); 
	int m = lua_tointeger(L, 2);  
	std::cout <<  "FunctionCall: " << n + m << "\n";
	return 0;	
}   		
*/

int main (int argc, char *argv[])
{  	
 	LuaAdapter lua;
 	
 	// Let Lua call our C++ function later
 	//lua.pushFunction(functionInLua, "functionInLua"); // see line 6
   

    // Change a global vars default value before loading test.lua
	if(lua.set("GlobalVar", 321)==false){
        std::cout << "Could not set 'GlobalVar'!\n";
    }

    // THEN load the test script! 
 	lua.load("test.lua");	

    
 	// Using Lua for configuration:
    double number {0};
    lua.get("number", number);
 	std::cout << "Number: "<< number << "\n";

    int width {0};
    lua.get("width", width);	
 	std::cout << "width: " << width << "\n";

    std::string title {""};
    lua.get("title", title);	
 	std::cout << "title: " << title << "\n";


    std::cout << "\n";
    
 	// Read a table
 	if(lua.openTable("Table1")) {

        int ID {0};
        lua.getField("ID", ID);	
        std::cout << "ID: " << ID << "\n";
	 	
	 	int Val {0};
        lua.getField("Val", Val);	
        std::cout << "Val: " << Val << "\n";
       

        lua.closeTable();
 	}
 	
 	std::cout << "\n";


    
 	// Matrix
 	if(lua.openTable("matrix")) {
 		for(unsigned char j=1; j<=3; j++){
 			for(unsigned char i=1; i<=3; i++){
                int temp = 0;
                lua.getNestedField(j, i, temp); // matrix[j][i]
		 		std::cout << temp << " ";
            }
		 	std::cout << "\n"; 
		 }
         lua.closeTable();
 	}
 	std::cout << "\n";
 	
 	
 	// Calling a function from test.lua to compute stuff:
 	int test[] = {36, 24};
    int result {0};
    lua.callFunction("gcd", 2, test, result);
 	std::cout << "gcd: " << result << "\n"; 	
 	
    std::cout << "\n";

    
	std::cout << "Lua stack top: " << lua.getTop() << "\n";


    // More table-tests
	if(lua.openTable("Table2")) { 		
       int X {0};
       lua.getField("X", X);	
       std::cout << "X: " << X << "\n";

       int Y {0};
       lua.getField("Y", Y);	
       std::cout << "Y: " << Y << "\n";
        
        if(lua.openNestedTable("Test") )  { 
            int A = 0;
            lua.getField("A", A);	
            std::cout << "\t A: " << A << "\n";
            int B = 0;
            lua.getField("B", B);	
            std::cout << "\t B: " << B << "\n";
            
            lua.closeTable();
        }
        lua.closeTable();
    }
   
    std::cout << "\nLua stack top: " << lua.getTop() << "\n";
 	std::cout << "\n"; 		
	
   	return 0;
}

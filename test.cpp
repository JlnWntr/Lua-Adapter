#ifndef LUA_H
	#include "lua_adapter.hpp"
#endif


int main (int argc, char *argv[]) {  	
    
    LuaAdapter lua;

    // Hint: You CAN change a default value of a global lua variable BEFORE loading the actual lua-file
    if(lua.set("GlobalVar", 321)==false){
        std::cout << "Could not set 'GlobalVar'!\n";
    }

    // and THEN load the script:
    lua.load("test.lua");	

    
    //get int
    int width {0};
    lua.get("width", width);	
    std::cout << "width: " << width << "\n";

    
    //get double
    double number {0};
    lua.get("number", number);
    std::cout << "Number: "<< number << "\n";


    //get string
    std::string title {"empty"};
    lua.get("title", title);	
    std::cout << "title: " << title << "\n";

    std::cout << "\n";

    // tables
    if(lua.openTable("Table1")) {
        int ID {0};
        lua.getField("ID", ID);	
        std::cout << "ID: " << ID << "\n";

        int Val {0};
        lua.getField("Val", Val);	
        std::cout << "Val: " << Val << "\n";

        lua.closeTable(); // close tables when you're done
    }

    std::cout << "\n";

   
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

    // more table-tests
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

    // Check lua's internal stack
    std::cout << "\n";
    std::cout << "Lua stack top: " << lua.getTop() << "\n";


    // calling a function from test.lua to compute stuff:
    int test[] = {36, 24};
    int result {0};
    lua.callFunction("gcd", 2, test, result);
    std::cout << "gcd: " << result << "\n"; 	


    std::cout << "\nLua stack top: " << lua.getTop() << "\n";
    std::cout << "\n"; 		

    return 0;
}

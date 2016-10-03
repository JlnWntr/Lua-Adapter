# Lua-Adapter
Use this lightweight wrapper- / adapter-class as an interface between Lua and C++.

Usage:
   
      int width {0};
      lua.get("width", width);	
» see **test.cpp** for more examples

Compile: 
      
      g++ -std=c++0x lua_adapter.cpp test.cpp  -llua -ldl -Wall



Requirements:
- liblua5.2-dev
- liblua5.2-0 (or later)



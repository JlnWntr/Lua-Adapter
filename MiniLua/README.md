# Mini Lua-Adapter
Use this minimal wrapper- / adapter-class as an interface between Lua and C++.

Usage:      

     MiniLua lua{"test.lua"}; 
     
     std::string title{"empty"};
     lua.Get("title", title);

Compilation & testing: 
      
      g++ MiniLua.cpp test.cpp -std=c++0x -llua5.3
  

Requirements:
- liblua5.3-dev
- liblua5.3-0



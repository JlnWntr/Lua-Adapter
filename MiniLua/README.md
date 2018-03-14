# Mini Lua-Adapter
Use this minimal wrapper- / adapter-class as an interface between Lua and C++.

Usage:

     MiniLua lua{"test.lua"};

     int width {0};
     lua.Get("width", width);

     std::string title{"empty"};
     if(lua.Get("title", title))
        std::cout << title;

Compile & test:

      g++ MiniLua.cpp test.cpp -std=c++0x -llua -ldl


Requirements:
      [Download and install Lua](https://www.lua.org/download.html).

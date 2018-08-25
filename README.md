# Lua-Adapter
Use this lightweight wrapper- / adapter-class as an interface between Lua and C++.

Usage:

      LuaAdapter lua{"test.lua"};

      int width {0};
      lua.Get("width", width);  // width=600
» see [test.cpp](https://github.com/JlnWntr/Lua-Adapter/blob/master/test.cpp) for more examples

Compile & test:

      g++-8 LuaAdapter.hpp LuaFunction.hpp LuaTable.hpp test.cpp -std=c++17 -llua -ldl -Wall -fconcepts


Requirements:

- [Download and install Lua](https://www.lua.org/download.html).
- g++-8 or higher


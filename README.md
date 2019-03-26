# Lua-Adapter
Use this C++-17 class to bind [Lua](https://www.lua.org/download.html) or even [MoonScript](https://github.com/JlnWntr/Lua-Adapter/tree/master/examples/moonscript) to C++.

## Usage
```c++
#include "LuaAdapter.hpp"

int main(){
  LuaAdapter lua{"test.lua"};

  int width {0};
  lua.Get("width", width);  // width=600

  return 0;
}
```
```bash
$ g++-8 the_program_above.cpp -std=c++17 -llua -ldl
```

» See more [examples](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples).

## Lua without Lua
Run Lua-code (embedded in your C++-code) on maschines where Lua is *not* even available!
[examples/static.cpp](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples/static.cpp) shows you how to compile any Lua-code into your C++-application togehter with the Lua-interpreter.


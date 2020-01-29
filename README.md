# Lua-Adapter
Use this C++-class to bind [Lua](https://www.lua.org/download.html) or even [MoonScript](https://github.com/JlnWntr/Lua-Adapter/tree/master/examples/moonscript) to C++.

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
Please see more [examples](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples).

## Build using g++
```bash
$ g++-8 the_program_above.cpp -std=c++17 -llua -ldl
```


## Build using CMake

```bash
$ cmake ./
```
It should be possible to build this project with Jetbrains CLion. If the very first attempt to build fails, try `Build > Rebuild 'test_lua'` from the menu.

## Support for older g++-versions
If the latest g++ features are not available on your system (for example on Windows), then try [Lua-Adapter/MiniLua](https://github.com/JlnWntr/Lua-Adapter/tree/master/MiniLua). This is a somewhat minimal and simple version of Lua-Adapter which doesn't make use of these new features (like [`if constexpr ..`](https://github.com/JlnWntr/Lua-Adapter/blob/master/LuaAdapter.hpp#L123))

# Lua-Adapter
Use this C++-class to bind [Lua 5.3+](https://www.lua.org/download.html) or even [MoonScript](https://github.com/JlnWntr/Lua-Adapter/tree/master/examples/moonscript) to C++.

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

#### Test

```bash
$ g++-8 the_program_above.cpp -std=c++17 -llua -ldl
```
Please see [more examples](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples).


### Build Lua 5.4 and test LuaAdapter using CMake
The [CMakeLists.txt](https://github.com/JlnWntr/Lua-Adapter/blob/master/CMakeLists.txt) in this directory downloads [Lua](https://www.lua.org), builds it and test-runs [test.cpp](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples/test.cpp).

```bash
$ cmake ./
$ make
```

It should be possible to build a (test-)project from here with *Jetbrains CLion*.
(If the very first attempt to build fails, try `Build > Rebuild 'test_lua'` from the menu.)

## Support for older g++-versions
If the latest *g++* features are not available on your system (for example on Windows), then try [Lua-Adapter/MiniLua](https://github.com/JlnWntr/Lua-Adapter/tree/master/MiniLua). This is a somewhat minimal and simple version of Lua-Adapter which does not make use of new features (like [`if constexpr`](https://github.com/JlnWntr/Lua-Adapter/blob/master/LuaAdapter.hpp#L150) etc.)

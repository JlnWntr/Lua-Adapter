# Lua-Adapter
Use this C++-17 class to bind Lua or even [MoonScript](https://github.com/JlnWntr/Lua-Adapter/tree/master/examples/moonscript) to C++.

## Usage
```c++
LuaAdapter lua{"test.lua"};
int width {0};
lua.Get("width", width);  // width=600
```
» See more [examples](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples).
      
## Compilation
Just 
 ```C
 #include "LuaAdapter.hpp"
```
and compile your program
```bash
$ g++-8 your_program.cpp -std=c++17 -llua -ldl
```

## Requirements

- [Download and install Lua](https://www.lua.org/download.html).
- g++-8 or higher

### Troubleshooting

If `apt-get install g++-8` does not work on your Debian, try `add-apt-repository ppa:ubuntu-toolchain-r/test && apt-get update`
first.

If g++-8 is not available on your system, you can [try an older version of Lua-Adapter](https://github.com/JlnWntr/Lua-Adapter/tree/all_in_one_old).

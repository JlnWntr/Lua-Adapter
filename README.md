# Lua-Adapter
Use this lightweight wrapper- / adapter-class as an interface between Lua and C++.

## Usage

    LuaAdapter lua{"test.lua"};
    int width {0};
    lua.Get("width", width);  // width=600

» See more [examples](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples).
      
## Compilation
In general:

    g++ LuaAdapter.hpp your_program.cpp -std=c++17 -llua -ldl

## Requirements

- [Download and install Lua](https://www.lua.org/download.html).
- g++-8 or higher

### Troubleshooting

If `apt-get install g++-8` does not work on your Debian, try `add-apt-repository ppa:ubuntu-toolchain-r/test && apt-get update`
first.

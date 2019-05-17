# Lua-Adapter library

## Usage

```C++
#ifndef LUA_ADAPTER_LIB_H
  #include "LuaAdapter.hpp"
#endif

int main() {
  Load("../examples/test.lua");

  int height{0};
  Get("height", height);
```
See [test.cpp](https://github.com/JlnWntr/Lua-Adapter/blob/master/lib/test.cpp) for more examples.

Compile the program with

```bash
g++ program.cpp LuaAdapter.o -llua -ldl
```

### Motivation

The current Lua-Adapter code is hard to compile on systems, where g++ with `-std=c++17` is *not* available.
This could be a mitigation of that problem. This needs further testing and definitely [more interfaces](https://github.com/JlnWntr/Lua-Adapter/blob/master/lib/LuaAdapter.hpp) though. Your contribution is more than welcome.

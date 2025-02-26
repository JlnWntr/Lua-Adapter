# Lua-Adapter
Use [Lua 5.3+](https://www.lua.org/download.html) in your C++ program.

## Usage
```c++
#include "LuaAdapter.hpp"

int main(){
  LuaAdapter lua{"test.lua"}; // load lua-file

  int width {0};
  lua.Get("width", width);  // width=600

  return 0;
}
```

### Build and test Lua-Adapter using `g++`

```bash
$ g++ the_program_above.cpp -std=c++17 -llua -ldl
```

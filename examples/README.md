# Examples
Have a look at these examples to learn how to use Lua-Adapter.


## General usage example

```C++
#include "../LuaAdapter.hpp"

int main(){
  LuaAdapter lua{"test.lua"};

  int width {0};
  lua.Get("width", width);  // width=600

  return 0;
}
```

```bash
 $ g++-8 test.cpp -std=c++17 -llua -ldl -Wall -DLUA_ADAPTER_DEBUG
```

See [test.cpp](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples/test.cpp) for *more* examples.

## Table example

```C++
#include "../LuaTable.hpp"

int main(){
  LuaAdapter lua{"test.lua"};
  LuaTable table{lua};

  if (table.Open("Table1")) {
    int ID {0};
    table.Get("ID", ID);
  }

  return 0;
}
```

```bash
 $ g++-8 tables.cpp -std=c++17 -llua -ldl -Wall -DLUA_ADAPTER_DEBUG
```
Please see [tables.cpp](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples/tables.cpp) for *more* examples.

## Function example

```C++
#include "../LuaFunction.hpp"

/* This C++-function can be called from Lua. (See test.lua) */
static int test_function(lua_State *L);

int main(){
  LuaAdapter lua{"test.lua"};
  LuaFunction function{lua};

  function.Call("Print", (std::string)"Hello!");

  return 0;
}
```

```bash
 $ g++-8 functions.cpp -std=c++17 -llua -ldl -fconcepts -Wall -DLUA_ADAPTER_DEBUG
```
See [functions.cpp](https://github.com/JlnWntr/Lua-Adapter/blob/master/examples/functions.cpp) for even *more* examples.

## Contact
If you need help or if you have questions, send me an e-mail to jlnwntr@gmail.com

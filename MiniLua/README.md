# Mini Lua-Adapter
Use this minimal wrapper- / adapter-class as an interface between Lua 5.3+ and C++:

```c++
MiniLua lua{"test.lua"};

int width {0};
lua.Get("width", width);

std::string title{"empty"};
if(lua.Get("title", title))
     std::cout << title;
```

## Compile and test

Linux or Unix:
```bash
$ g++ MiniLua.cpp test.cpp -llua -ldl
```

### Build Lua 5.4 and test MiniLua using CMake
The [CMakeLists.txt](https://github.com/JlnWntr/Lua-Adapter/blob/master/MiniLua/CMakeLists.txt) in this directory downloads [Lua](https://www.lua.org) and tries to build it.
It compiles a short [test](https://github.com/JlnWntr/Lua-Adapter/blob/master/MiniLua/test.cpp)-program, too.

```bash
$ cmake ./
$ make
```

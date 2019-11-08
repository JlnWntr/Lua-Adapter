# Mini Lua-Adapter
Use this minimal wrapper- / adapter-class as an interface between Lua and C++.

## Usage
```c++
MiniLua lua{"test.lua"};

int width {0};
lua.Get("width", width);

std::string title{"empty"};
if(lua.Get("title", title))
     std::cout << title;
```

## Compilation & test
```bash
g++ MiniLua.cpp test.cpp -llua -ldl
```

## Requirements
[Download and install Lua](https://www.lua.org/download.html).

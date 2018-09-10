# C++ ⇆ Moonscript

You can even run and use [Moonscript](http://moonscript.org/) on the fly in your C++ application using LuaAdapter.

### Example

    g++-8 ../../LuaFunction.hpp moon.cpp -std=c++17 -llua -ldl -Wall -fconcepts

## Prerequisits:

    git clone https://github.com/leafo/moonscript.git
    git clone https://github.com/pygy/LuLPeg.git

That's all. *No* (additional) installations are required.

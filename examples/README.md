Have a look at these examples to learn how to use Lua-Adapter.
(It should be easy.)

Compile and test:

 * General usage

        g++-8 ../LuaAdapter.hpp test.cpp -std=c++17 -llua -ldl -Wall

 * Tables

        g++-8 ../LuaTable.hpp tables.cpp -std=c++17 -llua -ldl -Wall

 * Functions

        g++-8 ../LuaFunction.hpp functions.cpp -std=c++17 -llua -ldl -Wall -fconcepts

If you need help or if you have questions, send me an e-mail to jlnwntr@gmail.com

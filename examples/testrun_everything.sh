#!/bin/sh
g++-8 ../LuaAdapter.hpp test.cpp -std=c++17 -llua -ldl -Wall
./a.out

g++-8 ../LuaTable.hpp tables.cpp -std=c++17 -llua -ldl -Wall
./a.out

g++-8 ../LuaFunction.hpp functions.cpp -std=c++17 -llua -ldl -Wall -fconcepts
./a.out

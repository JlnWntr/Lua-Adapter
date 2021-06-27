#!/bin/sh
g++ test.cpp -std=c++17 -llua -ldl -Wall -g -DLUA_ADAPTER_DEBUG
./a.out

g++ tables.cpp -std=c++17 -llua -ldl -Wall -g -DLUA_ADAPTER_DEBUG
./a.out

g++ functions.cpp -std=c++17 -llua -ldl -Wall -fconcepts -g -DLUA_ADAPTER_DEBUG
./a.out

#!/bin/sh
g++ test.cpp -std=c++17 -llua -ldl -Wall -g -fsanitize=address -DLUA_ADAPTER_DEBUG
./a.out

g++ tables.cpp -std=c++17 -llua -ldl -Wall -g -fsanitize=address
./a.out

g++ functions.cpp -std=c++17 -llua -ldl -Wall -fconcepts -g -fsanitize=address
./a.out

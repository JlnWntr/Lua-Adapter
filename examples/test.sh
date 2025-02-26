#!/bin/sh
g++ test.cpp -o test -std=c++17 -llua -ldl -Wall -g -DLUA_ADAPTER_DEBUG
./test


g++ functions.cpp -o functions -std=c++17 -llua -ldl -Wall -g -DLUA_ADAPTER_DEBUG
./functions

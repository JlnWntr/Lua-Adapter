#!/bin/sh
g++-8 test.cpp -std=c++17 -llua -ldl -Wall
./a.out

g++-8 tables.cpp -std=c++17 -llua -ldl -Wall
./a.out

g++-8 functions.cpp -std=c++17 -llua -ldl -Wall -fconcepts
./a.out

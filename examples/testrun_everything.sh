#!/bin/sh
g++-8 test.cpp -std=c++17 -llua -ldl -Wall -g -fsanitize=address
./a.out

g++-8 tables.cpp -std=c++17 -llua -ldl -Wall -g -fsanitize=address
./a.out

g++-8 functions.cpp -std=c++17 -llua -ldl -Wall -fconcepts -g -fsanitize=address
./a.out

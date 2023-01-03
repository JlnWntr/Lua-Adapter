/*
 * Copyright (c) 2015-2023 JlnWntr (jlnwntr@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* #define LUA_ADAPTER_DEBUG */

#ifndef LUA_TABLE_H
#include "../LuaTable.hpp"
#endif
#ifndef LUA_ADAPTER_DEBUG
#include <iostream>
#endif

int main() {
    LuaAdapter lua{"test.lua"};
    LuaTable luaTable{lua};

    if (luaTable.Open("Table1")) {
        int ID{0};
        luaTable.Get("ID", ID);
        std::cout << "ID: " << ID << std::endl;

        int Value{0};
        luaTable.Get("Value", Value);
        std::cout << "Value: " << Value << std::endl;

        std::string Text{};
        luaTable.Get("Text", Text);
        std::cout << "Text: " << Text << std::endl;

        luaTable.Close(); // close "Table1"
    }
    std::cout << std::endl;

    if (luaTable.Open("matrix")) {

        // Get the length of the table
        const unsigned short int length{(unsigned short int)luaTable.Length()};
        std::cout << "Table-length: " << length << std::endl;

        for (unsigned short int j = 1; j <= length; j++) {
            for (unsigned short int  i = 1; i <= length; i++) {
                int temp {0};
                luaTable.Get({j, i}, temp); // matrix[j][i]
                std::cout << temp << " ";
            }
            std::cout << std::endl;
        }
        luaTable.Close(); // close table "matrix"
    }
    std::cout << std::endl;

    if (luaTable.Open("Table2")) {
        int X{0};
        luaTable.Get("X", X);
        std::cout << "X: " << X << std::endl;

        int Y{0};
        luaTable.Get("Y", Y);
        std::cout << "Y: " << Y << std::endl;

        if (luaTable.Open("Test")) { // open 'nested' table "Test"
            int A {0};
            luaTable.Get("A", A);
            std::cout << "\t A: " << A << std::endl;
            int B {0};
            luaTable.Get("B", B);
            std::cout << "\t B: " << B << std::endl;

            luaTable.Close(); // close nested table "Test"
        }
        luaTable.Close(); // close "Table2"
    }

    if (luaTable.Open("Strings")) {
        std::string k, v;

        // Get string-field by index
        luaTable.Get(1, k);
        luaTable.Get(2, v);

        std::cout << "Key: " << k << std::endl;
        std::cout << "Value: " << v << std::endl;

        luaTable.Close(); // close "Strings"
    }
    // Check lua's internal stack
    std::cout << "stack top: " << lua.GetTop() << std::endl; // should be 0
    return 0;
}

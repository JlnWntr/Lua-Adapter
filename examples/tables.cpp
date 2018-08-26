/*
 * Copyright (c) 2015-2018 JlnWntr (jlnwntr@gmail.com)
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
#ifndef LUA_TABLE_H
  #include "../LuaTable.hpp"
#endif

int main() {
  LuaAdapter lua{"test.lua"};
  LuaTable luaTable{lua};

  if (luaTable.Open("Table1")) {
    int ID{0};
    luaTable.Get("ID", ID);
    std::cout << "ID: " << ID << "\n";

    int Value{0};
    luaTable.Get("Value", Value);
    std::cout << "Value: " << Value << "\n";

    std::string Text{};
    luaTable.Get("Text", Text);
    std::cout << "Text: " << Text << "\n";

    luaTable.Close(); // close "Table1"
  }
  std::cout << "\n";

  if (luaTable.Open("matrix")) {

    // Get the length of the table
    const unsigned short int length{luaTable.Length()};
    std::cout << "Table-length: " << length << "\n";

    for (unsigned char j = 1; j <= length; j++) {
      for (unsigned char i = 1; i <= length; i++) {
        int temp = 0;
        luaTable.Get((const std::vector<unsigned short int>){j, i},
                     temp); // matrix[j][i]
        std::cout << temp << " ";
      }
      std::cout << "\n";
    }
    luaTable.Close(); // close table "matrix"
  }
  std::cout << "\n";

  if (luaTable.Open("Table2")) {
    int X{0};
    luaTable.Get("X", X);
    std::cout << "X: " << X << "\n";

    int Y{0};
    luaTable.Get("Y", Y);
    std::cout << "Y: " << Y << "\n";

    if (luaTable.Open("Test")) { // open 'nested' table "Test"
      int A = 0;
      luaTable.Get("A", A);
      std::cout << "\t A: " << A << "\n";
      int B = 0;
      luaTable.Get("B", B);
      std::cout << "\t B: " << B << "\n";

      luaTable.Close(); // close nested table "Test"
    }
    luaTable.Close(); // close "Table2"
  }

  if (luaTable.Open("Strings")) {
    std::string k, v;

    // Get string-field by index
    luaTable.Get(1, k);
    luaTable.Get(2, v);

    std::cout << "Key: " << k << "\n";
    std::cout << "Value: " << v << "\n";

    luaTable.Close(); // close "Strings"
  }
  return 0;
}
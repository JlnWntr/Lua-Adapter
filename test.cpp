/*
 * Copyright (c) 2015-2017 JlnWntr (jlnwntr@gmail.com)
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

#ifndef LUA_H
#include "lua_adapter.hpp"
#endif

static int testCFunction(lua_State *L);

int main(int argc, char *argv[]) {

  LuaAdapter lua("test.lua");

  // simply parameterize your application!
  int height{0};
  lua.get("height", height);
  std::cout << "height: " << height << "\n";

  // let's close ..
  lua.close();
  // and (re-)initialize for further tests..
  lua.init();

  // Hint: You CAN change a (default) value of a global lua variable BEFORE
  // loading the actual lua-file
  if (lua.set("GlobalVar", 321) == false) {
    std::cout << "Could not set 'GlobalVar'!\n";
  }

  // and THEN load the script:
  lua.load("test.lua");

  // get an int
  int width{0};
  lua.get("width", width);
  std::cout << "width: " << width << "\n";

  // get double
  double number{0};
  lua.get("number", number);
  std::cout << "Number: " << number << "\n";

  // get string
  std::string title{"empty"};
  lua.get("title", title);
  std::cout << "title: " << title << "\n";

  // get boolean
  bool boolean{false};
  lua.get("fullscreen", boolean);
  std::cout << "fullscreen: " << (boolean ? "true" : "false") << "\n";

  std::cout << "\n";

  // tables
  if (lua.openTable("Table1")) {
    int ID{0};
    lua.getField("ID", ID);
    std::cout << "ID: " << ID << "\n";

    int Value{0};
    lua.getField("Value", Value);
    std::cout << "Value: " << Value << "\n";

    std::string Text{};
    lua.getField("Text", Text);
    std::cout << "Text: " << Text << "\n";

    lua.closeTable(); // close "Table1"
  }

  std::cout << "\n";

  if (lua.openTable("matrix")) {

    // Get the length of the table
    const unsigned short int length{lua.getTableLength()};
    std::cout << "Table-length: " << length << "\n";

    for (unsigned char j = 1; j <= length; j++) {
      for (unsigned char i = 1; i <= length; i++) {
        int temp = 0;
        lua.getNestedField(j, i, temp); // matrix[j][i]
        std::cout << temp << " ";
      }
      std::cout << "\n";
    }
    lua.closeTable(); // close table "matrix"
  }
  std::cout << "\n";

  // more table-tests
  if (lua.openTable("Table2")) {
    int X{0};
    lua.getField("X", X);
    std::cout << "X: " << X << "\n";

    int Y{0};
    lua.getField("Y", Y);
    std::cout << "Y: " << Y << "\n";

    if (lua.openNestedTable("Test")) {
      int A = 0;
      lua.getField("A", A);
      std::cout << "\t A: " << A << "\n";
      int B = 0;
      lua.getField("B", B);
      std::cout << "\t B: " << B << "\n";

      lua.closeTable(); // close nested table "Test"
    }
    lua.closeTable(); // close "Table2"
  }

  // Check lua's internal stack
  std::cout << "\n";
  std::cout << "Lua stack top: " << lua.getTop() << "\n"; // should be 0

  // calling a function from test.lua to compute stuff
  int test[] = {36, 24};
  int result{0};
  lua.callFunction("gcd", 2, test, result);
  std::cout << "gcd: " << result << "\n";
  std::cout << "\n";

  // 'Declare' a C/C++-function for lua
  lua.pushFunction(testCFunction, "testFunction");

  // and call it!
  double result2{0};
  lua.callFunction("testFunction", result2);
  std::cout << "testing C-Function: " << result2 << "\n";

  std::cout << "Lua stack top: " << lua.getTop() << "\n"; // 0
  return 0;
}

// This function can be called from Lua
static int testCFunction(lua_State *L) {
  LuaAdapter lua{L};
  double number{0};
  lua.get("number", number);

  std::cout << "Number: " << number << "\n";
  std::cout << "Lua stack top: " << lua.getTop() << "\n"; // 0

  number *= 2;
  lua.push(number);
  return 1;
}

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

#ifndef LUA_ADAPTER_H
#include "LuaAdapter.hpp"
#endif
#ifndef LUA_FUNCTION_H
#include "LuaFunction.hpp"
#endif
#ifndef LUA_TABLE_H
#include "LuaTable.hpp"
#endif

static int test_function(lua_State *L);

int main(int argc, char *argv[]) {

  LuaAdapter lua{"test.lua"};
  // lua.Debug();

  /**
   * Parameterize your application.
   */
  // get height
  int height{0};
  lua.Get("height", height);
  std::cout << "height: " << height << "\n\n";

  // get an int
  int width{0};
  lua.Get("width", width);
  std::cout << "width: " << width << "\n";

  // get double
  double number{0};
  lua.Get("number", number);
  std::cout << "Number: " << number << "\n";

  // get string
  std::string title{"empty"};
  lua.Get("title", title);
  std::cout << "title: " << title << "\n";

  // get boolean
  bool boolean{false};
  lua.Get("fullscreen", boolean);
  std::cout << "fullscreen: " << (boolean ? "true" : "false") << "\n";

  std::cout << "\n";

  /** - TABLE-TESTS ----------------------------------------------------*/
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
        luaTable.Get(j, i, temp); // matrix[j][i]
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

  /* ------------------ ------------------------------------------------*/
  // Check lua's internal stack
  std::cout << "\n";
  std::cout << "Lua stack top: " << lua.GetTop() << "\n"; // should be 0

  // let's close ..
  lua.Close();
  // and (re-)initialize for further tests..
  lua.Init();

  /** - FUNCTION-TESTS -------------------------------------------------*/

  LuaFunction luaFunction{lua};
  /**
   * Define a C/C++-function that can be called from lua (see test.lua)
   **/
  luaFunction.Push(test_function, "test_function");
  /**
   * and THEN load the script:
   */
  lua.Load("test.lua");

  /**
   * calling a lua-function
   **/
  luaFunction.Call("test");

  /**
   * calling another lua-function (see test.lua)
   */
  int test[] = {36, 24};
  int result{0};
  luaFunction.Call("gcd", 2, test, result);
  std::cout << "gcd: " << result << "\n";
  std::cout << "\n";

  test_function(nullptr); // this is just to ignore a compile-warning

  return 0;
}

/**
 * This C++-function can be called from Lua
 */
static int test_function(lua_State *L) {
  if (!L)
    return 0;
  LuaAdapter lua{L};
  double number{lua_tonumber(L, 1)}; /* get argument */
  number *= 2;
  lua.Push(number);
  return 1;
}
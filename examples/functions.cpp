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
#ifndef LUA_FUNCTION_H
  #include "../LuaFunction.hpp"
#endif

static int test_function(lua_State *L);

int main() {
  LuaAdapter lua{};
  LuaFunction luaFunction{lua};

  // Define a C/C++-function that can be called from lua (see test.lua)
  luaFunction.Push(test_function, "test_function");

  // THEN load the script:
  lua.Load("test.lua");

  // null arguments
  int Return_int{0};
  luaFunction.Call("Random", 0, Return_int);
  std::cout << "Random: " << Return_int << "\n";

  // one argument, no return value
  luaFunction.Call("Print", (const std::string) "This is a string.");
  luaFunction.Call("Print", 1);
  luaFunction.Call("Print", 2.2);
  luaFunction.Call("Print", true);

  // one argument, one return value
  const double double_arg {2.3};
  double double_result{};
  const std::string function{"Inc"};
  luaFunction.Call(function, double_arg, double_result);
  std::cout << "Incrementing a double: " << double_result << "\n";


  std::string Return_string{};
  luaFunction.Call("String", 0, Return_string);
  std::cout << "String: " << Return_string << "\n";

  // n=3 arguments
  int array[] = {1, 2, 3};
  luaFunction.Call("Return", 3, array, Return_int);
  std::cout << "Return(1, 2, 3) = " << Return_int << "\n";

  // n=2 arguments
  int test[] = {36, 24};
  int result{0};
  luaFunction.Call("gcd", 2, test, result);
  std::cout << "gcd: " << result << "\n";
  std::cout << "\n";

  test_function(nullptr); // this is to ignore a compile-warning
  std::cout << "Test ended!\n";
  return 0;
}

/**
 * This C++-function can be called from Lua
 */
static int test_function(lua_State *L) {
  if (!L)
    return 0;
  LuaAdapter lua{L};
  double number{lua_tonumber(L, 1)};
  number *= 2;
  lua.Push(number);
  return 1;
}
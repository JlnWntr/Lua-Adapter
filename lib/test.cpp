/*
 * Copyright (c) 2015-2019 JlnWntr (jlnwntr@gmail.com)
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

#ifndef LUA_ADAPTER_LIB_H
  #include "LuaAdapter.hpp"
#endif
static int test_function(lua_State *L);


int main() {
  // Define a C++-function that can be called from lua (see test.lua)
  if(not Push(test_function, "test_function")){
    std::cout << "Could NOT push C-function! \n";
    return 1;
  }

  if (not Load("../examples/test.lua")) {
    std::cout << "Could NOT load lua-file! \n";
    return 1;
  }

  // get height
  int height{0};
  Get("height", height);
  std::cout << "Test 1: height = " << height << std::endl;

  // get an int
  int width{0};
  Get("width", width);
  std::cout << "Test 2: width = " << width << std::endl;

  // get double
  double number{0};
  Get("number", number);
  std::cout << "Test 3: Number = " << number << std::endl;

  // get string
  std::string title{"empty"};
  Get("title", title);
  std::cout << "Test 4: title = " << title << std::endl;


/** FUNCTIONS **/

// null arguments
  int Return_int{0};
  Call("Random", Return_int);
  std::cout << "Test 5: Random = " << Return_int << std::endl;

  // one argument, no return value
  std::cout << "Test 6:" << std::endl;
  Call("Print", (const std::string) "This is a string.");
  Call("Print", 1);
  Call("Print", 2.2);
  Call("Print", true);

  // one argument, one return value
  const int arg {2};
  int result{};
  Call("Inc", arg, result);
  std::cout << "Test 7: Incrementing 2 = " << result << std::endl;

  // zero arguments, one return value
  Call("Random", 0, result);
  std::cout << "Test 8: Random-function return = " << result << std::endl;

  // two arguments, one return value
  int test2[] = {36, 24};
  result = 0;
  Call("gcd", 2, test2, result);
  std::cout << "Test 9: gcd(36, 24) = " << result << std::endl;

  // three arguments, one return value
  int array[] = {1, 2, 3};
  Call("Sum3", 3, array, Return_int);
  std::cout << "Test 10: Sum(1, 2, 3) = " << Return_int << std::endl;

  test_function(nullptr); // this is to ignore a compile-warning

  return 0;
}

/**
 * This C++-function can be called from Lua
 */
static int test_function(lua_State *L) {
  if (not L) return 0;
  double number{lua_tonumber(L, 1)};
  number *= 2;
  lua_pushnumber(L, number);
  return 1;
}
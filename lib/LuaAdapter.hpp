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
#define LUA_ADAPTER_LIB_H

#include <iostream>
#include <string>
#include <lua.hpp>

  typedef int (*Lua_callback_function)(lua_State *L);

  bool Load(std::string file);

  bool Get(const char *name, int &r) ;
  bool Get(const char *name, double &r) ;
  bool Get(const char *name, std::string &r) ;

  bool Set(const char *name, int &a) ;
  bool Set(const char *name, double &a);
  bool Set(const char *name, std::string &a);

  bool Push(Lua_callback_function f, const char *name);
  bool Push(Lua_callback_function f, const std::string name);

  bool Call(const char *f, const int c, const int *const a, int &r) ;
  bool Call(const std::string f, const int c, const int *const a, int &r);
  bool Call(const char *f, const int c, const double *const a, double &r) ;
  bool Call(const char *f, const int a, int &r) ;
  bool Call(const std::string f, const int a, int &r);
  bool Call(const char *f, const double a, double &r) ;
  bool Call(const std::string f, const int c, const double *const a, double &r);
  bool Call(const char *f, const int c, const std::string *const a, std::string &r);
  bool Call(const std::string f, const int c, const std::string *const a, std::string &r);
  bool Call(const char *f, const int c, const int *const a);
  bool Call(const std::string f, const int c, const int *const a);
  bool Call(const char *f, const int a);
  bool Call(const std::string f, const int a);
  bool Call(const char *f, const int c, const std::string *const a);
  bool Call(const std::string f, const int c, const std::string *const a);
  bool Call(const char *f, const std::string a) ;
  bool Call(const std::string f, const std::string a);
  bool Call(const char *f, const int c, const double *const a);
  bool Call(const std::string f, const int c, const double *const a) ;
  bool Call(const char *f, const double a);

#endif
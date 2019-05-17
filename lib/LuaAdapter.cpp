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

#ifndef LUA_FUNCTION_H
  #include "../LuaFunction.hpp"
#endif
#include <iostream>

  LuaAdapter lua;
  LuaFunction function{lua};

  bool Load(std::string file){return lua.Load(file);}

  bool Get(const char *name, int &r) {return lua.Get(name, r);}
  bool Get(const char *name, double &r) {return lua.Get(name, r);}
  bool Get(const char *name, std::string &r) {return lua.Get(name, r);}

  bool Set(const char *name, int &a) {return lua.Set(name, a);}
  bool Set(const char *name, double &a) {return lua.Set(name, a);}
  bool Set(const char *name, std::string &a) {return lua.Set(name, a);}

  bool Push(Lua_callback_function f, const char *name) {return function.Push(f, name);}
  bool Push(Lua_callback_function f, const std::string name) {return function.Push(f, name.c_str());}

  bool Call(const char *f, const int c, const int *const a, int &r) {return function.Call(f,c,a,r);}
  bool Call(const std::string f, const int c, const int *const a, int &r) {return function.Call(f.c_str(), c, a, r); }
  bool Call(const char *f, const int c, const double *const a, double &r) {return function.Call(f,c,a,r);}
  bool Call(const std::string f, const int c, const double *const a, double &r) {return function.Call(f.c_str(), c, a, r); }
  bool Call(const char *f, const int c, const std::string *const a, std::string &r) {return function.Call(f,c,a,r);}
  bool Call(const std::string f, const int c, const std::string *const a, std::string &r) {return function.Call(f.c_str(), c, a, r); }
  bool Call(const char *f, const int a, int &r) {return function.Call(f,1, (const int *const)&a,r);}
  bool Call(const std::string f, const int c, const int a, int &r) {return function.Call(f.c_str(), 1, (const int *const)&a, r); }
  bool Call(const char *f, const double a, double &r) {return function.Call(f,1,(const double *const)&a,r);}
  bool Call(const std::string f, const double a, double &r) {return function.Call(f.c_str(), 1, (const double *const)&a, r); }
  bool Call(const char *f, const std::string a, std::string &r) {return function.Call(f,1,(const std::string *const)&a,r);}
  bool Call(const std::string f, const std::string a, std::string &r) {return function.Call(f.c_str(), 1, (const std::string *const)&a, r); }
  bool Call(const char *f, const int c, const int *const a) {return function.Call(f,c,a);}
  bool Call(const std::string f, const int c, const int *const a) {return function.Call(f.c_str(), c, a);}
  bool Call(const char *f, const int a) {return function.Call(f, 1, (const int *)&a);}
  bool Call(const std::string f, const int a) {return function.Call(f.c_str(), a);}
  bool Call(const char *f, const int c, const std::string *const a) {return function.Call(f,c,a);}
  bool Call(const std::string f, const int c, const std::string *const a) {return function.Call(f.c_str(), c, a);}
  bool Call(const char *f, const std::string a) {return function.Call(f, 1, (const std::string *)&a);}
  bool Call(const std::string f, const std::string a) {return function.Call(f.c_str(), a);}
  bool Call(const char *f, const int c, const double *const a) {return function.Call(f,c,a);}
  bool Call(const std::string f, const int c, const double *const a) {return function.Call(f.c_str(), c, a);}
  bool Call(const char *f, const double a) {return function.Call(f, 1, (const double *)&a);}
  bool Call(const std::string f, const double a) {return function.Call(f.c_str(), a);}
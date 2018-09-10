--[[--
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
--]]--

function moonscript(filename)

    local lpeg = require("LuLPeg.lulpeg"):register(not _ENV and _G)

    package.path = './moonscript/?.lua;' .. package.path
    local moon = require "moonscript.base"

    -- Load moon script
    local file, err = io.open(filename)
    if not file then return nil end
    local code = assert(file:read("*a"))
    file:close()

    -- Compile to lua
    local lua_code = moon.to_lua(code)

    -- Run lua code
    local run = load(lua_code); assert(run); run()
end
-- Test:
-- moonscript("hello.moon")
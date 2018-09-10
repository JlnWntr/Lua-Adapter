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
/** This is how you can compile lua-bytecode into your c(++)-program.
**/
#include "../LuaAdapter.hpp" 

/**
 * $ luac -s test.lua
 * $ xxd -i luac.out > test.h
 * $ g++ static.cpp /path/to/lua-5.3.5/src/liblua.a -Wall -ldl -std=c++17
 */
#include "test.h" // « generated via xxd (s. above)

int main() {
    LuaAdapter lua{};
    lua.Load((const char *)luac_out, luac_out_len);

    f.Call("Print", "hello");

    return 0;
}

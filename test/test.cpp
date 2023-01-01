#include <lua.hpp>

#include <cassert>

int main(int, char**)
{
    lua_State* L = luaL_newstate();
    assert(luaL_dostring(L, "require('hyjal');"));
    lua_close(L);
    return 0;
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "hyjal.hpp"

#include <fmt/core.h>
#include <lua.hpp>

namespace meorawr::hyjal {
    void hello()
    {
        lua_State* L = luaL_newstate();
        luaL_openlibsx(L, LUALIB_ELUNE);
        lua_getfield(L, LUA_GLOBALSINDEX, "print");
        lua_pushstring(L, fmt::format("{}", "Hello! :)").c_str());
        lua_getfield(L, LUA_GLOBALSINDEX, "issecure");
        lua_call(L, 0, 1);
        lua_call(L, 2, 0);
        lua_close(L);
    }
}

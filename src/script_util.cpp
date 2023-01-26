// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "script_util.hpp"

#include "object.hpp"

#include <lua.hpp>

#include <string_view>
#include <utility>

namespace meorawr::hyjal {
    std::string_view lua_checkstringview(lua_State* L, int idx)
    {
        std::size_t len;
        const char* str = lua_tolstring(L, idx, &len);

        if (!str) {
            luaL_typerror(L, idx, "string");
            std::unreachable();
        }

        return std::string_view(str, len);
    }

    std::string_view lua_optstringview(lua_State* L, int idx, std::string_view opt)
    {
        if (lua_isnoneornil(L, idx)) {
            return opt;
        }

        return lua_checkstringview(L, idx);
    }

    std::string_view lua_tostringview(lua_State* L, int idx)
    {
        std::size_t len;
        const char* str = lua_tolstring(L, idx, &len);
        return std::string_view(str, len);
    }

    void lua_pushstringview(lua_State* L, std::string_view str)
    {
        lua_pushlstring(L, str.data(), str.size());
    }

    bool lua_isscriptuserdata(lua_State* L, int idx)
    {
        // TODO: Is it worth figuring out a way to strengthen this check to
        //       prevent us from crashing on maliciously crafted tables?

        return lua_islightuserdata(L, idx);
    }

    bool lua_isscriptobject(lua_State* L, int idx)
    {
        bool result = false;

        if (lua_istable(L, idx)) {
            lua_rawgeti(L, idx, 0);
            result = lua_isscriptuserdata(L, -1);
            lua_pop(L, -1);
        }

        return result;
    }

    void lua_pushscriptobject(lua_State* L, object& object)
    {
        // TODO: Persistent table refs!
        lua_createtable(L, 0, 1);
        lua_pushlightuserdata(L, std::addressof(object));
        lua_rawseti(L, -2, 0);
    }
}

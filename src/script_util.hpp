// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "object.hpp"

#include <lua.hpp>

#include <concepts>
#include <string_view>

namespace meorawr::hyjal {
    std::string_view lua_checkstringview(lua_State* L, int idx);
    std::string_view lua_optstringview(lua_State* L, int idx, std::string_view opt = {});
    std::string_view lua_tostringview(lua_State* L, int idx);
    void lua_pushstringview(lua_State* L, std::string_view str);

    bool lua_isscriptuserdata(lua_State* L, int idx);
    bool lua_isscriptobject(lua_State* L, int idx);
    void lua_pushscriptobject(lua_State* L, object& object);

    template<std::derived_from<object> T = object>
    T* lua_checkscriptobject(lua_State* L, int idx)
    {
        if (!lua_istable(L, idx)) {
            luaL_error(L, "Attempt to find 'this' in non-table object (used '.' instead of ':' ?)");
        }

        lua_rawgeti(L, idx, 0);

        if (!lua_isscriptuserdata(L, -1)) {
            luaL_error(L, "Attempt to find 'this' in non-framescript object");
        }

        T* obj = object_cast<T>(static_cast<object*>(lua_touserdata(L, -1)));
        lua_pop(L, 1);

        if (!obj) {
            luaL_error(L, "Wrong object type for function");
        }

        return obj;
    }

    template<std::derived_from<object> T = object>
    T* lua_optscriptobject(lua_State* L, int idx, T* opt = {})
    {
        if (lua_isnoneornil(L, idx)) {
            return opt;
        }

        return lua_checkscriptobject<T>(L, idx);
    }

    template<std::derived_from<object> T = object>
    T* lua_toscriptobject(lua_State* L, int idx)
    {
        T* obj = nullptr;

        if (lua_istable(L, idx)) {
            lua_rawgeti(L, idx, 0);

            if (lua_isscriptuserdata(L, -1)) {
                obj = object_cast<T>(static_cast<object*>(lua_touserdata(L, -1)));
            }

            lua_pop(L, 1);
        }

        return obj;
    }
}

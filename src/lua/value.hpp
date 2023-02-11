// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "type.hpp"

#include <lua.hpp>

#include <concepts>
#include <string_view>

namespace meorawr::hyjal::lua {
    // clang-format off

    template<typename T>
    concept pushable_to = requires(T v, lua_State* state) {
        { v.push_to(state) } -> std::same_as<void>;
    };

    template<typename T>
    concept pushable_value = requires(T v, lua_State* state) {
        { push_value(state, v) } -> std::same_as<void>;
    };

    // clang-format on

    inline void push_value(lua_State* state, std::nullptr_t) = delete;

    inline void push_value(lua_State* state, nil_t)
    {
        lua_pushnil(state);
    }

    inline void push_value(lua_State* state, bool b)
    {
        lua_pushboolean(state, b);
    }

    inline void push_value(lua_State* state, std::integral auto n)
    {
        lua_pushinteger(state, n);
    }

    inline void push_value(lua_State* state, std::floating_point auto n)
    {
        lua_pushnumber(state, n);
    }

    inline void push_value(lua_State* state, const char* s)
    {
        lua_pushstring(state, s);
    }

    inline void push_value(lua_State* state, std::string_view s)
    {
        lua_pushlstring(state, s.data(), s.size());
    }

    inline void push_value(lua_State* state, pushable_to auto&& v)
    {
        v.push_to(state);
    }
}

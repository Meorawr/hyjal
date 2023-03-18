// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "value_type.hpp"

#include <lua.hpp>

#include <concepts>
#include <string_view>

namespace meorawr::hyjal::detail::push_back {
    template<typename T>
    concept pushable_to = requires(T v, lua_State* state) { { v.push_to(state) } -> std::same_as<void>; };

    void push_back(lua_State* state, lua::nil_t);
    void push_back(lua_State* state, std::same_as<bool> auto b);
    void push_back(lua_State* state, std::integral auto n);
    void push_back(lua_State* state, std::floating_point auto n);
    void push_back(lua_State* state, const char* s);
    void push_back(lua_State* state, std::string_view s);
    void push_back(lua_State* state, std::nullptr_t) = delete;
    void push_back(lua_State* state, pushable_to auto&& v);

    template<typename T>
    concept has_adl = requires(lua_State* state, T v) { { push_back(state, v) } -> std::same_as<void>; };

    struct cpo_t {
        void operator()(lua_State* state, has_adl auto&& v) const { push_back(state, std::forward<decltype(v)>(v)); }
    };
}

namespace meorawr::hyjal::lua::inline cpo {
    inline constexpr auto push_back = detail::push_back::cpo_t{};

    template<typename T>
    concept pushable_value = requires(T v, lua_State* state) { push_back(state, v); };
}

namespace meorawr::hyjal::detail::push_back {
    inline void push_back(lua_State* state, lua::nil_t)
    {
        lua_pushnil(state);
    }

    inline void push_back(lua_State* state, std::same_as<bool> auto b)
    {
        lua_pushboolean(state, b);
    }

    inline void push_back(lua_State* state, std::integral auto n)
    {
        lua_pushinteger(state, n);
    }

    inline void push_back(lua_State* state, std::floating_point auto n)
    {
        lua_pushnumber(state, n);
    }

    inline void push_back(lua_State* state, const char* s)
    {
        lua_pushstring(state, s);
    }

    inline void push_back(lua_State* state, std::string_view s)
    {
        lua_pushlstring(state, s.data(), s.size());
    }

    inline void push_back(lua_State* state, pushable_to auto&& v)
    {
        v.push_to(state);
    }
}

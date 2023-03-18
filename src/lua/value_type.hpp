// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <lua.hpp>

namespace meorawr::hyjal::lua {
    enum class value_type {
        none = LUA_TNONE,
        nil = LUA_TNIL,
        boolean = LUA_TBOOLEAN,
        lightuserdata = LUA_TLIGHTUSERDATA,
        number = LUA_TNUMBER,
        string = LUA_TSTRING,
        table = LUA_TTABLE,
        function = LUA_TFUNCTION,
        userdata = LUA_TUSERDATA,
        thread = LUA_TTHREAD,
    };

    struct nil_t {
        constexpr explicit operator bool() noexcept { return false; }
    };

    inline constexpr nil_t nil;

    using number_t = lua_Number;
    using integer_t = lua_Integer;
}

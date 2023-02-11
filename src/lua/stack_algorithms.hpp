// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"

#include <lua.hpp>

namespace meorawr::hyjal::lua::stack_algorithms {
    inline bool empty(lua_State* state) noexcept
    {
        return lua_gettop(state) == 0;
    }

    inline index_size_t size(lua_State* state) noexcept
    {
        return lua_gettop(state);
    }

    inline index_t absolute_index(lua_State* state, index_t index) noexcept
    {
        if (index > 0) {
            return index;
        } else {
            return lua_absindex(state, index);
        }
    }

    inline index_t relative_index(lua_State* state, index_t index) noexcept
    {
        if (index < 0) {
            return index;
        } else {
            return -(lua_gettop(state) - index + 1);
        }
    }
}

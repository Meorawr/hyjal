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

    inline void copy(lua_State* from, index_t from_index, lua_State* to, index_t to_index)
    {
        if (from == to) [[likely]] {
            lua_copy(to, from_index, to_index);
        } else {
            // Assumes xmovable; should assert this?
            lua_pushvalue(from, from_index);
            lua_xmove(from, to, 1);
            lua_replace(to, to_index);
        }
    }
}

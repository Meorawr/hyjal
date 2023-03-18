// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"
#include "stack_fwd.hpp"

#include <lua.hpp>

namespace meorawr::hyjal::lua {
    class value_reference {
    public:
        value_reference(lua_State* state, accessible_index index) noexcept;
        value_reference(value_reference&) = delete;
        value_reference(value_reference&&) = delete;

        value_reference& operator=(value_reference& other);
        value_reference& operator=(value_reference&&) = delete;

        lua_State* state() const noexcept;
        index_t index() const noexcept;
        value_type type() const noexcept;

    private:
        lua_State* state_;
        accessible_index index_;
    };

    inline value_reference::value_reference(lua_State* state, accessible_index index) noexcept
        : state_(state)
        , index_(index)
    {
    }

    inline value_reference& value_reference::operator=(value_reference& other)
    {
        stack_algorithms::copy(other.state_, other.index_, index_);
        return *this;
    }

    inline lua_State* value_reference::state() const noexcept
    {
        return state_;
    }

    inline index_t value_reference::index() const noexcept
    {
        return index_;
    }

    inline value_type value_reference::type() const noexcept
    {
        return static_cast<value_type>(lua_type(state_, index_));
    }
}

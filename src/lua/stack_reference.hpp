// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"

#include <lua.hpp>

namespace meorawr::hyjal::lua {
    class stack_reference {
    public:
        constexpr stack_reference(lua_State* state, accessible_index index) noexcept;
        stack_reference(stack_reference&) = delete;
        stack_reference(stack_reference&&) = delete;

        stack_reference& operator=(stack_reference& other);
        stack_reference& operator=(stack_reference&&) = delete;

        constexpr lua_State* state() const noexcept;
        constexpr index_t index() const noexcept;

    private:
        lua_State* state_;
        accessible_index index_;
    };

    constexpr stack_reference::stack_reference(lua_State* state, accessible_index index) noexcept
        : state_(state)
        , index_(index)
    {
    }

    inline stack_reference& stack_reference::operator=(stack_reference& other)
    {
        stack_algorithms::copy(other.state_, other.index_, state_, index_);
        return *this;
    }

    constexpr lua_State* stack_reference::state() const noexcept
    {
        return state_;
    }

    constexpr index_t stack_reference::index() const noexcept
    {
        return index_;
    }
}

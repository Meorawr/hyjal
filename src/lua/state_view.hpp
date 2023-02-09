// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "stack_index.hpp"
#include "stack_reference.hpp"
#include "state.hpp"

#include <lua.hpp>

#include <utility>

namespace meorawr::hyjal::lua {
    class state_view {
    public:
        constexpr state_view(state_t state) noexcept;
        constexpr state_view(std::nullptr_t) noexcept = delete;

        constexpr operator state_t() const noexcept;

        // Stack element accessors

        constexpr stack_reference operator[](stack_index index) const noexcept;
        constexpr stack_reference at(stack_index index) const noexcept;
        constexpr stack_reference top() const noexcept;
        constexpr stack_reference base() const noexcept;

        // Non-member functions

        friend constexpr void swap(state_view& a, state_view& b) noexcept;

    private:
        state_t state_ = {};
    };

    constexpr state_view::state_view(state_t state) noexcept
        : state_(state)
    {
    }

    constexpr state_view::operator state_t() const noexcept
    {
        return state_;
    }

    constexpr stack_reference state_view::operator[](stack_index index) const noexcept
    {
        return stack_reference(state_, index);
    }

    constexpr stack_reference state_view::at(stack_index index) const noexcept
    {
        return stack_reference(state_, index);
    }

    constexpr stack_reference state_view::top() const noexcept
    {
        return stack_reference(state_, top_index);
    }

    constexpr stack_reference state_view::base() const noexcept
    {
        return stack_reference(state_, base_index);
    }

    constexpr void swap(state_view& a, state_view& b) noexcept
    {
        using std::swap;
        swap(a.state_, b.state_);
    }
}

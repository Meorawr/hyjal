// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "stack_index.hpp"
#include "stack_iterator.hpp"
#include "stack_reference.hpp"
#include "state.hpp"

#include <lua.hpp>

#include <iterator>
#include <utility>

namespace meorawr::hyjal::lua {
    class state_view {
    public:
        using size_type = index_size_t;
        using difference_type = index_difference_t;
        using iterator = stack_iterator;
        using reverse_iterator = std::reverse_iterator<stack_iterator>;

        constexpr state_view(state_t state) noexcept;
        constexpr state_view(std::nullptr_t) noexcept = delete;

        constexpr operator state_t() const noexcept;

        // Stack iterators

        iterator begin() const noexcept;
        stack_top_sentinel end() const noexcept;
        reverse_iterator rbegin() const noexcept;
        stack_base_sentinel rend() const noexcept;

        // Stack element accessors

        stack_reference operator[](stack_index index) const noexcept;
        stack_reference at(stack_index index) const noexcept;
        stack_reference top() const noexcept;
        stack_reference base() const noexcept;

        // Stack capacity

        bool empty() const noexcept;
        size_type size() const noexcept;
        constexpr size_type max_size() const noexcept;

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

    inline state_view::iterator state_view::begin() const noexcept
    {
        return iterator(state_, base_index);
    }

    inline stack_top_sentinel state_view::end() const noexcept
    {
        return {};
    }

    inline state_view::reverse_iterator state_view::rbegin() const noexcept
    {
        return std::reverse_iterator(iterator(state_, top_index));
    }

    inline stack_base_sentinel state_view::rend() const noexcept
    {
        return {};
    }

    inline stack_reference state_view::operator[](stack_index index) const noexcept
    {
        return stack_reference(state_, index);
    }

    inline stack_reference state_view::at(stack_index index) const noexcept
    {
        return stack_reference(state_, index);
    }

    inline stack_reference state_view::top() const noexcept
    {
        return stack_reference(state_, top_index);
    }

    inline stack_reference state_view::base() const noexcept
    {
        return stack_reference(state_, base_index);
    }

    inline bool state_view::empty() const noexcept
    {
        return lua_gettop(state_) == 0;
    }

    inline state_view::size_type state_view::size() const noexcept
    {
        return lua_gettop(state_);
    }

    inline constexpr state_view::size_type state_view::max_size() const noexcept
    {
        return LUAI_MAXCSTACK;
    }

    constexpr void swap(state_view& a, state_view& b) noexcept
    {
        using std::swap;
        swap(a.state_, b.state_);
    }
}

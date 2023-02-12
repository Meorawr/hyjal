// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "../detail/concepts.hpp"
#include "stack_algorithms.hpp"
#include "stack_iterator.hpp"
#include "stack_reference.hpp"
#include "state_deleter.hpp"

#include <lua.hpp>

#include <iterator>
#include <memory>
#include <utility>

namespace meorawr::hyjal::lua {
    template<detail::pointer_to<lua_State> Pointer>
    class basic_state {
    public:
        using size_type = index_size_t;
        using difference_type = index_difference_t;
        using iterator = stack_iterator;
        using reverse_iterator = std::reverse_iterator<stack_iterator>;

        constexpr basic_state(Pointer state) noexcept
            : state_(std::move(state))
        {
        }

        constexpr basic_state(std::nullptr_t) noexcept = delete;

        template<typename... Args>
        requires std::constructible_from<Pointer, Args...>
        constexpr basic_state(Args&&... args)
            : state_(std::forward<Args>(args)...)
        {
        }

        constexpr operator lua_State*() const noexcept
        {
            return std::to_address(state_);
        }

        // Stack iterators

        iterator begin() const noexcept
        {
            return iterator(state_, base_index);
        }

        stack_top_sentinel end() const noexcept
        {
            return {};
        }

        reverse_iterator rbegin() const noexcept
        {
            return std::reverse_iterator(iterator(state_, top_index));
        }

        stack_base_sentinel rend() const noexcept
        {
            return {};
        }

        // Stack element accessors

        stack_reference operator[](stack_index index) const noexcept
        {
            return stack_reference(state_, index);
        }

        stack_reference at(stack_index index) const noexcept
        {
            return stack_reference(state_, index);
        }

        stack_reference top() const noexcept
        {
            return stack_reference(state_, top_index);
        }

        stack_reference base() const noexcept
        {
            return stack_reference(state_, base_index);
        }

        // Stack capacity

        bool empty() const noexcept
        {
            return stack_algorithms::empty(state_);
        }

        size_type size() const noexcept
        {
            return stack_algorithms::size(state_);
        }

        constexpr size_type max_size() const noexcept
        {
            return LUAI_MAXCSTACK;
        }

        // Non-member functions

        friend constexpr void swap(basic_state& a, basic_state& b) noexcept
        {
            using std::swap;
            swap(a.state_, b.state_);
        }

    private:
        Pointer state_ = {};
    };

    using state_view = basic_state<lua_State*>;
    using unique_state = basic_state<std::unique_ptr<lua_State, state_deleter>>;
}

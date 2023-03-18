// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "../detail/concepts.hpp"
#include "stack_algorithms.hpp"
#include "stack_iterator.hpp"
#include "state_deleter.hpp"
#include "value_reference.hpp"

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

        basic_state(Pointer state) noexcept
            : state_(std::move(state))
        {
        }

        basic_state(std::nullptr_t) noexcept = delete;

        template<typename... Args>
        requires std::constructible_from<Pointer, Args...>
        basic_state(Args&&... args)
            : state_(std::forward<Args>(args)...)
        {
        }

        operator lua_State*() const noexcept { return std::to_address(state_); }

        // Stack iterators

        auto begin() const noexcept { return stack_algorithms::begin(*this); }
        auto end() const noexcept { return stack_algorithms::end(*this); }
        auto rbegin() const noexcept { return stack_algorithms::rbegin(*this); }
        auto rend() const noexcept { return stack_algorithms::rend(*this); }

        // Stack element accessors

        value_reference operator[](stack_index index) const noexcept { return value_reference(*this, index); }
        value_reference at(stack_index index) const noexcept { return value_reference(*this, index); }
        value_reference top() const noexcept { return value_reference(*this, top_index); }
        value_reference base() const noexcept { return value_reference(*this, base_index); }

        // Stack capacity

        bool empty() const noexcept { return stack_algorithms::empty(*this); }
        auto size() const noexcept { return stack_algorithms::size(*this); }
        auto max_size() const noexcept { return stack_algorithms::max_size(*this); }

        // Stack operations

        void clear() const noexcept { return stack_algorithms::clear(*this); }
        void resize(size_type size) const { stack_algorithms::resize(*this, size); }
        void reserve(difference_type count) const { stack_algorithms::reserve(*this, count); }

        void push_back(pushable_value auto&& value) const { lua::push_back(*this, std::forward<decltype(value)>(value)); }

        void pop_back() const { stack_algorithms::pop_back(*this); }
        void pop_n(size_type count) const { stack_algorithms::pop_n(*this, count); }

        // Non-member functions

        friend inline void swap(basic_state& a, basic_state& b) noexcept
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

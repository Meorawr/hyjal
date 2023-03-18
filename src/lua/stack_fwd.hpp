// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "value_push.hpp"
#include "value_type.hpp"

#include <concepts>
#include <string_view>

namespace meorawr::hyjal::lua {
    class stack_iterator;
    class stack_iterator_proxy;
    class stack_sentinel;
    class stack_top_sentinel;
    class stack_base_sentinel;
    class value_reference;

    namespace stack_algorithms {
        // Stack capacity

        bool empty(lua_State* state) noexcept;
        index_size_t size(lua_State* state) noexcept;
        constexpr index_size_t max_size(lua_State* state) noexcept;

        // Stack iteration

        using stack_forward_iterator = stack_iterator;
        using stack_forward_sentinel = stack_top_sentinel;
        using stack_reverse_iterator = std::reverse_iterator<stack_forward_iterator>;
        using stack_reverse_sentinel = stack_base_sentinel;

        stack_forward_iterator begin(lua_State* state) noexcept;
        stack_forward_sentinel end(lua_State* state) noexcept;

        stack_reverse_iterator rbegin(lua_State* state) noexcept;
        stack_reverse_sentinel rend(lua_State* state) noexcept;

        // Stack element accessors

        value_reference front(lua_State* state) noexcept;
        value_reference back(lua_State* state) noexcept;

        void clear(lua_State* state);
        void resize(lua_State* state, index_size_t size);
        void reserve(lua_State* state, index_difference_t count);

        index_t insert(lua_State* state, stack_index index, pushable_value auto&& v);
        index_t insert(lua_State* state, stack_index index, accessible_index from);

        template<index_concept<linear_index_tag> From>
        index_t insert(lua_State* state, stack_index index, From first, From last);

        index_t replace(lua_State* state, accessible_index index, pushable_value auto&& v);

        index_t erase(lua_State* state, stack_index index);
        index_t erase(lua_State* state, stack_index first, stack_index last);

        index_t copy(lua_State* state, accessible_index from, accessible_index to);

        template<index_concept<linear_index_tag> From, index_concept<linear_index_tag> To>
        index_t copy(lua_State* state, From first, From last, To to);

        template<index_concept<linear_index_tag> From, index_concept<linear_index_tag> To>
        index_t copy_backward(lua_State* state, From first, From last, To to);

        void pop_back(lua_State* state);
        void pop_n(lua_State* state, index_size_t count);
    }
}

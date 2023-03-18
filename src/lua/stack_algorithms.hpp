// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"
#include "stack_fwd.hpp"
#include "stack_iterator.hpp"
#include "value_reference.hpp"

#include <lua.hpp>

namespace meorawr::hyjal::lua::stack_algorithms {
    inline void clear(lua_State* state)
    {
        lua_settop(state, 0);
    }

    inline void resize(lua_State* state, index_size_t size)
    {
        lua_settop(state, size);
    }

    inline void reserve(lua_State* state, index_difference_t count)
    {
        lua_checkstack(state, count);
    }

    inline bool empty(lua_State* state) noexcept
    {
        return lua_gettop(state) == 0;
    }

    inline index_size_t size(lua_State* state) noexcept
    {
        return lua_gettop(state);
    }

    constexpr index_size_t max_size(lua_State*) noexcept
    {
        return LUAI_MAXCSTACK;
    }

    inline stack_forward_iterator begin(lua_State* state) noexcept
    {
        return stack_forward_iterator(state, base_index);
    }

    inline stack_forward_sentinel end(lua_State*) noexcept
    {
        return {};
    }

    inline stack_reverse_iterator rbegin(lua_State* state) noexcept
    {
        return stack_reverse_iterator(stack_forward_iterator(state, top_index));
    }

    inline stack_reverse_sentinel rend(lua_State*) noexcept
    {
        return {};
    }

    inline value_reference front(lua_State* state) noexcept
    {
        return value_reference(state, base_index);
    }

    inline value_reference back(lua_State* state) noexcept
    {
        return value_reference(state, absolute_index(state, top_index));
    }

    inline index_t insert(lua_State* state, stack_index index, pushable_value auto&& v)
    {
        push_back(state, std::forward<decltype(v)>(v));
        lua_insert(state, index); // TODO: adjust index
        return index;
    }

    inline index_t insert(lua_State* state, stack_index index, accessible_index from)
    {
        lua_pushvalue(state, from);
        lua_insert(state, index); // TODO: adjust index
        return index;
    }

    template<index_concept<linear_index_tag> From>
    inline index_t insert(lua_State* state, stack_index index, From first, From last);

    inline index_t replace(lua_State* state, accessible_index index, pushable_value auto&& v)
    {
        push_back(state, std::forward<decltype(v)>(v));
        lua_replace(state, index); // TODO: adjust index
        return index;
    }

    inline index_t erase(lua_State* state, stack_index index)
    {
        lua_remove(state, index);
        return index;
    }

    inline index_t erase(lua_State* state, stack_index first, stack_index last)
    {
        if (first >= last) [[unlikely]] {
            return last;
        }

        index_size_t old_size = lua_gettop(state);
        index_size_t new_size = old_size - (last - first);
        index_t after = first;

        while (last <= old_size) {
            lua_copy(state, last++, first++);
        }

        lua_settop(state, new_size);
        return after;
    }

    inline index_t copy(lua_State* state, accessible_index from, accessible_index to)
    {
        lua_copy(state, from, to);
        return to;
    }

    template<index_concept<linear_index_tag> From, index_concept<linear_index_tag> To>
    inline index_t copy(lua_State* state, From first, From last, To to)
    {
        while (first < last) {
            lua_copy(state, first++, to++);
        }
    }

    template<index_concept<linear_index_tag> From, index_concept<linear_index_tag> To>
    inline index_t copy_backward(lua_State* state, From first, From last, To to)
    {
        while (last > first) {
            lua_copy(state, --last, --to);
        }
    }

    inline void pop_back(lua_State* state)
    {
        lua_settop(state, -2);
    }

    inline void pop_n(lua_State* state, index_size_t count)
    {
        lua_settop(state, -(count + 1));
    }
}

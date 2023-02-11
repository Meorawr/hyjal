// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "stack_algorithms.hpp"
#include "stack_index.hpp"
#include "stack_iterator.hpp"
#include "stack_reference.hpp"
#include "state.hpp"

#include <lua.hpp>

#include <iterator>
#include <memory>
#include <utility>

namespace meorawr::hyjal::lua {
    template<typename Pointer>
    class basic_state_view {
    public:
        using size_type = index_size_t;
        using difference_type = index_difference_t;
        using iterator = stack_iterator;
        using reverse_iterator = std::reverse_iterator<stack_iterator>;

        constexpr basic_state_view() noexcept = default;
        constexpr basic_state_view(Pointer state) noexcept;
        constexpr basic_state_view(std::nullptr_t) noexcept = delete;

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

        friend constexpr void swap(basic_state_view& a, basic_state_view& b) noexcept;

    private:
        Pointer state_ = {};
    };

    template<typename Pointer>
    constexpr basic_state_view<Pointer>::basic_state_view(Pointer state) noexcept
        : state_(std::move(state))
    {
    }

    template<typename Pointer>
    constexpr basic_state_view<Pointer>::operator state_t() const noexcept
    {
        return std::to_address(state_);
    }

    template<typename Pointer>
    inline auto basic_state_view<Pointer>::begin() const noexcept -> iterator
    {
        return iterator(state_, base_index);
    }

    template<typename Pointer>
    inline stack_top_sentinel basic_state_view<Pointer>::end() const noexcept
    {
        return {};
    }

    template<typename Pointer>
    inline auto basic_state_view<Pointer>::rbegin() const noexcept -> reverse_iterator
    {
        return std::reverse_iterator(iterator(state_, top_index));
    }

    template<typename Pointer>
    inline stack_base_sentinel basic_state_view<Pointer>::rend() const noexcept
    {
        return {};
    }

    template<typename Pointer>
    inline stack_reference basic_state_view<Pointer>::operator[](stack_index index) const noexcept
    {
        return stack_reference(state_, index);
    }

    template<typename Pointer>
    inline stack_reference basic_state_view<Pointer>::at(stack_index index) const noexcept
    {
        return stack_reference(state_, index);
    }

    template<typename Pointer>
    inline stack_reference basic_state_view<Pointer>::top() const noexcept
    {
        return stack_reference(state_, top_index);
    }

    template<typename Pointer>
    inline stack_reference basic_state_view<Pointer>::base() const noexcept
    {
        return stack_reference(state_, base_index);
    }

    template<typename Pointer>
    inline bool basic_state_view<Pointer>::empty() const noexcept
    {
        return stack_algorithms::empty(state_);
    }

    template<typename Pointer>
    inline auto basic_state_view<Pointer>::size() const noexcept -> size_type
    {
        return stack_algorithms::size(state_);
    }

    template<typename Pointer>
    inline constexpr auto basic_state_view<Pointer>::max_size() const noexcept -> size_type
    {
        return LUAI_MAXCSTACK;
    }

    template<typename Pointer>
    constexpr void swap(basic_state_view<Pointer>& a, basic_state_view<Pointer>& b) noexcept
    {
        using std::swap;
        swap(a.state_, b.state_);
    }

    using state_view = basic_state_view<state_t>;
}

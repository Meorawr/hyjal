// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"
#include "stack_fwd.hpp"
#include "value_reference.hpp"

#include <concepts>
#include <iterator>

namespace meorawr::hyjal::lua {
    class stack_iterator_proxy {
    public:
        stack_iterator_proxy(lua_State* state, absolute_index index) noexcept
            : ref_(state, index)
        {
        }

        value_reference* operator->() noexcept { return &ref_; }

    private:
        value_reference ref_;
    };

    class stack_iterator {
    public:
        using difference_type = index_difference_t;
        using value_type = value_reference;
        using pointer = stack_iterator_proxy;
        using reference = value_reference;
        using iterator_concept = std::random_access_iterator_tag;

        stack_iterator() noexcept = default;
        stack_iterator(lua_State* state, absolute_index index) noexcept;
        stack_iterator(lua_State* state, stack_index index) noexcept;

        // Input iterator operators

        reference operator*() const noexcept;
        pointer operator->() const noexcept;

        stack_iterator& operator++() noexcept;
        stack_iterator operator++(int) noexcept;

        // Forward iterator operators

        friend bool operator==(const stack_iterator& a, const stack_iterator& b) noexcept;

        // Bidirectional iterator operators

        stack_iterator& operator--() noexcept;
        stack_iterator operator--(int) noexcept;

        // Random-access iterator operators

        reference operator[](difference_type offset) const noexcept;

        stack_iterator& operator+=(difference_type offset) noexcept;
        stack_iterator& operator-=(difference_type offset) noexcept;

        friend stack_iterator operator+(const stack_iterator& iter, difference_type offset) noexcept;
        friend stack_iterator operator+(difference_type offset, const stack_iterator& iter) noexcept;
        friend stack_iterator operator-(const stack_iterator& iter, difference_type offset) noexcept;

        friend difference_type operator-(const stack_iterator& a, const stack_iterator& b) noexcept;

        friend std::partial_ordering operator<=>(const stack_iterator& a, const stack_iterator& b) noexcept;

        // Non-member functions

        friend void swap(stack_iterator& a, stack_iterator& b) noexcept;

        friend bool operator==(const stack_iterator& iter, stack_sentinel) noexcept;
        friend bool operator==(const stack_iterator& iter, stack_base_sentinel) noexcept;
        friend bool operator==(const stack_iterator& iter, stack_top_sentinel) noexcept;

        friend difference_type operator-(const stack_iterator& iter, stack_base_sentinel) noexcept;
        friend difference_type operator-(stack_base_sentinel, const stack_iterator& iter) noexcept;
        friend difference_type operator-(const stack_iterator& iter, stack_top_sentinel) noexcept;
        friend difference_type operator-(stack_top_sentinel, const stack_iterator& iter) noexcept;

    private:
        lua_State* state_ = {};
        absolute_index index_ = {};
    };

    inline stack_iterator::stack_iterator(lua_State* state, absolute_index index) noexcept
        : state_(state)
        , index_(index)
    {
    }

    inline stack_iterator::stack_iterator(lua_State* state, stack_index index) noexcept
        : stack_iterator(state, absolute_index(state, index))
    {
    }

    inline stack_iterator::reference stack_iterator::operator*() const noexcept
    {
        return reference(state_, index_);
    }

    inline stack_iterator::pointer stack_iterator::operator->() const noexcept
    {
        return pointer(state_, index_);
    }

    inline stack_iterator& stack_iterator::operator++() noexcept
    {
        ++index_;
        return *this;
    }

    inline stack_iterator stack_iterator::operator++(int) noexcept
    {
        stack_iterator copy = *this;
        ++index_;
        return copy;
    }

    inline bool operator==(const stack_iterator& a, const stack_iterator& b) noexcept
    {
        return a.state_ == b.state_ && a.index_ == b.index_;
    }

    inline stack_iterator& stack_iterator::operator--() noexcept
    {
        --index_;
        return *this;
    }

    inline stack_iterator stack_iterator::operator--(int) noexcept
    {
        stack_iterator copy = *this;
        --index_;
        return copy;
    }

    inline stack_iterator::reference stack_iterator::operator[](difference_type offset) const noexcept
    {
        return reference(state_, index_ + offset);
    }

    inline stack_iterator& stack_iterator::operator+=(difference_type offset) noexcept
    {
        index_ += offset;
        return *this;
    }

    inline stack_iterator& stack_iterator::operator-=(difference_type offset) noexcept
    {
        index_ -= offset;
        return *this;
    }

    inline stack_iterator operator+(const stack_iterator& iter, stack_iterator::difference_type offset) noexcept
    {
        stack_iterator copy = iter;
        copy.index_ += offset;
        return copy;
    }

    inline stack_iterator operator+(stack_iterator::difference_type offset, const stack_iterator& iter) noexcept
    {
        stack_iterator copy = iter;
        copy.index_ += offset;
        return copy;
    }

    inline stack_iterator operator-(const stack_iterator& iter, stack_iterator::difference_type offset) noexcept
    {
        stack_iterator copy = iter;
        copy.index_ -= offset;
        return copy;
    }

    inline stack_iterator::difference_type operator-(const stack_iterator& a, const stack_iterator& b) noexcept
    {
        if (a.state_ != b.state_) [[unlikely]] {
            return 0;
        } else {
            return a.index_ - b.index_;
        }
    }

    inline std::partial_ordering operator<=>(const stack_iterator& a, const stack_iterator& b) noexcept
    {
        if (a.state_ != b.state_) [[unlikely]] {
            return std::partial_ordering::unordered;
        } else {
            return a.index_ <=> b.index_;
        }
    }

    inline void swap(stack_iterator& a, stack_iterator& b) noexcept
    {
        using std::swap;
        swap(a.state_, b.state_);
        swap(a.index_, b.index_);
    }

    static_assert(std::input_iterator<stack_iterator>);
    static_assert(std::forward_iterator<stack_iterator>);
    static_assert(std::bidirectional_iterator<stack_iterator>);
    static_assert(std::random_access_iterator<stack_iterator>);

    class stack_sentinel { };
    class stack_base_sentinel { };
    class stack_top_sentinel { };

    inline bool operator==(const stack_iterator& iter, stack_sentinel) noexcept
    {
        if (iter.state_ == nullptr) [[unlikely]] {
            return true;
        } else {
            return iter.index_ < 1 || iter.index_ > stack_algorithms::size(iter.state_);
        }
    }

    inline bool operator==(const stack_iterator& iter, stack_base_sentinel) noexcept
    {
        return iter.index_ < 1;
    }

    inline bool operator==(const stack_iterator& iter, stack_top_sentinel) noexcept
    {
        if (iter.state_ == nullptr) [[unlikely]] {
            return true;
        } else {
            return iter.index_ > stack_algorithms::size(iter.state_);
        }
    }

    inline stack_iterator::difference_type operator-(const stack_iterator& iter, stack_base_sentinel) noexcept
    {
        return -iter.index_;
    }

    inline stack_iterator::difference_type operator-(stack_base_sentinel, const stack_iterator& iter) noexcept
    {
        return iter.index_;
    }

    inline stack_iterator::difference_type operator-(const stack_iterator& iter, stack_top_sentinel) noexcept
    {
        if (iter.state_ == nullptr) [[unlikely]] {
            return 0;
        } else {
            return iter.index_ - stack_algorithms::size(iter.state_) + 1;
        }
    }

    inline stack_iterator::difference_type operator-(stack_top_sentinel, const stack_iterator& iter) noexcept
    {
        if (iter.state_ == nullptr) [[unlikely]] {
            return 0;
        } else {
            return stack_algorithms::size(iter.state_) - iter.index_ + 1;
        }
    }

    static_assert(std::sentinel_for<stack_sentinel, stack_iterator>);
    static_assert(std::sized_sentinel_for<stack_base_sentinel, stack_iterator>);
    static_assert(std::sized_sentinel_for<stack_top_sentinel, stack_iterator>);
}

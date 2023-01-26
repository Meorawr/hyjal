// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "child_list.hpp"

#include <concepts>
#include <iterator>
#include <type_traits>

namespace meorawr::hyjal {
    template<typename T>
    requires std::is_class_v<T> && std::same_as<T, std::remove_cv_t<T>>
    class child_list_view {
        using list_type = child_list<T>;
        static_assert(!list_type::stateful_value_traits);

    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = list_type::iterator;
        using const_iterator = list_type::const_iterator;
        using size_type = list_type::size_type;
        using difference_type = list_type::difference_type;

        child_list_view() noexcept = default;
        child_list_view(list_type& list) noexcept
            : begin_(list.begin())
            , end_(list.end())
        {
        }

        bool empty() const noexcept { return begin_ == end_; }
        size_type size() const noexcept { return static_cast<size_type>(std::distance(begin_, end_)); }

        iterator begin() const noexcept { return begin_; }
        const_iterator cbegin() const noexcept { return begin_; }
        iterator end() const noexcept { return end_; }
        const_iterator cend() const noexcept { return end_; }

        iterator iterator_to(reference ref) const noexcept { return iterator(std::addressof(ref), nullptr); }

    private:
        iterator begin_ = {};
        iterator end_ = {};
    };
}

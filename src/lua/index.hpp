// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <compare>
#include <concepts>

namespace meorawr::hyjal::lua {
    using index_t = int;
    using index_difference_t = int;
    using index_size_t = int;

    struct basic_index_tag { };
    struct linear_index_tag : basic_index_tag { };

    template<typename I>
    using index_category_t = typename I::index_category;

    template<typename I>
    using index_concept_t = typename index_category_t<I>::index_concept;

    // clang-format off

    template<typename I, typename C>
    concept compatible_index = std::same_as<index_category_t<I>, C>;

    template<typename I>
    concept integral_index = std::integral<I> && std::convertible_to<I, index_t>;

    template<typename I>
    concept basic_index = std::derived_from<index_concept_t<I>, basic_index_tag> && std::convertible_to<I, index_t> && requires(I index) {
        { index.i } -> std::same_as<index_t&>;
    };

    template<typename I>
    concept linear_index = basic_index<I> && std::derived_from<index_concept_t<I>, linear_index_tag>;

    // clang-format on

    // Basic index comparison operators

    template<basic_index I>
    constexpr bool operator==(I a, I b) noexcept
    {
        return a.i == b.i;
    }

    constexpr bool operator==(basic_index auto a, index_t b) noexcept
    {
        return static_cast<index_t>(a) == b;
    }

    constexpr auto operator==(basic_index auto, basic_index auto) noexcept = delete;
    constexpr auto operator<=>(basic_index auto, basic_index auto) noexcept = delete;
    constexpr auto operator<=>(basic_index auto, index_t) noexcept = delete;

    // Linear index comparison operators

    template<linear_index I>
    constexpr std::strong_ordering operator<=>(I a, I b) noexcept
    {
        return a.i <=> b.i;
    }

    constexpr std::strong_ordering operator<=>(linear_index auto a, index_t b) noexcept
    {
        return static_cast<index_t>(a) <=> b;
    }

    // Linear index arithmetic operators

    constexpr auto& operator++(linear_index auto& index) noexcept
    {
        ++index.i;
        return index;
    }

    constexpr auto operator++(linear_index auto& index, int) noexcept
    {
        auto copy = index;
        ++index.i;
        return index;
    }

    constexpr auto& operator--(linear_index auto& index) noexcept
    {
        --index.i;
        return index;
    }

    constexpr auto operator--(linear_index auto& index, int) noexcept
    {
        auto copy = index;
        --index.i;
        return index;
    }

    constexpr auto& operator+=(linear_index auto& index, index_difference_t offset) noexcept
    {
        index.i += offset;
        return index;
    }

    constexpr auto& operator-=(linear_index auto& index, index_difference_t offset) noexcept
    {
        index.i -= offset;
        return index;
    }

    constexpr auto operator+(linear_index auto index, index_difference_t offset) noexcept
    {
        index.i += offset;
        return index;
    }

    constexpr auto operator-(linear_index auto index, index_difference_t offset) noexcept
    {
        index.i -= offset;
        return index;
    }

    template<linear_index I>
    constexpr index_difference_t operator-(I lhs, I rhs) noexcept
    {
        return lhs.i - rhs.i;
    }
}

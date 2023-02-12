// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <lua.hpp>

#include <compare>
#include <concepts>

namespace meorawr::hyjal::lua {
    using index_t = int;
    using index_difference_t = int;
    using index_size_t = int;

    // Index concept tags

    struct basic_index_tag { };
    struct linear_index_tag : basic_index_tag { };

    // Index category tags

    struct raw_index_tag { };
    struct accessible_index_tag { };
    struct stack_index_tag : accessible_index_tag { };
    struct pseudo_index_tag : accessible_index_tag { };
    struct upvalue_index_tag : accessible_index_tag { };

    // Index types

    struct raw_index;
    struct accessible_index;
    struct stack_index;
    struct absolute_index;
    struct relative_index;
    struct pseudo_index;
    struct upvalue_index;

    /**
     * Index type traits and concepts
     */

    template<typename I>
    using index_category_t = typename I::category_type;

    template<typename I>
    using index_concept_t = typename I::concept_type;

    // clang-format off

    template<typename I, typename C>
    concept index_category = std::derived_from<index_category_t<I>, C>;

    template<typename I, typename C>
    concept index_concept = std::derived_from<index_concept_t<I>, C>;

    template<typename I>
    concept basic_index = index_concept<I, basic_index_tag> && std::convertible_to<I, index_t> && requires(I index) {
        typename I::category_type;

        { index.i } -> std::same_as<index_t&>;
    };

    template<typename I>
    concept linear_index = basic_index<I> && index_concept<I, linear_index_tag>;

    // clang-format on

    /**
     * Index types
     */

    struct raw_index {
        using category_type = raw_index_tag;
        using concept_type = basic_index_tag;

        constexpr raw_index() noexcept = default;
        constexpr raw_index(index_t i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    constexpr raw_index::raw_index(index_t i) noexcept
        : i(i)
    {
    }

    struct accessible_index {
        using category_type = accessible_index_tag;
        using concept_type = basic_index_tag;

        constexpr accessible_index() noexcept = default;
        constexpr accessible_index(index_category<category_type> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    constexpr accessible_index::accessible_index(index_category<category_type> auto i) noexcept
        : i(static_cast<index_t>(i))
    {
    }

    struct stack_index {
        using category_type = stack_index_tag;
        using concept_type = linear_index_tag;

        constexpr stack_index() noexcept = default;
        consteval stack_index(std::integral auto i) noexcept;
        constexpr stack_index(index_category<category_type> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    consteval stack_index::stack_index(std::integral auto i) noexcept
        : i((i >= -LUAI_MAXCSTACK && i <= LUAI_MAXCSTACK && i != 0) ? i : throw)
    {
    }

    constexpr stack_index::stack_index(index_category<category_type> auto i) noexcept
        : i(static_cast<index_t>(i))
    {
    }

    struct absolute_index {
        using category_type = stack_index_tag;
        using concept_type = linear_index_tag;

        constexpr absolute_index() noexcept = default;
        consteval absolute_index(std::integral auto i) noexcept;
        constexpr absolute_index(lua_State* state, std::integral auto i) noexcept;
        constexpr absolute_index(lua_State* state, index_category<category_type> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    consteval absolute_index::absolute_index(std::integral auto i) noexcept
        : i((i > 0 && i <= LUAI_MAXCSTACK) ? i : throw)
    {
    }

    constexpr absolute_index::absolute_index(lua_State* state, std::integral auto i) noexcept
        : i((i > 0) ? i : lua_absindex(state, i))
    {
    }

    constexpr absolute_index::absolute_index(lua_State* state, index_category<category_type> auto i) noexcept
        : absolute_index(state, static_cast<index_t>(i))
    {
    }

    struct relative_index {
        using category_type = stack_index_tag;
        using concept_type = linear_index_tag;

        constexpr relative_index() noexcept = default;
        consteval relative_index(std::integral auto i) noexcept;
        constexpr relative_index(lua_State* state, std::integral auto i) noexcept;
        constexpr relative_index(lua_State* state, index_category<category_type> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    consteval relative_index::relative_index(std::integral auto i) noexcept
        : i((i >= -LUAI_MAXCSTACK && i < 0) ? i : throw)
    {
    }

    constexpr relative_index::relative_index(lua_State* state, std::integral auto i) noexcept
        : i((i < 0) ? i : -(lua_gettop(state) - i + 1))
    {
    }

    constexpr relative_index::relative_index(lua_State* state, index_category<category_type> auto i) noexcept
        : relative_index(state, static_cast<index_t>(i))
    {
    }

    struct pseudo_index {
        using category_type = pseudo_index_tag;
        using concept_type = basic_index_tag;

        constexpr pseudo_index() noexcept = delete;
        consteval pseudo_index(std::integral auto i) noexcept;
        consteval pseudo_index(std::same_as<lua_PseudoIndex> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i;
    };

    consteval pseudo_index::pseudo_index(std::integral auto i) noexcept
        : i((i >= LUA_GLOBALSINDEX && i <= LUA_ERRORHANDLERINDEX) ? i : throw)
    {
    }

    consteval pseudo_index::pseudo_index(std::same_as<lua_PseudoIndex> auto i) noexcept
        : pseudo_index(static_cast<index_t>(i))
    {
    }

    struct upvalue_index {
        using category_type = upvalue_index_tag;
        using concept_type = linear_index_tag;

        constexpr upvalue_index() noexcept = default;
        consteval upvalue_index(std::integral auto i) noexcept;

        constexpr operator index_t() const noexcept { return lua_upvalueindex(i); }

        index_t i = {};
    };

    consteval upvalue_index::upvalue_index(std::integral auto i) noexcept
        : i((i > 0 && i <= LUAI_MAXUPVALUES) ? i : throw)
    {
    }

    /**
     * Index operators
     */

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

    // TODO: Rewrite the two deletions below back to concept-auto templates
    //       pending resolution of this (possible) MSVC 19.33+ bug:
    //
    //       <https://developercommunity.visualstudio.com/t/10277316>

    template<basic_index I1, basic_index I2>
    constexpr bool operator==(I1, I2) noexcept = delete;

    template<basic_index I1, basic_index I2>
    constexpr std::strong_ordering operator<=>(I1, I2) noexcept = delete;
    constexpr std::strong_ordering operator<=>(basic_index auto, index_t) noexcept = delete;
    constexpr std::strong_ordering operator<=>(index_t, basic_index auto) noexcept = delete;

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

    constexpr std::strong_ordering operator<=>(index_t a, linear_index auto b) noexcept
    {
        return a <=> static_cast<index_t>(b);
    }

    // Linear index arithmetic operators

    constexpr auto& operator++(linear_index auto& index) noexcept
    {
        ++index.i;
        return index;
    }

    constexpr auto operator++(linear_index auto index, int) noexcept
    {
        ++index.i;
        return index;
    }

    constexpr auto& operator--(linear_index auto& index) noexcept
    {
        --index.i;
        return index;
    }

    constexpr auto operator--(linear_index auto index, int) noexcept
    {
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

    /**
     * Index constants
     */

    inline constexpr absolute_index base_index = 1;
    inline constexpr relative_index top_index = -1;

    inline constexpr pseudo_index globals_index = LUA_GLOBALSINDEX;
    inline constexpr pseudo_index registry_index = LUA_REGISTRYINDEX;
    inline constexpr pseudo_index error_handler_index = LUA_ERRORHANDLERINDEX;
    inline constexpr pseudo_index environment_index = LUA_ENVIRONINDEX;
}

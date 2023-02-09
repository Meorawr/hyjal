// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"
#include "stack_algorithms.hpp"

#include <lua.hpp>

namespace meorawr::hyjal::lua {
    struct stack_index_category {
        using index_concept = linear_index_tag;
    };

    struct stack_index {
        using index_category = stack_index_category;

        constexpr stack_index() noexcept = default;
        consteval stack_index(integral_index auto i) noexcept;
        constexpr stack_index(compatible_index<stack_index_category> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    struct absolute_index {
        using index_category = stack_index_category;

        constexpr absolute_index() noexcept = default;
        consteval absolute_index(integral_index auto i) noexcept;
        absolute_index(state_t state, integral_index auto i) noexcept;
        absolute_index(state_t state, compatible_index<stack_index_category> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    struct relative_index {
        using index_category = stack_index_category;

        constexpr relative_index() noexcept = default;
        consteval relative_index(integral_index auto i) noexcept;
        relative_index(state_t state, integral_index auto i) noexcept;
        relative_index(state_t state, compatible_index<stack_index_category> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    consteval stack_index::stack_index(integral_index auto i) noexcept
        : i((i >= -LUAI_MAXCSTACK && i <= LUAI_MAXCSTACK) ? i : throw)
    {
    }

    constexpr stack_index::stack_index(compatible_index<stack_index_category> auto i) noexcept
        : i(static_cast<index_t>(i))
    {
    }

    consteval absolute_index::absolute_index(integral_index auto i) noexcept
        : i((i > 0 && i <= LUAI_MAXCSTACK) ? i : throw)
    {
    }

    absolute_index::absolute_index(state_t state, integral_index auto i) noexcept
        : i((i < 0) ? i : stack_algorithms::absolute_index(state, i))
    {
    }

    absolute_index::absolute_index(state_t state, compatible_index<stack_index_category> auto i) noexcept
        : absolute_index(state, static_cast<index_t>(i))
    {
    }

    consteval relative_index::relative_index(integral_index auto i) noexcept
        : i((i >= -LUAI_MAXCSTACK && i < 0) ? i : throw)
    {
    }

    relative_index::relative_index(state_t state, integral_index auto i) noexcept
        : i((i < 0) ? i : stack_algorithms::relative_index(state, i))
    {
    }

    relative_index::relative_index(state_t state, compatible_index<stack_index_category> auto i) noexcept
        : relative_index(state, static_cast<index_t>(i))
    {
    }

    inline constexpr absolute_index base_index = 1;
    inline constexpr relative_index top_index = -1;
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"

#include <lua.hpp>

namespace meorawr::hyjal::lua {
    struct upvalue_index_category {
        using index_concept = linear_index_tag;
    };

    struct upvalue_index {
        using index_category = upvalue_index_category;

        constexpr upvalue_index() noexcept = default;
        consteval upvalue_index(integral_index auto i) noexcept;

        constexpr operator index_t() const noexcept { return lua_upvalueindex(i); }

        index_t i = {};
    };

    consteval upvalue_index::upvalue_index(integral_index auto i) noexcept
        : i((i >= 1 && i <= LUAI_MAXUPVALUES) ? i : throw)
    {
    }
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"

#include <lua.hpp>

#include <utility>

namespace meorawr::hyjal::lua {
    struct pseudo_index_category {
        using index_concept = basic_index_tag;
    };

    struct pseudo_index {
        using index_category = pseudo_index_category;

        consteval pseudo_index(integral_index auto i) noexcept;
        consteval pseudo_index(std::same_as<lua_PseudoIndex> auto i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i;
    };

    consteval pseudo_index::pseudo_index(integral_index auto i) noexcept
        : i((i >= LUA_GLOBALSINDEX && i <= LUA_ERRORHANDLERINDEX) ? i : throw)
    {
    }

    consteval pseudo_index::pseudo_index(std::same_as<lua_PseudoIndex> auto i) noexcept
        : pseudo_index(std::to_underlying(i))
    {
    }

    inline constexpr pseudo_index globals_index = LUA_GLOBALSINDEX;
    inline constexpr pseudo_index registry_index = LUA_REGISTRYINDEX;
    inline constexpr pseudo_index error_handler_index = LUA_ERRORHANDLERINDEX;
    inline constexpr pseudo_index environment_index = LUA_ENVIRONINDEX;
}

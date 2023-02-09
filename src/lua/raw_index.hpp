// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "index.hpp"

namespace meorawr::hyjal::lua {
    struct raw_index_category {
        using index_concept = basic_index_tag;
    };

    struct raw_index {
        using index_category = raw_index_category;

        constexpr raw_index() noexcept = default;
        constexpr raw_index(index_t i) noexcept;

        constexpr operator index_t() const noexcept { return i; }

        index_t i = {};
    };

    constexpr raw_index::raw_index(index_t i) noexcept
        : i(i)
    {
    }
}

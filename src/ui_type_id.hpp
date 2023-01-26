// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>
#include <string_view>

namespace meorawr::hyjal {
    class ui_type_id {
    public:
        consteval ui_type_id(std::string_view guid) noexcept;

        constexpr friend bool operator==(ui_type_id lhs, ui_type_id rhs) noexcept;

    private:
        std::uint32_t hash_;
    };

    consteval ui_type_id::ui_type_id(std::string_view guid) noexcept
    {
        constexpr std::uint32_t offset_basis = 0x811c9dc5;
        constexpr std::uint32_t prime = 0x01000193;

        hash_ = offset_basis;

        for (char c : guid) {
            hash_ = (hash_ ^ c) * prime;
        }
    }

    constexpr bool operator==(ui_type_id lhs, ui_type_id rhs) noexcept
    {
        return lhs.hash_ == rhs.hash_;
    }
}

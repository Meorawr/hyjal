// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "object.hpp"
#include "object_list_hook.hpp"

namespace meorawr::hyjal {
    class font : public object, public object_list_hook<font> {
    public:
        font(std::string_view name, ui_service& owner);
        font(const font&) = delete;
        font(font&&) = delete;

        font& operator=(const font&) = delete;
        font& operator=(font&&) = delete;

        ~font() noexcept override;

    protected:
        font(std::string_view name, const ui_type_info& type, ui_service& owner);
    };

    template<>
    struct ui_type_traits<font> {
        using derived_type = object;

        static constexpr ui_type_id type_id = {"182314f4-ca35-457f-8d21-c4b3e13fe799"};
        static constexpr std::string_view type_name = "Font";
    };
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "animatable_object.hpp"
#include "child_list_hook.hpp"
#include "object.hpp"

namespace meorawr::hyjal {
    class frame;

    class region : public object, public animatable_object, public child_list_hook<region, frame> {
    public:
        region(const region&) = delete;
        region(region&&) = delete;

        region& operator=(const region&) = delete;
        region& operator=(region&&) = delete;

        ~region() noexcept override;

        frame& parent() const noexcept;

        void set_parent(frame& parent);

    protected:
        region(std::string_view name, const ui_type_info& type, frame& parent);
    };

    template<>
    struct ui_type_traits<region> {
        using derived_type = object;

        static constexpr ui_type_id type_id = {"d3c0a94d-b1ff-4359-86bb-46b3f09d2ae3"};
        static constexpr std::string_view type_name = "Region";
    };
}

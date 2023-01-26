// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "child_list_hook.hpp"
#include "object.hpp"
#include "object_list_hook.hpp"

namespace meorawr::hyjal {
    class animation_group;

    class animation : public object, public object_list_hook<animation>, public child_list_hook<animation, animation_group> {
    public:
        animation(std::string_view name, animation_group& parent);
        animation(const animation&) = delete;
        animation(animation&&) = delete;

        animation& operator=(const animation&) = delete;
        animation& operator=(animation&&) = delete;

        ~animation() noexcept override;

        animation_group& parent() const noexcept;

        void set_parent(animation_group& parent);

    protected:
        animation(std::string_view name, const ui_type_info& type, animation_group& parent);
    };

    template<>
    struct ui_type_traits<animation> {
        using derived_type = object;

        static constexpr ui_type_id type_id = {"4c455a1b-621c-4fcb-b2d9-89349913e8ed"};
        static constexpr std::string_view type_name = "Animation";
    };
}

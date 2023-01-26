// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "child_list.hpp"
#include "child_list_hook.hpp"
#include "child_list_traits.hpp"
#include "child_list_view.hpp"
#include "object.hpp"

namespace meorawr::hyjal {
    class animatable_object;
    class animation;

    class animation_group : public object, public child_list_hook<animation_group, animatable_object> {
    public:
        animation_group(std::string_view name, animatable_object& parent);
        animation_group(const animation_group&) = delete;
        animation_group(animation_group&&) = delete;

        animation_group& operator=(const animation_group&) = delete;
        animation_group& operator=(animation_group&&) = delete;

        ~animation_group() noexcept override;

        child_list_view<animation> animations() const noexcept;
        animatable_object& parent() const noexcept;

        void set_parent(animatable_object& parent);

    protected:
        animation_group(std::string_view name, const ui_type_info& type, animatable_object& parent);

    private:
        void link_child(animation& animation) noexcept;
        void unlink_child(animation& animation) noexcept;

        child_list<animation> animations_;

        friend child_list_traits<animation>;
    };

    template<>
    struct ui_type_traits<animation_group> {
        using derived_type = object;

        static constexpr ui_type_id type_id = {"b7ad7f48-d9ce-4ba9-835b-e178be8bdacb"};
        static constexpr std::string_view type_name = "AnimationGroup";
    };
}

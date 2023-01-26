// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "child_list.hpp"
#include "child_list_traits.hpp"
#include "child_list_view.hpp"
#include "object_base.hpp"

namespace meorawr::hyjal {
    class animation_group;

    class animatable_object : public virtual object_base {
    public:
        animatable_object(const animatable_object&) = delete;
        animatable_object(animatable_object&&) = delete;

        animatable_object& operator=(const animatable_object&) = delete;
        animatable_object& operator=(animatable_object&&) = delete;

        child_list_view<animation_group> animation_groups() const noexcept;

    protected:
        animatable_object() noexcept;

        ~animatable_object() noexcept override;

    private:
        void link_child(animation_group& group) noexcept;
        void unlink_child(animation_group& group) noexcept;

        child_list<animation_group> groups_;

        friend child_list_traits<animation_group>;
    };
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "animatable_object.hpp"

#include "animation_group.hpp"
#include "ui_service.hpp"

namespace meorawr::hyjal {
    animatable_object::animatable_object() noexcept = default;

    animatable_object::~animatable_object() noexcept
    {
        groups_.clear_and_dispose(delete_object<animation_group>);
    }

    child_list_view<animation_group> animatable_object::animation_groups() const noexcept
    {
        return const_cast<child_list<animation_group>&>(groups_);
    }

    void animatable_object::link_child(animation_group& group) noexcept
    {
        groups_.push_back(group);
    }

    void animatable_object::unlink_child(animation_group& group) noexcept
    {
        groups_.erase(groups_.iterator_to(group));
    }
}

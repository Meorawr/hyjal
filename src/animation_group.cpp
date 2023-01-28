// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "animation_group.hpp"

#include "animatable_object.hpp"
#include "animation.hpp"
#include "ui_service.hpp"

namespace meorawr::hyjal {
    animation_group::animation_group(std::string_view name, animatable_object& parent)
        : animation_group(name, ui_type_info_v<animation_group>, parent)
    {
    }

    animation_group::animation_group(std::string_view name, const ui_type_info& type, animatable_object& parent)
        : object(name, type, parent.owner())
        , child_list_hook(&parent)
    {
    }

    animation_group::~animation_group() noexcept
    {
        animations_.clear_and_dispose(delete_object);
    }

    child_list_view<animation> animation_group::animations() const& noexcept
    {
        return const_cast<child_list<animation>&>(animations_);
    }

    animatable_object& animation_group::parent() const noexcept
    {
        return *child_list_hook::parent();
    }

    void animation_group::set_parent(animatable_object& parent)
    {
        child_list_hook::set_parent(&parent);
    }

    void animation_group::link_child(animation& animation) noexcept
    {
        animations_.push_back(animation);
    }

    void animation_group::unlink_child(animation& animation) noexcept
    {
        animations_.erase(animations_.iterator_to(animation));
    }
}

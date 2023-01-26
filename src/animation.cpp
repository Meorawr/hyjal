// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "animation.hpp"

#include "animation_group.hpp"
#include "ui_service.hpp"

namespace meorawr::hyjal {
    animation::animation(std::string_view name, animation_group& parent)
        : animation(name, ui_type_info_v<animation>, parent)
    {
    }

    animation::animation(std::string_view name, const ui_type_info& type, animation_group& parent)
        : object(name, type, parent.owner())
        , object_list_hook<animation>(parent.owner())
        , child_list_hook(&parent)
    {
    }

    animation::~animation() noexcept = default;

    animation_group& animation::parent() const noexcept
    {
        return *child_list_hook::parent();
    }

    void animation::set_parent(animation_group& parent)
    {
        child_list_hook::set_parent(&parent);
    }
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "region.hpp"

#include "frame.hpp"

namespace meorawr::hyjal {
    region::region(std::string_view name, const ui_type_info& type, frame& parent)
        : object(name, type, parent.owner())
        , child_list_hook(&parent)
    {
    }

    region::~region() noexcept = default;

    frame& region::parent() const noexcept
    {
        return *child_list_hook::parent();
    }

    void region::set_parent(frame& parent)
    {
        child_list_hook::set_parent(&parent);
    }
}

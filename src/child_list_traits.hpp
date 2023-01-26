// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace meorawr::hyjal {
    template<typename Child>
    struct child_list_traits {
        template<typename Parent>
        static void link_child(Parent& parent, Child& child) noexcept
        {
            parent.link_child(child);
        }

        template<typename Parent>
        static void unlink_child(Parent& parent, Child& child) noexcept
        {
            parent.unlink_child(child);
        }
    };
}

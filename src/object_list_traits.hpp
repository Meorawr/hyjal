// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace meorawr::hyjal {
    template<typename T>
    struct object_list_traits {
        template<typename Owner>
        static void link_object(Owner& owner, T& object) noexcept
        {
            owner.link_object(object);
        }
    };
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "intrusive_list_node.hpp"

namespace meorawr::hyjal {
    template<typename Child>
    struct child_list_node_tag;

    template<typename Child>
    using child_list_node = intrusive_list_node<child_list_node_tag<Child>>;

    template<typename Child>
    using child_list_node_traits = intrusive_list_node_traits<child_list_node_tag<Child>>;
}

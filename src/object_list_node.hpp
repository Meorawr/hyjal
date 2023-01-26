// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "intrusive_list_node.hpp"

namespace meorawr::hyjal {
    template<typename T>
    struct object_list_node_tag;

    template<typename T>
    using object_list_node = intrusive_list_node<object_list_node_tag<T>>;

    template<typename T>
    using object_list_node_traits = intrusive_list_node_traits<object_list_node_tag<T>>;
}

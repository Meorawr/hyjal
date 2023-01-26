// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "child_list_node.hpp"

#include <boost/intrusive/derivation_value_traits.hpp>
#include <boost/intrusive/link_mode.hpp>

namespace meorawr::hyjal {
    template<typename Child>
    using child_list_value_traits = boost::intrusive::derivation_value_traits<Child, child_list_node_traits<Child>, boost::intrusive::safe_link>;
}

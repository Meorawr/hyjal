// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "object_list_node.hpp"

#include <boost/intrusive/derivation_value_traits.hpp>
#include <boost/intrusive/link_mode.hpp>

namespace meorawr::hyjal {
    template<typename T>
    using object_list_value_traits = boost::intrusive::derivation_value_traits<T, object_list_node_traits<T>, boost::intrusive::auto_unlink>;
}

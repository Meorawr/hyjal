// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "child_list_value_traits.hpp"

#include <boost/intrusive/list.hpp>

namespace meorawr::hyjal {
    template<typename Child>
    using child_list = boost::intrusive::list<Child, boost::intrusive::value_traits<child_list_value_traits<Child>>>;
}

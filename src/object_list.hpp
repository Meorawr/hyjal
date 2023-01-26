// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "object_list_value_traits.hpp"

#include <boost/intrusive/list.hpp>

namespace meorawr::hyjal {
    template<typename T>
    using object_list = boost::intrusive::list<T, boost::intrusive::value_traits<object_list_value_traits<T>>, boost::intrusive::constant_time_size<false>>;
}

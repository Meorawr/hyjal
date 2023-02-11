// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "state_deleter.hpp"

namespace meorawr::hyjal::lua {
    void state_deleter::operator()(state_t state) const noexcept
    {
        lua_close(state);
    }
}
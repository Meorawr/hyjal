// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "lua/state_view.hpp"

namespace meorawr::hyjal {
    void register_script_functions(lua::state_view L, class ui_service& ui);
}

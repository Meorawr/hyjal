// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "../src/lua/state.hpp"

namespace meorawr::hyjal::test {
    lua::unique_state make_lua_state();
    lua::unique_state make_lua_state_and_service();
}

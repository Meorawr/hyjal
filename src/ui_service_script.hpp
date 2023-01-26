// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

struct lua_State;

namespace meorawr::hyjal {
    void register_script_functions(lua_State* L, class ui_service& ui);
}

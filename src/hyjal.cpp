// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "hyjal.hpp"

#include <lua.hpp>

#include <exception>

extern "C" int luaopen_hyjal(struct lua_State* L) noexcept
try {
    return 0;
} catch (const std::exception& ex) {
    return luaL_error(L, "%s", ex.what());
}

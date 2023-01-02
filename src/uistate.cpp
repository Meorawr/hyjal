// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "uistate.hpp"

#include <lua.hpp>

namespace meorawr::hyjal {
    UiState::UiState(lua_State* L)
        : UiObject({}, nullptr)
        , _L(L)
    {
    }

    UiState::~UiState() noexcept = default;
}

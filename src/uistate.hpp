// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MEORAWR_HYJAL_UISTATE_HPP
#define MEORAWR_HYJAL_UISTATE_HPP

#include "uiobject.hpp"

struct lua_State;

namespace meorawr::hyjal {
    // TODO: Temporary inheritance of UiObject to form a rough object tree.
    class UiState : public UiObject {
    public:
        UiState(lua_State* L);
        UiState(const UiState&) = delete;
        UiState(UiState&&) = delete;

        UiState& operator=(const UiState&) = delete;
        UiState& operator=(UiState&&) = delete;

        ~UiState() noexcept override;

    private:
        lua_State* _L;
    };
}

#endif

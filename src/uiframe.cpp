// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "uiframe.hpp"

namespace meorawr::hyjal {
    UiFrame::UiFrame(std::string_view name, UiObject* parent)
        : UiObject(name, parent)
    {
    }

    UiFrame::~UiFrame() noexcept = default;
}

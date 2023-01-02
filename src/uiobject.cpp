// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "uiobject.hpp"

namespace meorawr::hyjal {
    UiObject::UiObject(std::string_view name)
        : _name(name)
    {
    }

    UiObject::~UiObject() noexcept = default;

    std::string_view UiObject::name() const noexcept
    {
        return _name;
    }
}

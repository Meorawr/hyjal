// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "font.hpp"

#include "ui_service.hpp"

namespace meorawr::hyjal {
    font::font(std::string_view name, ui_service& owner)
        : font(name, ui_type_info_v<font>, owner)
    {
    }

    font::font(std::string_view name, const ui_type_info& type, ui_service& owner)
        : object(name, type, owner)
        , object_list_hook<font>(owner)
    {
    }

    font::~font() noexcept = default;
}

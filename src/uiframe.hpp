// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MEORAWR_HYJAL_UIFRAME_HPP
#define MEORAWR_HYJAL_UIFRAME_HPP

#include "uiobject.hpp"

namespace meorawr::hyjal {
    class UiFrame : public UiObject {
    public:
        UiFrame(std::string_view name, UiFrame* parent);
        UiFrame(const UiFrame&) = delete;
        UiFrame(UiFrame&&) = delete;

        UiFrame& operator=(const UiFrame&) = delete;
        UiFrame& operator=(UiFrame&&) = delete;

        ~UiFrame() noexcept override;

        UiFrame* get_parent() const noexcept;

    private:
        UiFrame* _parent;
    };
}

#endif

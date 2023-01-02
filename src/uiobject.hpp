// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MEORAWR_HYJAL_UIOBJECT_HPP
#define MEORAWR_HYJAL_UIOBJECT_HPP

#include <string_view>

namespace meorawr::hyjal {
    class UiObject {
    public:
        UiObject(std::string_view name, UiObject* parent);
        UiObject(const UiObject&) = delete;
        UiObject(UiObject&&) = delete;

        UiObject& operator=(const UiObject&) = delete;
        UiObject& operator=(UiObject&&) = delete;

        virtual ~UiObject() noexcept;

        std::string_view name() const noexcept;
        UiObject* parent() const noexcept;

        void set_parent(UiObject* parent);

    private:
        std::string_view _name; // TODO: Central string table with Lua state.
        UiObject* _parent = nullptr;
        UiObject* _child_head = nullptr;
        UiObject* _child_tail = nullptr;
        UiObject* _sibling_prev = nullptr;
        UiObject* _sibling_next = nullptr;
    };
}

#endif

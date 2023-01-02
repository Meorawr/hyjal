// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "uiobject.hpp"

#include <cassert>

namespace meorawr::hyjal {

    UiObject::UiObject(std::string_view name, UiObject* parent)
        : _name(name)
    {
        set_parent(parent);
    }

    UiObject::~UiObject() noexcept
    {
        while (_child_head)
            delete _child_head; // TODO: Do we want raw new/delete for objects?

        set_parent(nullptr);
    }

    std::string_view UiObject::name() const noexcept
    {
        return _name;
    }

    UiObject* UiObject::parent() const noexcept
    {
        return _parent;
    }

    void UiObject::set_parent(UiObject* parent)
    {
        if (_parent == parent)
            return;

        // If we've got an existing parent we need to unlink ourselves from it
        // and our current adjacent sibling(s).

        if (_parent) {
            if (_parent->_child_head == this)
                _parent->_child_head = _sibling_next;

            if (_parent->_child_tail == this)
                _parent->_child_tail = _sibling_prev;

            if (_sibling_prev)
                _sibling_prev->_sibling_next = _sibling_next;

            if (_sibling_next)
                _sibling_next->_sibling_prev = _sibling_prev;

            _sibling_prev = nullptr;
            _sibling_next = nullptr;
        }

        _parent = parent;

        // TODO: Changes of parents should implicitly set some flags to mark
        //       other bits of state dirty (like visibility).

        // If we've got a new parent we now need to link ourselves into it
        // and with the last sibling in the list.

        if (_parent) {
            if (_parent->_child_tail) {
                _sibling_prev = _parent->_child_tail;
                _sibling_prev->_sibling_next = this;
                _parent->_child_tail = this;
            } else {
                _parent->_child_head = this;
                _parent->_child_tail = this;
            }
        }
    }
}

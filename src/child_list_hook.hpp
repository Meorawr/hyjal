// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "child_list_node.hpp"
#include "child_list_traits.hpp"
#include "child_list_value_traits.hpp"

namespace meorawr::hyjal {
    template<typename Child, typename Parent>
    class child_list_hook : public child_list_node<Child> {
    public:
        using parent_type = Parent;

        explicit child_list_hook(parent_type* parent) noexcept;
        child_list_hook(const child_list_hook&) = delete;
        child_list_hook(child_list_hook&&) = delete;

        child_list_hook& operator=(const child_list_hook&) = delete;
        child_list_hook& operator=(child_list_hook&&) = delete;

        ~child_list_hook() noexcept;

        parent_type* parent() const noexcept;

        void set_parent(parent_type* parent) noexcept;

    private:
        parent_type* parent_ = nullptr;
    };

    template<typename Child, typename Parent>
    child_list_hook<Child, Parent>::child_list_hook(parent_type* parent) noexcept
    {
        set_parent(parent);
    }

    template<typename Child, typename Parent>
    child_list_hook<Child, Parent>::~child_list_hook() noexcept
    {
        set_parent(nullptr);
    }

    template<typename Child, typename Parent>
    auto child_list_hook<Child, Parent>::parent() const noexcept -> parent_type*
    {
        return parent_;
    }

    template<typename Child, typename Parent>
    void child_list_hook<Child, Parent>::set_parent(parent_type* parent) noexcept
    {
        if (parent_)
            child_list_traits<Child>::unlink_child(*parent_, *child_list_value_traits<Child>::to_value_ptr(this));

        parent_ = parent;

        if (parent_)
            child_list_traits<Child>::link_child(*parent_, *child_list_value_traits<Child>::to_value_ptr(this));
    }
}

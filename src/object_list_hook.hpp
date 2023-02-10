// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "object_list_node.hpp"
#include "object_list_traits.hpp"
#include "object_list_value_traits.hpp"

#include <boost/intrusive/circular_list_algorithms.hpp>

namespace meorawr::hyjal {
    class ui_service;

    template<typename T>
    class object_list_hook : public object_list_node<T> {
    public:
        explicit object_list_hook(ui_service& ui) noexcept;
        object_list_hook(const object_list_hook&) = delete;
        object_list_hook(object_list_hook&&) = delete;

        object_list_hook& operator=(const object_list_hook&) = delete;
        object_list_hook& operator=(object_list_hook&&) = delete;

        ~object_list_hook() noexcept;

    private:
        using node_algorithms = boost::intrusive::circular_list_algorithms<object_list_node_traits<T>>;
    };

    template<typename T>
    object_list_hook<T>::object_list_hook(ui_service& ui) noexcept
    {
        object_list_traits<T>::link_object(ui, *object_list_value_traits<T>::to_value_ptr(this));
    }

    template<typename T>
    object_list_hook<T>::~object_list_hook() noexcept
    {
        if (!node_algorithms::inited(this)) {
            node_algorithms::unlink(this);
        }
    }
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "ui_type_info.hpp"

#include <string_view>

namespace meorawr::hyjal {
    class ui_service;

    class object_base {
    public:
        object_base() noexcept = default;
        object_base(const object_base&) = delete;
        object_base(object_base&&) = delete;

        object_base& operator=(const object_base&) = delete;
        object_base& operator=(object_base&&) = delete;

        virtual ~object_base() noexcept = default;

        ui_service& owner() const noexcept { return owner_impl(); }
        std::string_view name() const noexcept { return name_impl(); }
        const ui_type_info& type() const noexcept { return type_impl(); }

    private:
        virtual ui_service& owner_impl() const noexcept = 0;
        virtual std::string_view name_impl() const noexcept = 0;
        virtual const ui_type_info& type_impl() const noexcept = 0;
    };
}

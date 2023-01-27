// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "object_base.hpp"
#include "object_list_hook.hpp"
#include "ui_type_info.hpp"

#include <string_view>

namespace meorawr::hyjal {
    class object : public virtual object_base, public object_list_hook<object> {
    public:
        object(std::string_view name, ui_service& owner);
        object(const object&) = delete;
        object(object&&) = delete;

        object& operator=(const object&) = delete;
        object& operator=(object&&) = delete;

        ~object() noexcept override;

        ui_service& owner() const noexcept { return owner_; }
        std::string_view name() const noexcept { return name_; }
        const ui_type_info& type() const noexcept { return type_; }

    protected:
        object(std::string_view name, const ui_type_info& type, ui_service& owner);

    private:
        struct allocation_info {
            std::size_t size = 0;
            std::size_t align = 0;
        };

        ui_service& owner_impl() const noexcept override;
        std::string_view name_impl() const noexcept override;
        const ui_type_info& type_impl() const noexcept override;

        ui_service& owner_;
        std::string_view name_;
        const ui_type_info& type_;
    };

    template<>
    struct ui_type_traits<object> {
        static constexpr ui_type_id type_id = {"34635b6e-8500-4171-9a67-764c2cd42aa2"};
        static constexpr std::string_view type_name = "Object";
    };

    template<ui_type To, std::derived_from<object_base> From>
    requires std::derived_from<To, From>
    To* object_cast(From* from) noexcept
    {
        return const_cast<To*>(object_cast<To>(const_cast<const From*>(from)));
    }

    template<ui_type To, std::derived_from<object_base> From>
    requires std::derived_from<To, From>
    const To* object_cast(const From* from) noexcept
    {
        if constexpr (std::derived_from<From, To>) {
            return static_cast<const To*>(from);
        } else if (from->type().derives_from(ui_type_info_v<To>)) {
            return static_cast<const To*>(from);
        } else {
            return nullptr;
        }
    }

    template<ui_type To, std::derived_from<object_base> From>
    requires std::derived_from<To, From>
    To& object_cast(From& from)
    {
        return const_cast<To&>(object_cast<To>(const_cast<const From&>(from)));
    }

    template<ui_type To, std::derived_from<object_base> From>
    requires std::derived_from<To, From>
    const To& object_cast(const From& from)
    {
        // TODO: Better error reporting here rather than a segfault? :)
        return *object_cast<To>(from);
    }
}

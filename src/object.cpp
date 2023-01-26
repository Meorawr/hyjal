// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "object.hpp"

#include "ui_service.hpp"

namespace meorawr::hyjal {
    void* object::operator new(std::size_t size, ui_service& ui, allocation_info&& info)
    {
        return operator new(size, static_cast<std::align_val_t>(alignof(max_align_t)), ui, std::move(info));
    }

    void* object::operator new(std::size_t size, std::align_val_t align, ui_service& ui, allocation_info&& info)
    {
        info.size = size;
        info.align = static_cast<std::size_t>(align);
        return ui.object_memory_resource()->allocate(info.size, info.align);
    }

    void object::operator delete(void* ptr, ui_service& ui, allocation_info&& info) noexcept
    {
        ui.object_memory_resource()->deallocate(ptr, info.size, info.align);
    }

    object::object(std::string_view name, ui_service& owner)
        : object(name, ui_type_info_v<object>, owner)
    {
    }

    object::object(std::string_view name, const ui_type_info& type, ui_service& owner)
        : object_list_hook<object>(owner)
        , owner_(owner)
        , name_(name)
        , type_(type)
    {
    }

    object::~object() noexcept = default;

    ui_service& object::owner_impl() const noexcept
    {
        return owner_;
    }

    std::string_view object::name_impl() const noexcept
    {
        return name_;
    }

    const ui_type_info& object::type_impl() const noexcept
    {
        return type_;
    }
}

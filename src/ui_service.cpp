// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ui_service.hpp"

#include "animation.hpp"
#include "font.hpp"
#include "frame.hpp"
#include "object.hpp"

namespace meorawr::hyjal {
    ui_service::ui_service(lua_State* L, std::pmr::memory_resource* resource)
        : L(L)
        , object_memory_resource_(resource)
    {
    }

    ui_service::~ui_service() noexcept = default;

    lua_State* ui_service::lua_state() const noexcept
    {
        return L;
    }

    object_list_view<animation> ui_service::animations() const& noexcept
    {
        return const_cast<object_list<animation>&>(animations_);
    }

    object_list_view<object> ui_service::objects() const& noexcept
    {
        return const_cast<object_list<object>&>(objects_);
    }

    object_list_view<frame> ui_service::frames() const& noexcept
    {
        return const_cast<object_list<frame>&>(frames_);
    }

    object_list_view<font> ui_service::fonts() const& noexcept
    {
        return const_cast<object_list<font>&>(fonts_);
    }

    void* ui_service::allocate_object(const ui_type_info& type)
    {
        return object_memory_resource_.allocate(type.size_of(), type.align_of());
    }

    void ui_service::deallocate_object(void* ptr, const ui_type_info& type) noexcept
    {
        return object_memory_resource_.deallocate(ptr, type.size_of(), type.align_of());
    }

    void ui_service::link_object(animation& animation) noexcept
    {
        animations_.push_back(animation);
    }

    void ui_service::link_object(font& font) noexcept
    {
        fonts_.push_back(font);
    }

    void ui_service::link_object(frame& frame) noexcept
    {
        frames_.push_back(frame);
    }

    void ui_service::link_object(object& object) noexcept
    {
        objects_.push_back(object);
    }

    void delete_object(object* ptr) noexcept
    {
        if (ptr) {
            ui_service& ui = ptr->owner();
            const ui_type_info& type = ptr->type();

            ptr->~object();
            ui.deallocate_object(ptr, type);
        }
    }
}

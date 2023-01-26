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

    std::pmr::memory_resource* ui_service::object_memory_resource() noexcept
    {
        return &object_memory_resource_;
    }

    object_list_view<animation> ui_service::animations() const noexcept
    {
        return const_cast<object_list<animation>&>(animations_);
    }

    object_list_view<object> ui_service::objects() const noexcept
    {
        return const_cast<object_list<object>&>(objects_);
    }

    object_list_view<frame> ui_service::frames() const noexcept
    {
        return const_cast<object_list<frame>&>(frames_);
    }

    object_list_view<font> ui_service::fonts() const noexcept
    {
        return const_cast<object_list<font>&>(fonts_);
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
}

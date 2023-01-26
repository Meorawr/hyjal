// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "frame.hpp"

#include "region.hpp"
#include "ui_service.hpp"

namespace meorawr::hyjal {
    frame::frame(std::string_view name, ui_service& owner)
        : frame(name, ui_type_info_v<frame>, nullptr, owner)
    {
    }

    frame::frame(std::string_view name, frame& parent)
        : frame(name, ui_type_info_v<frame>, &parent, parent.owner())
    {
    }

    frame::frame(std::string_view name, const ui_type_info& type, ui_service& owner)
        : frame(name, type, nullptr, owner)
    {
    }

    frame::frame(std::string_view name, const ui_type_info& type, frame& parent)
        : frame(name, type, &parent, parent.owner())
    {
    }

    frame::frame(std::string_view name, const ui_type_info& type, frame* parent, ui_service& owner)
        : object(name, type, owner)
        , object_list_hook<frame>(owner)
        , child_list_hook(parent)
    {
    }

    frame::~frame() noexcept
    {
        regions_.clear_and_dispose(delete_object<region>);
        frames_.clear_and_dispose(delete_object<frame>);
    }

    child_list_view<frame> frame::frames() const noexcept
    {
        return const_cast<child_list<frame>&>(frames_);
    }

    child_list_view<region> frame::regions() const noexcept
    {
        return const_cast<child_list<region>&>(regions_);
    }

    frame* frame::parent() const noexcept
    {
        return child_list_hook::parent();
    }

    void frame::set_parent(frame* parent)
    {
        child_list_hook::set_parent(parent);
    }

    void frame::link_child(frame& frame) noexcept
    {
        frames_.push_back(frame);
    }

    void frame::link_child(region& region) noexcept
    {
        regions_.push_back(region);
    }

    void frame::unlink_child(frame& frame) noexcept
    {
        frames_.erase(frames_.iterator_to(frame));
    }

    void frame::unlink_child(region& region) noexcept
    {
        regions_.erase(regions_.iterator_to(region));
    }
}

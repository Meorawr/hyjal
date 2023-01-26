// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "animatable_object.hpp"
#include "child_list.hpp"
#include "child_list_hook.hpp"
#include "child_list_traits.hpp"
#include "child_list_view.hpp"
#include "object.hpp"
#include "object_list_hook.hpp"

namespace meorawr::hyjal {
    class region;

    class frame : public object, public animatable_object, public object_list_hook<frame>, public child_list_hook<frame, frame> {
    public:
        frame(std::string_view name, ui_service& owner);
        frame(std::string_view name, frame& parent);
        frame(const object&) = delete;
        frame(object&&) = delete;

        frame& operator=(const frame&) = delete;
        frame& operator=(frame&&) = delete;

        ~frame() noexcept override;

        child_list_view<frame> frames() const noexcept;
        child_list_view<region> regions() const noexcept;
        frame* parent() const noexcept;

        void set_parent(frame* parent);

    protected:
        frame(std::string_view name, const ui_type_info& type, ui_service& owner);
        frame(std::string_view name, const ui_type_info& type, frame& parent);

    private:
        frame(std::string_view name, const ui_type_info& type, frame* parent, ui_service& owner);

        void link_child(frame& frame) noexcept;
        void link_child(region& region) noexcept;
        void unlink_child(frame& frame) noexcept;
        void unlink_child(region& region) noexcept;

        child_list<frame> frames_;
        child_list<region> regions_;

        friend child_list_traits<frame>;
        friend child_list_traits<region>;
    };

    template<>
    struct ui_type_traits<frame> {
        using derived_type = object;

        static constexpr ui_type_id type_id = {"5ec32673-130a-461f-90a6-ac7c5dcb722c"};
        static constexpr std::string_view type_name = "Frame";
    };
}

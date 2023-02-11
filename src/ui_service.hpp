// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "lua/state_view.hpp"
#include "object_list.hpp"
#include "object_list_traits.hpp"
#include "object_list_view.hpp"
#include "ui_type_info.hpp"
#include "ui_type_traits.hpp"

#include <memory_resource>

namespace meorawr::hyjal {
    class animation;
    class font;
    class frame;
    class object;

    class ui_service {
    public:
        explicit ui_service(lua::state_view L);
        ui_service(lua::state_view L, std::pmr::memory_resource* resource);
        ui_service(const ui_service&) = delete;
        ui_service(ui_service&&) = delete;

        ui_service& operator=(const ui_service&) = delete;
        ui_service& operator=(ui_service&&) = delete;

        ~ui_service() noexcept;

        lua::state_view lua_state() const noexcept;

        object_list_view<animation> animations() const& noexcept;
        object_list_view<object> objects() const& noexcept;
        object_list_view<frame> frames() const& noexcept;
        object_list_view<font> fonts() const& noexcept;

        void* allocate_object(const ui_type_info& type);
        void deallocate_object(void* ptr, const ui_type_info& type) noexcept;

    private:
        void link_object(animation& animation) noexcept;
        void link_object(font& font) noexcept;
        void link_object(frame& frame) noexcept;
        void link_object(object& object) noexcept;

        lua::state_view state_;
        std::pmr::unsynchronized_pool_resource object_memory_resource_;
        object_list<object> objects_;
        object_list<frame> frames_;
        object_list<font> fonts_;
        object_list<animation> animations_;

        friend object_list_traits<animation>;
        friend object_list_traits<font>;
        friend object_list_traits<frame>;
        friend object_list_traits<object>;
    };

    template<ui_type T>
    T* allocate_object(ui_service& ui)
    {
        return static_cast<T*>(ui.allocate_object(ui_type_info_v<T>));
    }

    template<ui_type T>
    void deallocate_object(ui_service& ui, T* ptr) noexcept
    {
        ui.deallocate_object(ptr, ui_type_info_v<T>);
    }

    // TODO: Constructor arguments are a bit awkward at the minute resulting
    //       in two new_object overloads being provided here.
    //
    //       The main issue is that some object types allow construction
    //       with either an explicit ui_service& owner parameter -or- an
    //       inferred one from a parent. In some cases, the owner-based
    //       overload also doesn't exist.
    //
    //       We could remove the parent-based constructors, however that
    //       impacts some (admittedly script-facing) invariants whereby
    //       it isn't possible to - for example - reassign texture regions
    //       to a nil parent. As such the member functions of those types
    //       reflect that by taking a reference to the parent rather than
    //       a potentially-null pointer.
    //
    //       Alternatively we allow specifying both the parent -and- the
    //       owner for those constructors, however this is leads to another
    //       awkward situation whereby it would become possible to in effect
    //       attach an object created through ui_service as the child of
    //       an object created through a different ui_service.
    //
    //       Technically speaking nothing is preventing a set_parent call
    //       from also doing something similar though, so maybe just a runtime
    //       check that 'parent.owner() == owner()' would be enough?

    template<std::derived_from<object> T, typename... Args>
    requires std::constructible_from<T, Args...>
    T* new_object(ui_service& ui, Args&&... args)
    {
        const auto deleter = [&](T* ptr) noexcept { deallocate_object(ui, ptr); };

        using deleter_type = decltype(deleter);
        using pointer_type = std::unique_ptr<T, deleter_type>;

        pointer_type ptr(allocate_object<T>(ui), deleter);
        new (ptr.get()) T(std::forward<Args>(args)...);
        return ptr.release();
    }

    template<std::derived_from<object> T, typename... Args>
    requires std::constructible_from<T, Args..., ui_service&>
    T* new_object(ui_service& ui, Args&&... args)
    {
        const auto deleter = [&](T* ptr) noexcept { deallocate_object(ui, ptr); };

        using deleter_type = decltype(deleter);
        using pointer_type = std::unique_ptr<T, deleter_type>;

        pointer_type ptr(allocate_object<T>(ui), deleter);
        new (ptr.get()) T(std::forward<Args>(args)..., ui);
        return ptr.release();
    }

    void delete_object(object* ptr) noexcept;
}

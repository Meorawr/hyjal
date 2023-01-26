// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "object_list.hpp"
#include "object_list_traits.hpp"
#include "object_list_view.hpp"

#include <memory_resource>

struct lua_State;

namespace meorawr::hyjal {
    class animation;
    class font;
    class frame;
    class object;

    class ui_service {
    public:
        ui_service(lua_State* L, std::pmr::memory_resource* resource);
        ui_service(const ui_service&) = delete;
        ui_service(ui_service&&) = delete;

        ui_service& operator=(const ui_service&) = delete;
        ui_service& operator=(ui_service&&) = delete;

        ~ui_service() noexcept;

        lua_State* lua_state() const noexcept;
        std::pmr::memory_resource* object_memory_resource() noexcept;

        object_list_view<animation> animations() const noexcept;
        object_list_view<object> objects() const noexcept;
        object_list_view<frame> frames() const noexcept;
        object_list_view<font> fonts() const noexcept;

    private:
        void link_object(animation& animation) noexcept;
        void link_object(font& font) noexcept;
        void link_object(frame& frame) noexcept;
        void link_object(object& object) noexcept;

        lua_State* L;
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

    // TODO: Currently the lifetime management of object isn't the nicest and
    //       we've got a few (somewhat conflicting) assumptions in places.
    //
    //       These free functions below were the initial point through which
    //       objects were intended to be created. Later, operator new/delete
    //       support with placement-style syntax was added to the object class
    //       to allow a more succinct 'new (ui) object_type(...)' expression
    //       to be used.
    //
    //       Elsewhere, various child_lists assume objects are allocated
    //       on the heap through the object memory resource in the UI state.
    //
    //       Both the placement-style new and these functions below work fine
    //       with that heap allocation assumption for child_list management,
    //       however we haven't explicitly deleted other overloads for new
    //       that could allow putting objects _outside_ of the object pool
    //       and into the general heap -or- directly creating them on the
    //       stack.
    //
    //       As such a few decisions need to be made;
    //
    //         1. With the placement-style syntax should these free functions
    //            continue to exist?
    //
    //         2. Do we disallow general heap allocation of objects? Note that
    //            we can't solve the stack allocation problem however.
    //
    //         3. Should objects gain support for a custom deallocator,
    //            to be called through a destructing-delete overload of
    //            operator delete?
    //
    //         4. Should we instead move objects to a PIMPL-style system
    //            whereby the 'object' types hold a non-owning pointer to the
    //            actual data, and use this to move the allocation away from
    //            the caller?
    //
    //       An initial design of the library did use PIMPL however it was
    //       scrapped due to the maintenance overhead - having to effectively
    //       duplicate parts of the public interface to the private one is
    //       awkward and attaching value-like semantics to the public types
    //       doesn't make much sense given objects aren't really "values".
    //
    //       Disallowing general heap allocations might solve a potential
    //       footgun (what if we accidentally forget the '(ui)'?), but there's
    //       probably some value in not requiring that *all* objects live in
    //       the pool.
    //
    //       Allowing a custom deallocator function to be attached to objects
    //       is probably the best route; worst case it'll just be the size of
    //       two pointers but objects aren't exactly going to be lightweight
    //       allocations numbering in the hundreds of thousands anyway.

    template<std::derived_from<object> T, typename... Args>
    requires std::constructible_from<T, Args...>
    T* new_object(ui_service& ui, Args&&... args)
    {
        std::pmr::polymorphic_allocator allocator(ui.object_memory_resource());
        return allocator.new_object<T>(std::forward<Args>(args)...);
    }

    template<std::derived_from<object> T, typename... Args>
    requires std::constructible_from<T, Args..., ui_service&>
    T* new_object(ui_service& ui, Args&&... args)
    {
        std::pmr::polymorphic_allocator allocator(ui.object_memory_resource());
        return allocator.new_object<T>(std::forward<Args>(args)..., ui);
    }

    template<std::derived_from<object> T>
    void delete_object(T* object)
    {
        std::pmr::polymorphic_allocator allocator(object->owner().object_memory_resource());
        allocator.delete_object<T>(object);
    }
}

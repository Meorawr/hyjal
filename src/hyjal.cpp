// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "hyjal.hpp"

#include "ui_service.hpp"
#include "ui_service_script.hpp"

#include <lua.hpp>

#include <exception>

using meorawr::hyjal::ui_service;

namespace {
    std::pmr::memory_resource* default_resource_ = nullptr;

    int destroy_ui_service(lua_State* L)
    {
        static_cast<ui_service*>(lua_touserdata(L, 1))->~ui_service();
        return 0;
    }
}

namespace meorawr::hyjal {
    std::pmr::memory_resource* get_default_memory_resource() noexcept
    {
        std::pmr::memory_resource* resource = default_resource_;

        if (!resource) {
            resource = std::pmr::get_default_resource();
        }

        return resource;
    }

    void set_default_memory_resource(std::pmr::memory_resource* resource) noexcept
    {
        default_resource_ = resource;
    }

    int luaopen_hyjal(lua_State* L) noexcept
    try {
        void* userdata = lua_newuserdata(L, sizeof(ui_service));
        lua_createtable(L, 0, 2);
        lua_pushcclosure(L, &destroy_ui_service, 0);
        lua_setfield(L, -2, "__gc");
        lua_pushboolean(L, false);
        lua_setfield(L, -2, "__metatable");
        ui_service* ui = new (userdata) ui_service(L);
        lua_setmetatable(L, -2);

        lua_pushvalue(L, LUA_GLOBALSINDEX);
        meorawr::hyjal::register_script_functions(L, *ui);
        lua_pop(L, 1);
        return 1;
    } catch (const std::exception& ex) {
        return luaL_error(L, "%s", ex.what());
    }
}

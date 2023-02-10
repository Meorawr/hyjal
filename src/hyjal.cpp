// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "hyjal.hpp"

#include "ui_service.hpp"
#include "ui_service_script.hpp"

#include <lua.hpp>

#include <exception>

using meorawr::hyjal::ui_service;

static int destroy_ui_service(lua_State* L) noexcept
{
    static_cast<ui_service*>(lua_touserdata(L, 1))->~ui_service();
    return 0;
}

extern "C" int luaopen_hyjal(struct lua_State* L) noexcept
try {
    void* userdata = lua_newuserdata(L, sizeof(ui_service));
    lua_createtable(L, 0, 2);
    lua_pushcclosure(L, &destroy_ui_service, 0);
    lua_setfield(L, -2, "__gc");
    lua_pushboolean(L, false);
    lua_setfield(L, -2, "__metatable");
    ui_service* ui = new (userdata) ui_service(L, std::pmr::get_default_resource());
    lua_setmetatable(L, -2);

    lua_pushvalue(L, LUA_GLOBALSINDEX);
    meorawr::hyjal::register_script_functions(L, *ui);
    lua_pop(L, 1);
    return 1;
} catch (const std::exception& ex) {
    return luaL_error(L, "%s", ex.what());
}

#include <fmt/core.h>

#include <doctest/doctest.h>

TEST_CASE("hyjal test")
{
    fmt::print("Boo!\n");
}

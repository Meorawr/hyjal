// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "hyjal.hpp"

#include "uiframe.hpp"
#include "uiobject.hpp"
#include "uistate.hpp"

#include <lua.hpp>

#include <exception>
#include <string_view>

namespace {
    template<typename T>
    int destroy_object(lua_State* L) noexcept
    {
        static_cast<T*>(lua_touserdata(L, -1))->~T();
        return 0;
    }

    int create_font(lua_State* L)
    {
        lua_createtable(L, 0, 0); // TODO: Object creation.
        return 1;
    }

    int temp_getparent(lua_State* L)
    {
        using meorawr::hyjal::UiObject;
        lua_rawgeti(L, 1, 0);
        UiObject* o = static_cast<UiObject*>(lua_touserdata(L, -1));

        if (UiObject* p = o->parent())
            lua_pushlightuserdata(L, p);
        else
            lua_pushnil(L);

        return 1;
    }

    int create_frame(lua_State* L)
    {
        using namespace std::literals;
        using meorawr::hyjal::UiFrame;
        using meorawr::hyjal::UiObject;

        lua_createtable(L, 0, 1);

        UiObject* parent = nullptr;

        // TODO: Converting a {[0] = ud} table will be quite common so should
        //       get lifted into a utility.
        if (lua_istable(L, 1)) {
            lua_rawgeti(L, 1, 0);
            parent = static_cast<UiObject*>(lua_touserdata(L, -1));
            lua_pop(L, 1);
        }

        if (!parent)
            parent = static_cast<UiObject*>(lua_touserdata(L, lua_upvalueindex(1)));

        lua_pushlightuserdata(L, new UiFrame("test"sv, parent));
        lua_rawseti(L, -2, 0);

        lua_createtable(L, 0, 1);
        lua_pushcclosure(L, temp_getparent, 0);
        lua_setfield(L, -2, "GetParent");

        lua_createtable(L, 0, 1);
        lua_insert(L, -2);
        lua_setfield(L, -2, "__index");

        lua_setmetatable(L, -2);

        lua_pushvalue(L, -1);
        luaL_ref(L, LUA_REGISTRYINDEX);

        return 1;
    }

    int does_template_exist(lua_State* L)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    int enumerate_frames(lua_State* L)
    {
        lua_pushnil(L);
        return 1;
    }

    int get_click_frame(lua_State* L)
    {
        lua_pushnil(L);
        return 1;
    }

    int get_current_keyboard_focus(lua_State* L)
    {
        lua_pushnil(L);
        return 1;
    }

    int get_default_scale(lua_State* L)
    {
        lua_pushnumber(L, 1.0);
        return 1;
    }

    int get_font_info(lua_State* L)
    {
        lua_pushnil(L);
        return 1;
    }

    int get_fonts(lua_State* L)
    {
        lua_createtable(L, 0, 0);
        return 1;
    }

    int get_frames_registered_for_event(lua_State* L)
    {
        lua_unused(L);
        return 0;
    }

    int get_mouse_button_name(lua_State* L)
    {
        lua_pushliteral(L, "LeftButton");
        return 1;
    }

    int get_mouse_click_focus(lua_State* L)
    {
        lua_pushnil(L);
        return 1;
    }

    int get_mouse_focus(lua_State* L)
    {
        lua_pushnil(L);
        return 1;
    }

    int get_mouse_motion_focus(lua_State* L)
    {
        lua_pushnil(L);
        return 1;
    }

    int get_num_frames(lua_State* L)
    {
        lua_pushinteger(L, 0);
        return 1;
    }

    const luaL_Reg GLOBAL_SCRIPT_FUNCTIONS[] = {
        {.name = "CreateFont", .func = create_font},
        {.name = "CreateFrame", .func = create_frame},
        {.name = "DoesTemplateExist", .func = does_template_exist},
        {.name = "EnumerateFrames", .func = enumerate_frames},
        {.name = "GetClickFrame", .func = get_click_frame},
        {.name = "GetCurrentKeyboardFocus", .func = get_current_keyboard_focus},
        {.name = "GetDefaultScale", .func = get_default_scale},
        {.name = "GetFontInfo", .func = get_font_info},
        {.name = "GetFonts", .func = get_fonts},
        {.name = "GetFramesRegisteredForEvent", .func = get_frames_registered_for_event},
        {.name = "GetMouseButtonName", .func = get_mouse_button_name},
        {.name = "GetMouseClickFocus", .func = get_mouse_click_focus},
        {.name = "GetMouseFocus", .func = get_mouse_focus},
        {.name = "GetMouseMotionFocus", .func = get_mouse_motion_focus},
        {.name = "GetNumFrames", .func = get_num_frames},
        {.name = nullptr, .func = nullptr},
    };
}

extern "C" {
    HYJAL_API int luaopen_hyjal(struct lua_State* L) noexcept
    try {
        using meorawr::hyjal::UiState;

        lua_createtable(L, 0, 1);
        lua_pushcclosure(L, destroy_object<UiState>, 0);
        lua_setfield(L, -2, "__gc");

        new (lua_newuserdata(L, sizeof(UiState))) UiState(L);
        lua_insert(L, -2);
        lua_setmetatable(L, -2);

        lua_pushvalue(L, LUA_GLOBALSINDEX);
        lua_pushvalue(L, -2);
        luaL_setfuncs(L, GLOBAL_SCRIPT_FUNCTIONS, 1);

        return 1;
    } catch (const std::exception& ex) {
        return luaL_error(L, "%s", ex.what());
    }
}

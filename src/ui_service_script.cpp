// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ui_service_script.hpp"

#include "font.hpp"
#include "frame.hpp"
#include "object.hpp"
#include "script_util.hpp"
#include "ui_service.hpp"

#include <lua.hpp>

namespace meorawr::hyjal {
    static int create_font(lua_State* L, ui_service& ui)
    {
        std::string_view name = lua_checkstringview(L, 1);
        lua_pushscriptobject(L, *new (ui) font(name, ui));
        return 1;
    }

    static int create_frame(lua_State* L, ui_service& ui)
    {
        std::string_view type = lua_checkstringview(L, 1);
        std::string_view name = lua_optstringview(L, 2);
        frame* parent = lua_optscriptobject<frame>(L, 3);
        std::string_view inherits = lua_optstringview(L, 4);
        int id = luaL_optint(L, 5, 0);

        frame* f;

        if (parent) {
            f = new (ui) frame(name, *parent);
        } else {
            f = new (ui) frame(name, ui);
        }

        lua_pushscriptobject(L, *f);
        return 1;
    }

    static int does_template_exist(lua_State* L, ui_service& ui)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    static int enumerate_frames(lua_State* L, ui_service& ui)
    {
        using view_type = object_list_view<frame>;
        using iterator_type = view_type::iterator;

        view_type frames = ui.frames();
        iterator_type it;
        iterator_type end = frames.end();

        if (frame* obj = lua_optscriptobject<frame>(L, 1); obj) {
            it = frames.iterator_to(*obj);
        } else {
            it = frames.begin();
        }

        if (++it != end) {
            lua_pushscriptobject(L, *it);
        } else {
            lua_pushnil(L);
        }

        return 1;
    }

    static int get_click_frame(lua_State* L, ui_service& ui)
    {
        lua_pushnil(L);
        return 1;
    }

    static int get_current_keyboard_focus(lua_State* L, ui_service& ui)
    {
        lua_pushnil(L);
        return 1;
    }

    static int get_default_scale(lua_State* L, ui_service& ui)
    {
        lua_pushnumber(L, 1.0f);
        return 1;
    }

    static int get_font_info(lua_State* L, ui_service& ui)
    {
        lua_pushnil(L);
        return 1;
    }

    static int get_fonts(lua_State* L, ui_service& ui)
    {
        lua_createtable(L, 16, 0);

        static int i = 0;

        for (const font& font : ui.fonts()) {
            lua_pushstringview(L, font.name());
            lua_rawseti(L, -2, ++i);
        }

        return 1;
    }

    static int get_frames_registered_for_event(lua_State* L, ui_service& ui)
    {
        return 0;
    }

    static int get_mouse_button_name(lua_State* L, ui_service& ui)
    {
        lua_pushnil(L);
        return 1;
    }

    static int get_mouse_click_focus(lua_State* L, ui_service& ui)
    {
        lua_pushnil(L);
        return 1;
    }

    static int get_mouse_focus(lua_State* L, ui_service& ui)
    {
        lua_pushnil(L);
        return 1;
    }

    static int get_mouse_motion_focus(lua_State* L, ui_service& ui)
    {
        lua_pushnil(L);
        return 1;
    }

    static int get_num_frames(lua_State* L, ui_service& ui)
    {
        lua_pushinteger(L, ui.frames().size());
        return 1;
    }

    template<int (*Fn)(lua_State*, ui_service&)>
    static int invoke_script_function(lua_State* L) noexcept
    try {
        return Fn(L, *static_cast<ui_service*>(lua_touserdata(L, lua_upvalueindex(1))));
    } catch (const std::exception& ex) {
        return luaL_error(L, "%s", ex.what());
    }

    void register_script_functions(lua_State* L, class ui_service& ui)
    {
        static luaL_Reg script_functions[] = {
            {.name = "CreateFont", .func = invoke_script_function<create_font>},
            {.name = "CreateFrame", .func = invoke_script_function<create_frame>},
            {.name = "DoesTemplateExist", .func = invoke_script_function<does_template_exist>},
            {.name = "EnumerateFrames", .func = invoke_script_function<enumerate_frames>},
            {.name = "GetClickFrame", .func = invoke_script_function<get_click_frame>},
            {.name = "GetCurrentKeyboardFocus", .func = invoke_script_function<get_current_keyboard_focus>},
            {.name = "GetDefaultScale", .func = invoke_script_function<get_default_scale>},
            {.name = "GetFontInfo", .func = invoke_script_function<get_font_info>},
            {.name = "GetFonts", .func = invoke_script_function<get_fonts>},
            {.name = "GetFramesRegisteredForEvent", .func = invoke_script_function<get_frames_registered_for_event>},
            {.name = "GetMouseButtonName", .func = invoke_script_function<get_mouse_button_name>},
            {.name = "GetMouseClickFocus", .func = invoke_script_function<get_mouse_click_focus>},
            {.name = "GetMouseFocus", .func = invoke_script_function<get_mouse_focus>},
            {.name = "GetMouseMotionFocus", .func = invoke_script_function<get_mouse_motion_focus>},
            {.name = "GetNumFrames", .func = invoke_script_function<get_num_frames>},
            {.name = nullptr, .func = nullptr},
        };

        lua_pushlightuserdata(L, std::addressof(ui));
        luaL_setfuncs(L, script_functions, 1);
    }
}

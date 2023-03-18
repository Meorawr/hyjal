// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "value_reference.hpp"
#include "value_type.hpp"

namespace meorawr::hyjal::detail::value_cast {
    template<typename T>
    struct cpo_t;
}

namespace meorawr::hyjal::lua::inline cpo {
    template<typename T>
    inline constexpr auto value_cast = detail::value_cast::cpo_t<T>{};
}

namespace meorawr::hyjal::detail::value_cast {
    template<>
    struct cpo_t<lua::nil_t> {
        auto operator()(const lua::value_reference&) const { return lua::nil; }
    };

    template<>
    struct cpo_t<bool> {
        auto operator()(const lua::value_reference& ref) const { return lua_toboolean(ref.state(), ref.index()); }
    };

    template<>
    struct cpo_t<int> {
        auto operator()(const lua::value_reference& ref) const { return lua_toint(ref.state(), ref.index()); }
    };

    template<>
    struct cpo_t<long> {
        auto operator()(const lua::value_reference& ref) const { return lua_tolong(ref.state(), ref.index()); }
    };

    template<>
    struct cpo_t<lua::integer_t> {
        auto operator()(const lua::value_reference& ref) const { return lua_tointeger(ref.state(), ref.index()); }
    };

    template<>
    struct cpo_t<lua::number_t> {
        auto operator()(const lua::value_reference& ref) const { return lua_tonumber(ref.state(), ref.index()); }
    };

    template<>
    struct cpo_t<std::string_view> {
        auto operator()(const lua::value_reference& ref) const
        {
            std::size_t size;
            const char* data = lua_tolstring(ref.state(), ref.index(), &size);
            return std::string_view(data, size);
        }
    };

    template<typename T>
    auto todo(const lua::value_reference& ref);

    template<typename T>
    concept has_adl = requires(const lua::value_reference& ref) { { todo<T>(ref) }; };

    template<typename T>
    struct cpo_t {
        auto operator()(const lua::value_reference& ref) const requires std::constructible_from<T, const lua::value_reference&> { return T(ref); }
        auto operator()(const lua::value_reference& ref) const requires has_adl<T> { return todo<T>(ref); }
    };
}

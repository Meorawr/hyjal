// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "ui_type_id.hpp"

#include <concepts>

#include <boost/mp11/mpl_list.hpp>

namespace meorawr::hyjal {
    template<typename T>
    struct ui_type_traits;

    /**
     * ui_type<T>
     *
     * Concept that passes if a given type T provides a valid specialization
     * of ui_type_traits<T>.
     */

    // clang-format off

    template<typename T>
    concept ui_type = requires {
        { ui_type_traits<T>::type_id } -> std::convertible_to<ui_type_id>;
        { ui_type_traits<T>::type_name } -> std::convertible_to<std::string_view>;    
    };

    // clang-format on

    /**
     * ui_base_type_t<T>
     *
     * Resolves to the 'base_type' typedef present in T's object traits.
     */

    template<ui_type T>
    requires(requires { typename ui_type_traits<T>::base_type; })
    struct ui_base_type {
        using type = typename ui_type_traits<T>::base_type;
    };

    template<ui_type T>
    using ui_base_type_t = typename ui_base_type<T>::type;

    /**
     * ui_derived_types_t<T>
     *
     * Resolves to a boost::mp11::mp_list containing the full chain of
     * derived-from types as would obtained through recursively querying
     * ui_base_type<T>.
     */

    template<ui_type... Ts>
    struct ui_derived_types;

    template<ui_type T, ui_type... Ts>
    requires(requires { typename ui_base_type<T>::type; })
    struct ui_derived_types<T, Ts...> {
        using type = ui_derived_types<typename ui_base_type<T>::type, Ts..., T>::type;
    };

    template<ui_type T, ui_type... Ts>
    requires(!requires { typename ui_base_type<T>::type; })
    struct ui_derived_types<T, Ts...> {
        using type = boost::mp11::mp_list<Ts..., T>;
    };

    template<ui_type T>
    using ui_derived_types_t = typename ui_derived_types<T>::type;
}

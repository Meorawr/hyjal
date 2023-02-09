// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "detail/type_list.hpp"
#include "ui_type_id.hpp"
#include "ui_type_traits.hpp"

namespace meorawr::hyjal {
    class ui_type_info {
    public:
        template<ui_type T>
        constexpr ui_type_info(std::type_identity<T>) noexcept
            : id_(ui_type_traits<T>::type_id)
            , name_(ui_type_traits<T>::type_name)
            , align_(alignof(T))
            , size_(sizeof(T))
            , derives_from_(generate_derivation_predicate(ui_derived_types_t<T>{}))
        {
        }

        constexpr ui_type_id id() const noexcept { return id_; }
        constexpr std::string_view name() const noexcept { return name_; }
        constexpr std::size_t align_of() const noexcept { return align_; }
        constexpr std::size_t size_of() const noexcept { return size_; }

        constexpr bool derives_from(const ui_type_info& other) const noexcept { return id_ == other.id_ || derives_from_(other.id_); }
        constexpr bool same_as(const ui_type_info& other) const noexcept { return id_ == other.id_; }

    private:
        using derivation_predicate = bool (*)(ui_type_id) noexcept;

        template<typename... Bases>
        static constexpr derivation_predicate generate_derivation_predicate(detail::type_list<Bases...>) noexcept;

        ui_type_id id_;
        std::string_view name_;
        std::size_t align_;
        std::size_t size_;
        derivation_predicate derives_from_;
    };

    template<ui_type T>
    inline constexpr const ui_type_info& ui_type_info_v = {std::type_identity<T>{}};

    template<typename... Bases>
    static constexpr auto ui_type_info::generate_derivation_predicate(detail::type_list<Bases...>) noexcept -> derivation_predicate
    {
        return [](ui_type_id other) noexcept -> bool {
            return ((ui_type_traits<Bases>::type_id == other) || ...);
        };
    }
}

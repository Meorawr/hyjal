// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>

namespace meorawr::hyjal::detail {
    // Note: LWG-3545 makes std::pointer_traits SFINAE-friendly if
    //       instantiated from a non-pointer type. This is implemented in
    //       recent versions of libstdc++ and libc++, however for MSVC's STL
    //       this isn't the case until VS 2022 17.6 Preview 1. As such this
    //       concept can - for now - only be used in contexts where failure
    //       to meet its requirement is entirely erroneous.

    template<typename P>
    concept pointer_type = requires { typename std::pointer_traits<P>::element_type; };

    template<typename P, typename To>
    concept pointer_to = requires(P p) { { std::to_address(p) } noexcept -> std::same_as<To*>; };
}

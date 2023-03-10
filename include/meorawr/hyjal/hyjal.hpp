// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MEORAWR_HYJAL_HPP
#define MEORAWR_HYJAL_HPP

#if defined(_MSC_VER)
#define HYJAL_DECL_EXPORT __declspec(dllexport)
#define HYJAL_DECL_IMPORT __declspec(dllimport)
#define HYJAL_DECL_HIDDEN
#elif defined(__GNUC__)
#define HYJAL_DECL_EXPORT __attribute__((visibility("default")))
#define HYJAL_DECL_IMPORT __attribute__((visibility("default")))
#define HYJAL_DECL_HIDDEN __attribute__((visibility("hidden")))
#else
#warning Unrecognized compiler type; symbol visibility macros may be incorrect.
#define HYJAL_DECL_EXPORT
#define HYJAL_DECL_IMPORT
#define HYJAL_DECL_HIDDEN
#endif

#if defined(HYJAL_BUILD_SHARED)
#define HYJAL_API HYJAL_DECL_EXPORT
#elif defined(HYJAL_USE_SHARED)
#define HYJAL_API HYJAL_DECL_IMPORT
#else
#define HYJAL_API
#endif

#define HYJAL_C_API extern "C" HYJAL_API

#include <memory_resource>

struct lua_State;

namespace meorawr::hyjal {
    HYJAL_API std::pmr::memory_resource* get_default_memory_resource() noexcept;
    HYJAL_API void set_default_memory_resource(std::pmr::memory_resource* resource) noexcept;

    HYJAL_C_API int luaopen_hyjal(lua_State* L) noexcept;
}

#endif

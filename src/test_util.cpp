// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test_util.hpp"

#include <meorawr/hyjal/hyjal.hpp>

namespace {
    void* lua_realloc(void* ud, void* old_ptr, std::size_t old_bytes, std::size_t new_bytes) noexcept
    try {
        auto* resource = static_cast<std::pmr::memory_resource*>(ud);
        void* new_ptr = nullptr;

        if (new_bytes) {
            new_ptr = resource->allocate(new_bytes);
        }

        if (new_bytes && old_bytes) {
            std::memcpy(new_ptr, old_ptr, std::min(old_bytes, new_bytes));
        }

        if (old_bytes) {
            resource->deallocate(old_ptr, old_bytes);
        }

        return new_ptr;
    } catch (...) {
        return nullptr;
    }
}

namespace meorawr::hyjal::test {
    lua::unique_state make_lua_state()
    {
        return lua::unique_state(lua_newstate(&lua_realloc, get_default_memory_resource()));
    }

    lua::unique_state make_lua_state_and_service()
    {
        lua::unique_state state = make_lua_state();
        luaopen_hyjal(state);
        return state;
    }
}

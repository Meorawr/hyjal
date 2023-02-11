// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "debug_memory_resource.hpp"

namespace meorawr::hyjal {
    debug_memory_resource::debug_memory_resource() noexcept
        : resource_(std::pmr::get_default_resource())
    {
    }

    debug_memory_resource::debug_memory_resource(std::pmr::memory_resource& resource) noexcept
        : resource_(&resource)
    {
    }

    std::size_t debug_memory_resource::allocated_bytes() const noexcept
    {
        return allocated_bytes_;
    }

    std::size_t debug_memory_resource::freed_bytes() const noexcept
    {
        return freed_bytes_;
    }

    void* debug_memory_resource::do_allocate(std::size_t bytes, std::size_t alignment)
    {
        void* ptr = resource_->allocate(bytes, alignment);
        allocated_bytes_ += bytes;
        return ptr;
    }

    void debug_memory_resource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment)
    {
        resource_->deallocate(ptr, bytes, alignment);
        freed_bytes_ += bytes;
    }

    bool debug_memory_resource::do_is_equal(const std::pmr::memory_resource& other) const noexcept
    {
        return this == &other;
    }
}

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <meorawr/hyjal/hyjal.hpp>

#include <doctest/doctest.h>
#include <fmt/core.h>
#include <lua.hpp>

#include <memory_resource>

class debug_memory_resource : public std::pmr::memory_resource {
public:
    explicit debug_memory_resource(std::pmr::memory_resource& resource) noexcept;

    std::size_t allocated_bytes() const noexcept { return allocated_bytes_; }
    std::size_t freed_bytes() const noexcept { return freed_bytes_; }
    std::size_t used_bytes() const noexcept { return allocated_bytes_ - freed_bytes_; }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

    std::pmr::memory_resource* resource_;
    std::size_t allocated_bytes_ = {};
    std::size_t freed_bytes_ = {};
};

debug_memory_resource::debug_memory_resource(std::pmr::memory_resource& resource) noexcept
    : resource_(&resource)
{
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
    const auto* ptr = dynamic_cast<const debug_memory_resource*>(&other);
    return ptr && ptr->resource_ == resource_;
}

int main(int argc, char** argv)
{
    debug_memory_resource debug_resource(*std::pmr::get_default_resource());
    std::pmr::set_default_resource(&debug_resource);

    // TODO: Temporary sequence of calls to force shared library linkage.
    lua_State* L = luaL_newstate();
    luaopen_hyjal(L);
    lua_close(L);

    doctest::Context context(argc, argv);
    int res = context.run();

    fmt::print("Memory statistics: {} bytes allocated, {} bytes freed, {} bytes leaked",
        debug_resource.allocated_bytes(),
        debug_resource.freed_bytes(),
        debug_resource.used_bytes());

    return res;
}

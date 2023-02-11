// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory_resource>

namespace meorawr::hyjal {
    class debug_memory_resource : public std::pmr::memory_resource {
    public:
        debug_memory_resource() noexcept;
        explicit debug_memory_resource(std::pmr::memory_resource& resource) noexcept;

        std::size_t allocated_bytes() const noexcept;
        std::size_t freed_bytes() const noexcept;

    private:
        void* do_allocate(std::size_t bytes, std::size_t alignment) override;
        void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

        std::pmr::memory_resource* resource_;
        std::size_t allocated_bytes_ = {};
        std::size_t freed_bytes_ = {};
    };
}

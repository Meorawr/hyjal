// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "debug_memory_listener.hpp"

#include "debug_memory_resource.hpp"

#include <meorawr/hyjal/hyjal.hpp>

namespace meorawr::hyjal {
    debug_memory_listener::debug_memory_listener(const doctest::ContextOptions& in)
        : cout_(*in.cout)
    {
    }

    void debug_memory_listener::test_run_start()
    {
    }

    void debug_memory_listener::test_run_end(const doctest::TestRunStats&)
    {
        namespace Color = doctest::Color;

        const auto* resource = dynamic_cast<const debug_memory_resource*>(get_default_memory_resource());

        if (resource) {
            std::size_t leaked_bytes = resource->allocated_bytes() - resource->freed_bytes();
            Color::Enum leaked_color = leaked_bytes ? Color::Red : Color::Green;

            cout_ << Color::Cyan << "[doctest] " << Color::None << "memory stats: "
                  << resource->allocated_bytes() << " bytes allocated | "
                  << resource->freed_bytes() << " bytes freed | "
                  << leaked_color << leaked_bytes << " bytes leaked" << Color::None
                  << "\n";
        }
    }

    void debug_memory_listener::report_query(const doctest::QueryData&)
    {
    }

    void debug_memory_listener::test_case_start(const doctest::TestCaseData&)
    {
    }

    void debug_memory_listener::test_case_reenter(const doctest::TestCaseData&)
    {
    }

    void debug_memory_listener::test_case_end(const doctest::CurrentTestCaseStats&)
    {
    }

    void debug_memory_listener::test_case_exception(const doctest::TestCaseException&)
    {
    }

    void debug_memory_listener::test_case_skipped(const doctest::TestCaseData&)
    {
    }

    void debug_memory_listener::subcase_start(const doctest::SubcaseSignature&)
    {
    }

    void debug_memory_listener::subcase_end()
    {
    }

    void debug_memory_listener::log_assert(const doctest::AssertData&)
    {
    }

    void debug_memory_listener::log_message(const doctest::MessageData&)
    {
    }
}

DOCTEST_REGISTER_LISTENER("debug_memory_listener", -1, meorawr::hyjal::debug_memory_listener);

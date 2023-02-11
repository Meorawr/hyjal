// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <doctest/doctest.h>

#include <ostream>

namespace meorawr::hyjal {
    class debug_memory_listener : public doctest::IReporter {
    public:
        explicit debug_memory_listener(const doctest::ContextOptions& in);

        void test_run_start() override;
        void test_run_end(const doctest::TestRunStats&) override;
        void report_query(const doctest::QueryData&) override;
        void test_case_start(const doctest::TestCaseData&) override;
        void test_case_reenter(const doctest::TestCaseData&) override;
        void test_case_end(const doctest::CurrentTestCaseStats&) override;
        void test_case_exception(const doctest::TestCaseException&) override;
        void test_case_skipped(const doctest::TestCaseData&) override;
        void subcase_start(const doctest::SubcaseSignature&) override;
        void subcase_end() override;
        void log_assert(const doctest::AssertData&) override;
        void log_message(const doctest::MessageData&) override;

    private:
        std::ostream& cout_;
    };
}

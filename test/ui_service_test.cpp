// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "object.hpp"
#include "test_util.hpp"
#include "ui_service.hpp"

#include <doctest/doctest.h>

TEST_SUITE("ui_service")
{
    using namespace meorawr::hyjal;
    using namespace meorawr::hyjal::test;

    TEST_CASE("object ownership"
        * doctest::description("Verifies that objects are destroyed automatically with the ui service"))
    {
        lua::unique_state state = make_lua_state();
        ui_service ui(state);

        allocate_object<object>(ui);

        // As long as it doesn't crash here it's good.
    }

    TEST_CASE("manual object deletion"
        * doctest::description("Verifies that objects can be manually created and deleted"))
    {
        lua::unique_state state = make_lua_state();
        ui_service ui(state);

        object* obj = new_object<object>(ui, "Test Object");
        delete_object(obj);

        // As long as it doesn't crash here it's good.
    }

    TEST_CASE("object list tracking"
        * doctest::description("Verifies that all created objects are stored in a list"))
    {
        static constexpr int num_objects = 1000;

        lua::unique_state state = make_lua_state();
        ui_service ui(state);

        for (auto i = 0; i < num_objects; ++i) {
            new_object<object>(ui, "");
        }

        DOCTEST_REQUIRE_EQ(ui.objects().size(), num_objects);
    }
}

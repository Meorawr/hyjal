// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "debug_memory_listener.hpp"
#include "debug_memory_resource.hpp"

#include <meorawr/hyjal/hyjal.hpp>

#include <doctest/doctest.h>

namespace meorawr::hyjal {
    extern "C" int main(int argc, char** argv)
    {
        debug_memory_resource debug_resource;
        set_default_memory_resource(&debug_resource);

        doctest::Context context(argc, argv);
        return context.run();
    }
}

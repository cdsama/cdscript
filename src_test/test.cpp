// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("std", "[core]")
{
    try
    {
        INFO("num " << 9223372036854775807 << std::stoll("9223372036854775808") << 9223372036854775808e20 << std::numeric_limits<int64_t>::max());
    }
    catch (const std::out_of_range &e)
    {
        std::cout << e.what() << '\n';
    }

    CHECK(true);
}
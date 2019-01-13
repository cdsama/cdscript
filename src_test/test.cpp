// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include <cerrno>
#include "catch2_ext.hpp"

TEST_CASE("std", "[test]")
{
    // auto x = std::strtoull("1777777777777777777777", nullptr, 8);
    // std::cout << x << std::endl;
    // if (errno == ERANGE)
    // {
    //     std::cout << "out of range" << std::endl;
    //     errno = 0;
    // }
    CHECK(true);
}

TEST_CASE("catch2-ext", "[test]")
{
    CHECK(WhatEquals("what").describe() == "\n!=\nwhat");
}
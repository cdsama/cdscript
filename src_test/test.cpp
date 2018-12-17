// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch2_ext.hpp"

#include <cerrno>

TEST_CASE("std", "[test]")
{
    auto x = std::strtoull("0000100000001111111111111111111111111111111111111111111111111111111111111111", 0, 2);
    std::cout << x << std::endl;
    if (errno == ERANGE)
    {
        std::cout << "out of range" << std::endl;
        errno = 0;
    }
    std::cout << (int32_t)((uint8_t)INT16_MAX) << std::endl;
    CHECK(true);
}

TEST_CASE("catch2-ext", "[test]")
{
    CHECK(WhatEquals("what").describe() == "\n!=\nwhat");
}
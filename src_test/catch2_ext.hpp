// Copyright (c) 2019 chendi
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <catch2/catch.hpp>
#include "utils.hpp"
using namespace cd::exception;
class WhatEquals : public Catch::MatcherBase<std::exception>
{
    std::string what;

  public:
    WhatEquals(const std::string &_what) : what(_what) {}

    virtual bool match(std::exception const &i) const override
    {
        return i.what() == what;
    }

    virtual std::string describe() const override
    {
        return "\n!=\n" + what;
    }
};
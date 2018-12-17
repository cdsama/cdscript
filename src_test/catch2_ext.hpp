#pragma once
#include <catch2/catch.hpp>

class WhatEquals : public Catch::MatcherBase<std::exception>
{
    std::string what;

  public:
    WhatEquals(const std::string &_what) : what(_what) {}

    virtual bool match(std::exception const &i) const override
    {
        return i.what() == what;
    }

    virtual std::string describe() const
    {
        return "\n!=\n" + what;
    }
};
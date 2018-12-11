// Copyright (c) 2018 chendi
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "lexer.hpp"
#include <sstream>
#include <limits>

using namespace cdscript;

TEST_CASE("Lexer","[core][lexer]")
{
    std::istringstream code("\"hello\"");
    auto lexer = Lexer::GetLexer(code);
    CHECK(lexer->GetToken().type == Token::String);
}

TEST_CASE("std","[core]")
{
    INFO("num " << 9223372036854775807 << std::stoll("9223372036854775808") << 9223372036854775808e20 << std::numeric_limits<int64_t>::max());
    CHECK(true);
}
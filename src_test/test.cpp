// Copyright (c) 2018 chendi
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "lexer.hpp"
#include <sstream>

using namespace cdscript;

TEST_CASE("Lexer","[core][lexer]")
{
    std::istringstream code("");
    auto lexer = Lexer::GetLexer(code);
    Token token;
    CHECK(lexer->GetToken(token) == Token::EndOfFile);
}
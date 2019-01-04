// Copyright (c) 2019 chendi
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cd::script;
TEST_CASE("Lexer-Identifier", "[core][lexer][newline]")
{
    {
        std::istringstream code("abc");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Identifier);
        CHECK(token.str() == "abc");
    }
    {
        std::istringstream code("\a\b");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected character :'\a' line:1 column:1"));
    }
}
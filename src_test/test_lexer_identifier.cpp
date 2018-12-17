#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cdscript;
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
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected character :\a line:1 column:1"));
    }
}
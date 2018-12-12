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

TEST_CASE("Lexer-String", "[core][lexer][string]")
{
    std::istringstream code("\"hello\"\"\"'hello'''");
    auto lexer = Lexer::GetLexer(code);
    auto token = lexer->GetToken();
    CHECK(token.type == Token::String);
    CHECK(std::any_cast<std::string>(token.value) == "hello");
    token = lexer->GetToken();
    CHECK(token.type == Token::String);
    CHECK(std::any_cast<std::string>(token.value) == "");
    token = lexer->GetToken();
    CHECK(token.type == Token::String);
    CHECK(std::any_cast<std::string>(token.value) == "hello");
    token = lexer->GetToken();
    CHECK(token.type == Token::String);
    CHECK(std::any_cast<std::string>(token.value) == "");
    token = lexer->GetToken();
    CHECK(token.type == Token::EndOfFile);
}

TEST_CASE("Lexer-String-Exception", "[core][lexer][string]")
{
    {
        std::istringstream code("\"\n\"");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_AS(lexer->GetToken(), exception::LexerError);
    }
    {
        std::istringstream code("'\n'");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_AS(lexer->GetToken(), exception::LexerError);
    }
    {
        std::istringstream code("\"");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_AS(lexer->GetToken(), exception::LexerError);
    }
    {
        std::istringstream code("'");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_AS(lexer->GetToken(), exception::LexerError);
    }
}

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
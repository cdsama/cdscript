// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cd;
using namespace script;
TEST_CASE("Lexer-NewLine", "[core][lexer][newline]")
{
    {
        std::istringstream code("");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 1);
        CHECK(token.column == 0);
    }
    {
        std::istringstream code("    ");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 1);
        CHECK(token.column == 4);
    }
    {
        std::istringstream code("\n");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 2);
        CHECK(token.column == 0);
    }
    {
        std::istringstream code("\n ");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 2);
        CHECK(token.column == 1);
    }
    {
        std::istringstream code("\n\r");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 2);
        CHECK(token.column == 0);
    }
    {
        std::istringstream code("\r\n");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 2);
        CHECK(token.column == 0);
    }
    {
        std::istringstream code("\n\n \n");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 4);
        CHECK(token.column == 0);
    }

    {
        std::istringstream code("\r\r \r");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 4);
        CHECK(token.column == 0);
    }
    {
        std::istringstream code("\r\r \n");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 4);
        CHECK(token.column == 0);
    }
    {
        std::istringstream code("\n\n \r");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
        CHECK(token.line == 4);
        CHECK(token.column == 0);
    }
}
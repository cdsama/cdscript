#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cdscript;
TEST_CASE("Lexer-Comment-SingleLine", "[core][lexer][comment]")
{
    {
        std::istringstream code("//abc");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "abc");
    }
    {
        std::istringstream code("//abc\n//abcd");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "abc");
        token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "abcd");
    }
    {
        std::istringstream code("abc//abc");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "abc");
    }
}

TEST_CASE("Lexer-Comment-MultiLine", "[core][lexer][comment]")
{
    {
        std::istringstream code("/*abc*/");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "abc");
    }
    {
        std::istringstream code("/*abc\n//abcd*/");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "abc\n//abcd");
    }
    {
        std::istringstream code("/*abc\r\n//abcd*/");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "abc\r\n//abcd");
    }
    {
        std::istringstream code("/*****-/***//**///\n");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "****-/**");
        token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "");
        CHECK(token.type == Token::Comment);
        CHECK(token.str() == "");
    }
    {
        std::istringstream code("/*abc\r\n//abcd");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("comment unclosed at <eof>"));
    }
    {
        std::istringstream code("/*abc\r\n//abcd****\n***xyz***\r***");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("comment unclosed at <eof>"));
    }
}
#include <catch2/catch.hpp>
#include "lexer.hpp"
using namespace cdscript;
TEST_CASE("Lexer-Comment-SingleLine", "[core][lexer][comment]")
{
    {
        std::istringstream code("//abc");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "abc");
    }
    {
        std::istringstream code("//abc\n//abcd");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "abc");
        token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "abcd");
    }
    {
        std::istringstream code("abc//abc");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "abc");
    }
}

TEST_CASE("Lexer-Comment-MultiLine", "[core][lexer][comment]")
{
    {
        std::istringstream code("/*abc*/");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "abc");
    }
    {
        std::istringstream code("/*abc\n//abcd*/");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "abc\n//abcd");
    }
    {
        std::istringstream code("/*abc\r\n//abcd*/");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "abc\r\n//abcd");
    }
    {
        std::istringstream code("/*****-/***//**///\n");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "****-/**");
        token = lexer->GetToken();
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "");
        CHECK(token.type == Token::Comment);
        CHECK(std::any_cast<std::string>(token.value) == "");
    }
    {
        std::istringstream code("/*abc\r\n//abcd");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), "comment unclosed at <eof>");
    }
    {
        std::istringstream code("/*abc\r\n//abcd****\n***xyz***\r***");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), "comment unclosed at <eof>");
    }
}
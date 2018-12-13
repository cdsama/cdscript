#include <catch2/catch.hpp>

#include "lexer.hpp"
#include <sstream>
#include <limits>

using namespace cdscript;

TEST_CASE("Lexer-String", "[core][lexer][string]")
{
    std::istringstream code(R"___("hello"""'hello''')___");
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

TEST_CASE("Lexer-String-EscapeCharacter", "[core][lexer][string]")
{
    {
        std::istringstream code(R"___("\a\b\f\n\r\t\v\\\"\'")___");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == "\a\b\f\n\r\t\v\\\"\'");
        token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
    }
    {
        std::istringstream code(R"___("\x48\x65\x6c\x6c\x6f \x57\x6f\x72\x6c\x64\x21")___");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == "Hello World!");
        token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
    }
    {
        std::istringstream code(R"___("\0\72\101\108\108\111 \87\111\114\108\100\33\255")___");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == std::string("\0Hello World!\xff", 14));
        token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
    }
    {
        std::istringstream code(R"___("\0725\x48TML5")___");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == "H5HTML5");
        token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
    }
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
    {
        std::istringstream code(R"___("\xxx")___");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_AS(lexer->GetToken(), exception::LexerError);
    }
    {
        std::istringstream code(R"___("\256")___");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_AS(lexer->GetToken(), exception::LexerError);
    }
}
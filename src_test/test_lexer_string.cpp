#include <catch2/catch.hpp>

#include "lexer.hpp"
#include <sstream>
#include <limits>
#include <iostream>

using namespace cdscript;

TEST_CASE("Lexer-String", "[core][lexer][string]")
{
    std::istringstream code(R"#("hello"""'hello''')#");
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
        std::istringstream code(R"#("\f\n\r\t\v\\\"\'")#");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == "\f\n\r\t\v\\\"\'");
        token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
    }
    {
        std::istringstream code(R"#("\x48\x65\x6c\x6c\x6f \x57\x6f\x72\x6c\x64\x21")#");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == "Hello World!");
        token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
    }
    {
        std::istringstream code(R"#("\0\72\101\108\108\111 \87\111\114\108\100\33\255")#");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == std::string("\0Hello World!\xff", 14));
        token = lexer->GetToken();
        CHECK(token.type == Token::EndOfFile);
    }
    {
        std::istringstream code(R"#("\0725\x48TML5")#");
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
        CHECK_THROWS_WITH(lexer->GetToken(), "incomplete string at line:1 column:2");
    }
    {
        std::istringstream code("'\n'");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), "incomplete string at line:1 column:2");
    }
    {
        std::istringstream code("\"");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), "incomplete string at <eof>");
    }
    {
        std::istringstream code("'");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), "incomplete string at <eof>");
    }
    {
        std::istringstream code(R"#("\xxx")#");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), R"#(unexpect character after '\x' line:1 column:4)#");
    }
    {
        std::istringstream code(R"#("\256")#");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), R"#(decimal escape too large near \256 line:1 column:6)#");
    }
    {
        std::istringstream code(R"#("\p")#");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), R"#(unexpect character after '\' line:1 column:3)#");
    }
}

TEST_CASE("Lexer-String-Raw", "[core][lexer][string]")
{
    {
        std::istringstream code(R"#(R"(\n)")#");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == R"#(\n)#");
    }
    {
        std::istringstream code(R"#(R"________________(\n)________________")#");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::String);
        CHECK(std::any_cast<std::string>(token.value) == R"#(\n)#");
    }
    {
        std::istringstream code(R"#(R"_________________()_________________")#");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), R"#(raw string delimiter longer than 16 characters : line:1 column:19)#");
    }
    {
        std::istringstream code(R"#(R"@()@")#");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), R"#(invalid character in raw string delimiter :@ line:1 column:3)#");
    }
    {
        std::istringstream code(R"#(R"()@")#");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), R"#(incomplete string at line:1 column:6)#");
    }
}
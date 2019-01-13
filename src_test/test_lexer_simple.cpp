// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <unordered_map>
#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cd;
using namespace script;
TEST_CASE("Lexer-SingleToken", "[core][lexer][simple]")
{
    std::istringstream code("+ - * / % & | ^ ! =");
    auto lexer = Lexer::GetLexer(code);
    CHECK(lexer->GetToken().type == '+');
    CHECK(lexer->GetToken().type == '-');
    CHECK(lexer->GetToken().type == '*');
    CHECK(lexer->GetToken().type == '/');
    CHECK(lexer->GetToken().type == '%');
    CHECK(lexer->GetToken().type == '&');
    CHECK(lexer->GetToken().type == '|');
    CHECK(lexer->GetToken().type == '^');
    CHECK(lexer->GetToken().type == '!');
    CHECK(lexer->GetToken().type == '=');
}

TEST_CASE("Lexer-PureSingleToken", "[core][lexer][simple]")
{
    std::istringstream code("?:;,@#()[]{}");
    auto lexer = Lexer::GetLexer(code);
    CHECK(lexer->GetToken().type == '?');
    CHECK(lexer->GetToken().type == ':');
    CHECK(lexer->GetToken().type == ';');
    CHECK(lexer->GetToken().type == ',');
    CHECK(lexer->GetToken().type == '@');
    CHECK(lexer->GetToken().type == '#');
    CHECK(lexer->GetToken().type == '(');
    CHECK(lexer->GetToken().type == ')');
    CHECK(lexer->GetToken().type == '[');
    CHECK(lexer->GetToken().type == ']');
    CHECK(lexer->GetToken().type == '{');
    CHECK(lexer->GetToken().type == '}');
}

TEST_CASE("Lexer-dot", "[core][lexer][simple]")
{
    std::istringstream code(". .. ...");
    auto lexer = Lexer::GetLexer(code);
    CHECK(lexer->GetToken().type == '.');
    CHECK(lexer->GetToken().type == Token::Concat);
    CHECK(lexer->GetToken().type == Token::VarArg);
}

TEST_CASE("Lexer-XEqualToken", "[core][lexer][simple]")
{
    std::istringstream code("+= -= *= /= %= &= |= ^= != ==");
    auto lexer = Lexer::GetLexer(code);
    CHECK(lexer->GetToken().type == Token::PlusEqual);
    CHECK(lexer->GetToken().type == Token::MinusEqual);
    CHECK(lexer->GetToken().type == Token::MultiplyEqual);
    CHECK(lexer->GetToken().type == Token::DivideEqual);
    CHECK(lexer->GetToken().type == Token::ModuloEqual);
    CHECK(lexer->GetToken().type == Token::AndEqual);
    CHECK(lexer->GetToken().type == Token::OrEqual);
    CHECK(lexer->GetToken().type == Token::ExclusiveOrEqual);
    CHECK(lexer->GetToken().type == Token::NotEqual);
    CHECK(lexer->GetToken().type == Token::Equal);
}
TEST_CASE("Lexer-XXToken", "[core][lexer][simple]")
{
    std::istringstream code("<< >> ++ -- && ||");
    auto lexer = Lexer::GetLexer(code);
    CHECK(lexer->GetToken().type == Token::LeftShift);
    CHECK(lexer->GetToken().type == Token::RightShift);
    CHECK(lexer->GetToken().type == Token::PlusPlus);
    CHECK(lexer->GetToken().type == Token::MinusMinus);
    CHECK(lexer->GetToken().type == Token::And);
    CHECK(lexer->GetToken().type == Token::Or);
}

TEST_CASE("Lexer-KeyWords", "[core][lexer][simple]")
{
    std::unordered_map<std::string, token_t> KeyWords = {
        {"null", Token::Null},
        {"true", Token::True},
        {"false", Token::False},
        {"if", Token::If},
        {"else", Token::Else},
        {"for", Token::For},
        {"while", Token::While},
        {"in", Token::In},
        {"break", Token::Break},
        {"continue", Token::Continue},
        {"return", Token::Return},
        {"fun", Token::Function},
        {"try", Token::Try},
        {"catch", Token::Catch},
        {"throw", Token::Throw},
        {"class", Token::Class},
        {"interface", Token::Interface},
        {"is", Token::Is},
        {"object", Token::Object},
        {"this", Token::This},
        {"super", Token::Super},
        {"any", Token::Any},
    };

    for (auto &&i : KeyWords)
    {
        std::istringstream code(i.first);
        auto lexer = Lexer::GetLexer(code);
        CHECK(lexer->GetToken().type == i.second);
    }
}
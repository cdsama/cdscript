#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cdscript;
TEST_CASE("Lexer-SingleToken", "[core][lexer][simple]")
{

    std::istringstream code("+ - * / % & | ^ !");
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
    std::istringstream code("+= -= *= /= %= &= |= ^= !=");
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
#include <catch2/catch.hpp>
#include "lexer.hpp"
using namespace cdscript;
TEST_CASE("Lexer-Identifier", "[core][lexer][newline]")
{
    {
        std::istringstream code("abc");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Identifier);
        CHECK(std::any_cast<std::string>(token.value) == "abc");
    }
    {
        std::istringstream code("\a\b");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_WITH(lexer->GetToken(), "unexpect character :\a line:1 column:1");
    }
}
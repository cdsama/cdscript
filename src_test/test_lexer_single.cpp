#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cdscript;
TEST_CASE("Lexer-Single", "[core][lexer][single]")
{
    {
        std::istringstream code("/");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == '/');
    }
}
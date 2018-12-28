#include "catch2_ext.hpp"

#include <sstream>
#include <limits>
#include <iostream>
#include <any>
#include <list>
#include "lexer.hpp"
#include "parser.hpp"
#include "visitor.hpp"

using namespace cdscript;

class TestVisitor : public Visitor
{
  public:
    void Visit(BinaryExpress *syntax, std::any &data) override
    {
        (void)syntax;
        auto types = std::any_cast<std::list<std::string> *>(data);
        types->push_back("BinaryExpress");
    }
};

TEST_CASE("Parser ", "[core][parser]")
{
    std::istringstream code("1+1");
    auto lexer = Lexer::GetLexer(code);
    auto parser = Parser::GetParser(lexer);
    auto ast = parser->GetAbstractSyntaxTree();
    std::list<std::string> types;
    std::any data = &types;
    TestVisitor visitor;
    ast->Visit(&visitor, data);
    REQUIRE_FALSE(types.empty());
    CHECK(types.front() == "BinaryExpress");
}
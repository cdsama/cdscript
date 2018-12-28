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
#define DEFAULT_VISIT_IMPL(__CLASS_NAME__)                          \
    void Visit(__CLASS_NAME__ *syntax, std::any &data) override     \
    {                                                               \
        (void)syntax;                                               \
        auto types = std::any_cast<std::list<std::string> *>(data); \
        types->push_back(#__CLASS_NAME__);                          \
    }

    void Visit(BinaryExpression *syntax, std::any &data) override
    {
        (void)syntax;
        auto types = std::any_cast<std::list<std::string> *>(data);
        types->push_back("BinaryExpression");
        syntax->left->Visit(this, data);
        syntax->right->Visit(this, data);
    }
    DEFAULT_VISIT_IMPL(LiteralValue)
};

TEST_CASE("Parser-LiteralValue", "[core][parser]")
{
    {
        std::istringstream code("1");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<std::string> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
    }
}

TEST_CASE("Parser-Binary", "[core][parser]")
{
    {
        std::istringstream code("1 + 1 + 1");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<std::string> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "BinaryExpression");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "BinaryExpression");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
    }
    {
        std::istringstream code("1 + 1 * 2");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<std::string> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "BinaryExpression");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "BinaryExpression");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
    }
    {
        std::istringstream code("1 * 1 + 2");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<std::string> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "BinaryExpression");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "BinaryExpression");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
        types.pop_front();
        REQUIRE_FALSE(types.empty());
        CHECK(types.front() == "LiteralValue");
    }
}
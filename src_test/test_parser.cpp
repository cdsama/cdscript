// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "catch2_ext.hpp"

#include <sstream>
#include <limits>
#include <iostream>
#include <any>
#include <list>
#include "lexer.hpp"
#include "parser.hpp"
#include "visitor.hpp"

using namespace cd;
using namespace script;


enum class ESyntax
{
    LiteralValue = 0,
    BinaryExpression,
};

class TestVisitor : public Visitor
{
  public:
#define DEFAULT_VISIT_IMPL(__CLASS_NAME__, __CHILDREN_VISIT__)  \
    void Visit(__CLASS_NAME__ *syntax, std::any &data) override \
    {                                                           \
        (void)syntax;                                           \
        auto types = std::any_cast<std::list<int> *>(data);     \
        types->push_back((int)ESyntax::__CLASS_NAME__);         \
        __CHILDREN_VISIT__                                      \
    }
    DEFAULT_VISIT_IMPL(LiteralValue, )
    DEFAULT_VISIT_IMPL(BinaryExpression, syntax->left->Visit(this, data); syntax->right->Visit(this, data);)
};

TEST_CASE("Parser-LiteralValue", "[core][parser]")
{
    {
        std::istringstream code("1");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<int> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        REQUIRE_FALSE(types.empty());
        std::list<int> result = {0};
        CHECK(types == result);
    }
}

TEST_CASE("Parser-Binary", "[core][parser]")
{
    {
        std::istringstream code("1 + 1 + 1");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<int> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        std::list<int> result = {1, 1, 0, 0, 0};
        CHECK(types == result);
    }
    {
        std::istringstream code("1 + 1 * 2");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<int> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        std::list<int> result = {1, 0, 1, 0, 0};
        CHECK(types == result);
    }
    {
        std::istringstream code("1 * 1 + 2");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<int> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        std::list<int> result = {1, 1, 0, 0, 0};
        CHECK(types == result);
    }
    {
        std::istringstream code("true && false || true");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<int> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        std::list<int> result = {1, 1, 0, 0, 0};
        CHECK(types == result);
    }
    {
        std::istringstream code("1 * 2 / 3 % 4 << 5 >> 6 < 7 > 8 <= 9 >= 10 == 11 != 12 & 13 ^ 14 | 15 && 16 || 17");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        auto ast = parser->GetAbstractSyntaxTree();
        std::list<int> types;
        std::any data = &types;
        TestVisitor visitor;
        ast->Visit(&visitor, data);
        std::list<int> result = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        CHECK(types == result);
    }
}
TEST_CASE("Parser-Comment", "[core][parser]")
{
    {
        std::istringstream code("//some comment\n 1 + 1");
        auto lexer = Lexer::GetLexer(code);
        auto parser = Parser::GetParser(lexer);
        REQUIRE_NOTHROW(parser->GetAbstractSyntaxTree());
    }
}
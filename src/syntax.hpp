// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <memory>
#include <any>
#include "token.hpp"

namespace cdscript
{
class Visitor;
class Syntax
{
  public:
    virtual ~Syntax() {}
    virtual void Visit([[maybe_unused]] Visitor *visitor, [[maybe_unused]] std::any &data){};
};

using syntax_t = std::unique_ptr<Syntax>;
#define DECL_VISIT_FUNC() void Visit(Visitor *visitor, std::any &data) override

class BinaryExpression : public Syntax
{
  public:
    BinaryExpression(syntax_t _left, syntax_t _right, Token &&_op)
        : left(std::move(_left)), right(std::move(_right)), op(_op)
    {
    }
    syntax_t left;
    syntax_t right;
    Token op;
    DECL_VISIT_FUNC();
};

class LiteralValue : public Syntax
{
  public:
    LiteralValue(Token &&_value)
        : value(_value)
    {
    }
    Token value;
    DECL_VISIT_FUNC();
};

} // namespace cdscript

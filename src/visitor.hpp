// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include "syntax.hpp"

namespace cd::script
{
class Visitor
{
  public:
    virtual ~Visitor() {}
    virtual void Visit(BinaryExpression *syntax, std::any &data) = 0;
    virtual void Visit(LiteralValue *syntax, std::any &data) = 0;
};

}  // namespace cd::script

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
    virtual void Visit(Visitor *visitor, std::any &data) = 0;
};

#define DECL_VISIT_FUNC() void Visit(Visitor *visitor, std::any &data) override

class BinaryExpress : public Syntax
{
  public:
    DECL_VISIT_FUNC();
};

} // namespace cdscript

// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include "syntax.hpp"

namespace cdscript
{

class Visitor
{
  public:
    virtual ~Visitor() {}
    virtual void Visit(BinaryExpress *syntax, std::any &data) = 0;
};

} // namespace cdscript

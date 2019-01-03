// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <sstream>
#include "syntax.hpp"

namespace cd::script
{
class Parser
{
  public:
    virtual ~Parser(){};
    [[nodiscard]] virtual std::unique_ptr<class Syntax> GetAbstractSyntaxTree() = 0;
    [[nodiscard]] static std::unique_ptr<Parser> GetParser(std::unique_ptr<class Lexer> &lexer);
};
} // namespace cd::script

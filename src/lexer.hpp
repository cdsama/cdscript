// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include "token.hpp"
#include <istream>

namespace cdscript
{

class Lexer
{
  public:
    virtual ~Lexer() {}
    virtual token_t GetToken(Token &token) = 0;
    static std::unique_ptr<Lexer> GetLexer(std::istream &code);
};

} // namespace cdscript

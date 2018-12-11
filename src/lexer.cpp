// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "lexer.hpp"

namespace cdscript
{

class LexerImpl : public Lexer
{
  private:
    std::istream &code;

  public:
    LexerImpl(std::istream &_code)
        : code(_code)
    {
    }
    ~LexerImpl()
    {
    }

    virtual token_t GetToken(Token &token) override
    {
        return Token::EndOfFile;
    }
};

std::unique_ptr<Lexer> Lexer::GetLexer(std::istream &code)
{
    return std::make_unique<LexerImpl>(code);
}
} // namespace cdscript

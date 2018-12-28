// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "lexer.hpp"
#include "syntax.hpp"
#include "parser.hpp"

namespace cdscript
{

class ParserImpl : public Parser
{
  private:
    std::unique_ptr<Lexer> &lexer;

  public:
    ParserImpl(std::unique_ptr<Lexer> &_lexer)
        : lexer(_lexer)
    {
    }
    ~ParserImpl() {}

    std::unique_ptr<Syntax> GetAbstractSyntaxTree() override
    {
        return std::make_unique<BinaryExpress>();
    }

    std::unique_ptr<Syntax> ParseExpress()
    {
    }
};

std::unique_ptr<Parser> Parser::GetParser(std::unique_ptr<Lexer> &lexer)
{
    return std::make_unique<ParserImpl>(lexer);
}
} // namespace cdscript

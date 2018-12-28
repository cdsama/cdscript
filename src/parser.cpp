// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cassert>

#include "lexer.hpp"
#include "syntax.hpp"
#include "parser.hpp"

namespace cdscript
{

class ParserImpl : public Parser
{
  private:
    std::unique_ptr<Lexer> &lexer;
    Token current;
    Token ahead1;
    Token ahead2;
    Token lastcomment;

  public:
    ParserImpl(std::unique_ptr<Lexer> &_lexer)
        : lexer(_lexer)
    {
    }
    ~ParserImpl() {}

    std::unique_ptr<Syntax> GetAbstractSyntaxTree() override
    {
        return ParseExpression();
    }

  private:
    using precedence_t = int32_t;
    precedence_t GetOperatorPrecedence(token_t type)
    {
        switch (type)
        {
        case '*':
        case '/':
        case '%':
            return 90;
        case '+':
        case '-':
            return 80;
        default:
            return 0;
        }
    }

    bool IsDirectExpression(token_t type)
    {
        switch (type)
        {
        case Token::Number:
        case Token::String:
            return true;
        default:
            return false;
        }
    }

    syntax_t ParseExpression([[maybe_unused]] syntax_t left = syntax_t(), [[maybe_unused]] precedence_t left_precedence = 0, [[maybe_unused]] Token op = Token())
    {
        LookAhead();
        std::unique_ptr<Syntax> expression;
        if (IsDirectExpression(ahead1.type))
        {
            expression = ParseDirectExpression();
        }
        while (true)
        {
            int right_precedence = GetOperatorPrecedence(LookAhead().type);
            if (left_precedence < right_precedence)
            {
                expression = ParseExpression(std::move(expression), right_precedence, NextToken());
            }
            else if (left_precedence == right_precedence)
            {
                if (left_precedence == 0)
                    return expression;
                assert(left);
                expression = std::make_unique<BinaryExpression>(std::move(left), std::move(expression), std::move(op));
                return ParseExpression(std::move(expression), right_precedence, NextToken());
            }
            else
            {
                if (left)
                {
                    expression = std::make_unique<BinaryExpression>(std::move(left), std::move(expression), std::move(op));
                }
                return expression;
            }
        }
    }

    syntax_t ParseDirectExpression()
    {
        switch (LookAhead().type)
        {
        case Token::Number:
        case Token::String:
            return std::make_unique<LiteralValue>(std::move(NextToken()));
        default:
            throw - 1;
        }
    }

    Token GetNoCommentToken()
    {
        Token token = lexer->GetToken();
        while (token.type == Token::Comment)
        {
            lastcomment = std::move(token);
            token = lexer->GetToken();
        }
        return token;
    }

    Token &NextToken()
    {
        if (ahead1.type != Token::EndOfFile)
        {
            current = ahead1;
            ahead1 = ahead2;
            if (ahead2.type != Token::EndOfFile)
            {
                ahead2.type = Token::EndOfFile;
            }
        }
        else
        {
            current = GetNoCommentToken();
        }
        return current;
    }

    Token &LookAhead()
    {
        if (ahead1.type == Token::EndOfFile)
        {
            ahead1 = GetNoCommentToken();
        }
        return ahead1;
    }

    Token &LookAhead2()
    {
        LookAhead();
        if (ahead2.type == Token::EndOfFile)
        {
            ahead2 = GetNoCommentToken();
        }
        return ahead2;
    }
};

std::unique_ptr<Parser> Parser::GetParser(std::unique_ptr<Lexer> &lexer)
{
    return std::make_unique<ParserImpl>(lexer);
}
} // namespace cdscript

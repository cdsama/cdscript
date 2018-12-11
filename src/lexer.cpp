// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "lexer.hpp"
#include <iostream>

namespace cdscript
{

class LexerImpl : public Lexer
{
  private:
    std::istream &code;
    char current;
    size_t line;
    size_t column;
    std::string buffer;

  public:
    LexerImpl(std::istream &_code)
        : code(_code), current(EOF), line(1), column(0), buffer("")
    {
    }
    ~LexerImpl()
    {
    }

    inline Token EndOfFileToken()
    {
        Token token;
        token.line = line;
        token.column = column;
        token.type = Token::EndOfFile;
        return token;
    }

    virtual Token GetToken() override
    {
        if (current == EOF)
        {
            current = Next();
        }

        while (current != EOF)
        {

            switch (current)
            {
            case ' ':
            case '\a':
            case '\b':
            case '\f':
            case '\t':
            case '\v':
            {
                current = Next();
                break;
            }
            case '\'':
            case '"':
            {
                return SingleLineStringToken();
            }

            default:
                current = Next();
                break;
            }
        }

        return EndOfFileToken();
    }

    char Next()
    {
        if (code.eof())
        {
            return EOF;
        }
        char next;
        code >> next;
        std::cout << current << std::endl;
        return next;
    }

    inline Token NormalToken(token_t type)
    {
        Token token;
        token.type = type;
        token.line = line;
        token.column = column;
        return token;
    }
    inline Token StringToken(token_t type, const std::string &str)
    {
        auto token = NormalToken(type);
        token.value = str;
        return token;
    }

    Token SingleLineStringToken()
    {
        auto quote = current;
        current = Next();
        buffer.clear();

        while (current != quote)
        {
            if (current == EOF)
            {
                throw exception::LexerError("incomplete string at <eof>");
            }

            if (current == '\r' || current == '\n')
            {
                throw exception::LexerError("incomplete string at this line: ") << line;
            }

            current = Next();
            buffer.push_back(current);
        }

        current = Next();
        return StringToken(Token::String, buffer);
    }
};

std::unique_ptr<Lexer> Lexer::GetLexer(std::istream &code)
{
    return std::make_unique<LexerImpl>(code);
}
} // namespace cdscript

// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "lexer.hpp"
#include <iostream>
#include <cctype>

namespace cdscript
{
bool isdigit(char ch)
{
    return std::isdigit(static_cast<unsigned char>(ch));
}
bool isxdigit(char ch)
{
    return std::isxdigit(static_cast<unsigned char>(ch));
}
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
            case '\0':
            {
                current = Next();
                break;
            }
            case '\r':
            case '\n':
            {
                NewLine();
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
        char next;
        code.read(&next, sizeof(next));
        if (code.eof())
        {
            return EOF;
        }
        column++;
        return next;
    }

    void NewLine()
    {
        ++line;
        int next = Next();
        if (((next == '\r' || next == '\n') && next != current) || (next == EOF))
        {
            column = 0;
            current = Next();
        }
        else
        {
            column = 1;
            current = next;
        }
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
                throw exception::LexerError("incomplete string at line:") << line << " column:" << column;
            }

            ConvertEscapeCharacter();
        }

        current = Next();
        return StringToken(Token::String, buffer);
    }

    void ConvertEscapeCharacter()
    {
        if (current == '\\')
        {
            current = Next();
            if (current == 'a')
            {
                buffer.push_back('\a');
            }
            else if (current == 'b')
            {
                buffer.push_back('\b');
            }
            else if (current == 'f')
            {
                buffer.push_back('\f');
            }
            else if (current == 'n')
            {
                buffer.push_back('\n');
            }
            else if (current == 'r')
            {
                buffer.push_back('\r');
            }
            else if (current == 't')
            {
                buffer.push_back('\t');
            }
            else if (current == 'v')
            {
                buffer.push_back('\v');
            }
            else if (current == '\\')
            {
                buffer.push_back('\\');
            }
            else if (current == '"')
            {
                buffer.push_back('"');
            }
            else if (current == '\'')
            {
                buffer.push_back('\'');
            }
            else if (current == 'x')
            {
                current = Next();
                char hex[3] = {0};
                int i = 0;
                for (; i < 2 && isxdigit(current); ++i, current = Next())
                {
                    hex[i] = current;
                }
                if (i == 0)
                {
                    throw exception::LexerError("unexpect character after '\\x' line:") << line << " column:" << column;
                }
                buffer.push_back(static_cast<char>(std::strtoul(hex, 0, 16)));
                return;
            }
            else if (isdigit(current))
            {
                char dec[4] = {0};
                for (int i = 0; i < 3 && isdigit(current); ++i, current = Next())
                {
                    dec[i] = current;
                }
                auto result = std::strtoul(dec, 0, 10);
                if (result > 255)
                {
                    throw exception::LexerError("decimal escape too large near \\") << result << " line:" << line << " column:" << column;
                }
                buffer.push_back(static_cast<char>(result));
                return;
            }
            else
            {
                throw exception::LexerError("unexpect character after '\\' line:") << line << " column:" << column;
            }
        }
        else
        {
            buffer.push_back(current);
        }

        current = Next();
    }
};

std::unique_ptr<Lexer> Lexer::GetLexer(std::istream &code)
{
    return std::make_unique<LexerImpl>(code);
}
} // namespace cdscript

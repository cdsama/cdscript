// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "lexer.hpp"
#include <iostream>
#include <cctype>
#include <cerrno>
#include <algorithm>

namespace cdscript
{
static const std::string disallowed = R"#($()\`@)#";
inline bool isbdigit(char ch) { return ch == '0' || ch == '1'; }
inline bool isodigit(char ch) { return ch >= '0' && ch <= '8'; }
inline bool isdigit(char ch) { return std::isdigit(static_cast<unsigned char>(ch)); }
inline bool isxdigit(char ch) { return std::isxdigit(static_cast<unsigned char>(ch)); }
inline bool isalpha(char ch) { return std::isalpha(static_cast<unsigned char>(ch)); }
inline bool isalnum(char ch) { return std::isalnum(static_cast<unsigned char>(ch)); }
inline bool ispunct(char ch) { return std::ispunct(static_cast<unsigned char>(ch)); }
inline bool isidhead(char ch) { return isalpha(ch) || ch == '_'; }
inline bool isidbody(char ch) { return isalnum(ch) || ch == '_'; }
inline bool isallowedpunct(char ch) { return ispunct(ch) && disallowed.find(ch) == std::string::npos; }
inline bool isdelimiter(char ch) { return isalnum(ch) || isallowedpunct(ch); }
inline bool isexponent(char ch) { return ch == 'E' || ch == 'e'; }
inline bool isxexponent(char ch) { return ch == 'P' || ch == 'p'; }
inline bool isunsigned(char ch) { return ch == 'U' || ch == 'u'; }
inline bool issigned(char ch) { return ch == 'I' || ch == 'i'; }
inline bool isfloat(char ch) { return ch == 'F' || ch == 'f'; }

static const size_t BIT8 = 8;
static const size_t BIT16 = 16;
static const size_t BIT32 = 32;
static const size_t BIT64 = 64;

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

    inline Token NormalToken(token_t type)
    {
        Token token;
        token.type = type;
        token.line = line;
        token.column = column;
        return token;
    }

    inline Token EndOfFileToken()
    {
        return NormalToken(Token::EndOfFile);
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
            case '/':
            {
                char next = Next();
                if (next == '/')
                {
                    current = Next();
                    return SingleLineCommentToken();
                }
                else if (next == '*')
                {
                    current = Next();
                    return MultiLineCommentToken();
                }
                else
                {
                    current = next;
                    return NormalToken('/');
                }
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                return NumberToken();
            }
            default:
                return IdentifierToken();
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

    enum class ShouldPush
    {
        ForCRLF,
        Nothing,
    };
    void NewLine(ShouldPush type = ShouldPush::Nothing)
    {
        ++line;
        char next = Next();
        if (((next == '\r' || next == '\n') && next != current) || (next == EOF))
        {
            if (type == ShouldPush::ForCRLF && next != EOF)
            {
                buffer.push_back(next);
            }
            column = 0;
            current = Next();
        }
        else
        {
            column = 1;
            current = next;
        }
    }

    inline Token StringToken(const std::string &str)
    {
        auto token = NormalToken(Token::String);
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
                throw ParseError("incomplete string at <eof>");
            }

            if (current == '\r' || current == '\n')
            {
                throw ParseError("incomplete string at line:") << line << " column:" << column;
            }

            ConvertEscapeCharacter();
        }

        current = Next();
        return StringToken(buffer);
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
                int32_t i = 0;
                for (; i < 2 && isxdigit(current); ++i, current = Next())
                {
                    hex[i] = current;
                }
                if (i == 0)
                {
                    throw ParseError("unexpected character after '\\x' line:") << line << " column:" << column;
                }
                buffer.push_back(static_cast<char>(std::strtoul(hex, 0, 16)));
                return;
            }
            else if (isdigit(current))
            {
                char dec[4] = {0};
                for (int32_t i = 0; i < 3 && isdigit(current); ++i, current = Next())
                {
                    dec[i] = current;
                }
                auto result = std::strtoul(dec, 0, 10);
                if (result > 255)
                {
                    throw ParseError("decimal escape too large near \\") << result << " line:" << line << " column:" << column;
                }
                buffer.push_back(static_cast<char>(result));
                return;
            }
            else
            {
                throw ParseError("unexpected character after '\\' line:") << line << " column:" << column;
            }
        }
        else
        {
            buffer.push_back(current);
        }

        current = Next();
    }

    Token IdentifierToken()
    {
        if (!isidhead(current))
        {
            throw ParseError("unexpected character :") << current << " line:" << line << " column:" << column;
        }
        buffer.clear();
        buffer.push_back(current);
        current = Next();

        while (isidbody(current))
        {
            buffer.push_back(current);
            current = Next();
        }

        if (current == '"' && buffer == "R")
        {
            return RawStringToken();
        }

        Token token = NormalToken(Token::Identifier);
        token.value = buffer;
        return token;
    }

    Token RawStringToken()
    {
        buffer.clear();
        const int32_t max_delimiter_length = 16;
        std::string delimiter = "";
        delimiter.reserve(max_delimiter_length);
        current = Next();
        while (isdelimiter(current) && (delimiter.length() < max_delimiter_length))
        {
            delimiter.push_back(current);
            current = Next();
        }
        if (isdelimiter(current))
        {
            throw ParseError("raw string delimiter longer than ") << max_delimiter_length << " characters : line:" << line << " column:" << column;
        }
        if (current != '(')
        {
            throw ParseError("invalid character in raw string delimiter :") << current << " line:" << line << " column:" << column;
        }
        delimiter.push_back('"');
        current = Next();
        bool finished = false;
        while (!finished)
        {
            if (current == EOF)
            {
                throw ParseError("incomplete raw string at <eof>");
            }
            while (current != ')' && current != EOF)
            {
                buffer.push_back(current);
                current = Next();
            }
            current = Next();
            std::string matchdelimiter = "";
            for (const char dchar : delimiter)
            {
                if (current != dchar)
                {
                    break;
                }
                matchdelimiter.push_back(dchar);
                current = Next();
            }
            if (matchdelimiter.length() == delimiter.length())
            {
                finished = true;
            }
            else
            {

                buffer.push_back(')');
                buffer.append(matchdelimiter);
            }
        }

        return StringToken(buffer);
    }

    inline Token CommentToken(const std::string &str)
    {
        Token token = NormalToken(Token::Comment);
        token.value = str;
        return token;
    }

    Token SingleLineCommentToken()
    {
        buffer.clear();
        while (current != '\r' && current != '\n' && current != EOF)
        {
            buffer.push_back(current);
            current = Next();
        }
        return CommentToken(buffer);
    }

    Token MultiLineCommentToken()
    {
        buffer.clear();
        bool finished = false;
        while (current != EOF)
        {
            if (current == '\r' || current == '\n')
            {
                buffer.push_back(current);
                NewLine(ShouldPush::ForCRLF);
            }
            else if (current == '*')
            {
                char next = Next();
                if (next == '/')
                {
                    finished = true;
                    current = Next();
                    break;
                }
                else
                {
                    buffer.push_back(current);
                    current = next;
                }
            }
            else
            {
                buffer.push_back(current);
                current = Next();
            }
        }

        if (!finished)
        {
            throw ParseError("comment unclosed at <eof>");
        }
        return CommentToken(buffer);
    }

    Token NumberToken()
    {
        buffer.clear();
        if (current == '0')
        {
            char next = Next();
            if (next == 'b' || next == 'B')
            {
                current = Next();
                return BinNumberToken();
            }
            else if (next == 'x' || next == 'X')
            {
                buffer.push_back(current);
                buffer.push_back(next);
                current = Next();
                return HexNumberToken();
            }
            else if (isdigit(next))
            {
                buffer.push_back(current);
                current = next;
                return OctNumberToken();
            }
        }
        return DecNumberToken();
    }

    Token BinNumberToken()
    {
        bool should_push = false;
        bool is_signed = true;
        bool has_digit = false;
        size_t bit = 32;
        while (isxdigit(current))
        {
            if (isbdigit(current))
            {
                has_digit = true;
                should_push = (should_push || current == '1'); // trim 0
                if (should_push)
                {
                    buffer.push_back(current);
                }
                current = Next();
            }
            else
            {
                throw ParseError("unexpected digit '") << current << "' in binary number literal at line:" << line << " column:" << column;
            }
        }

        if (!has_digit)
        {
            throw ParseError("expect digit after binary number literal prefix at line:") << line << " column:" << column;
        }
        else if (buffer.empty())
        {
            buffer.push_back('0');
        }

        bool should_parse_bit = false;
        if (isunsigned(current))
        {
            is_signed = false;
            should_parse_bit = true;
        }
        else if (issigned(current))
        {
            should_parse_bit = true;
        }
        else if (isidhead(current))
        {
            throw ParseError("unexpected character '") << current << "' after binary number literal at line:" << line << " column:" << column;
        }
        else if (current == '.')
        {
            throw ParseError("unexpected '.' in binary number literal at line:") << line << " column:" << column;
        }
        current = Next();
        if (should_parse_bit)
        {
            bit = ParseBit();
            if (buffer.length() > bit)
            {
                throw ParseError("binary number literal is out of range at line:") << line << " column:" << column;
            }
        }
        else
        {
            auto length = buffer.length();
            if (length <= 32)
            {
                bit = 32;
            }
            else if (length <= 64)
            {
                bit = 64;
            }
            else
            {
                throw ParseError("binary number literal is out of range at line:") << line << " column:" << column;
            }
        }

        if (is_signed)
        {
            int64_t number = std::strtoll(buffer.c_str(), nullptr, 2);
            return NumberTokenByBit<true>(number, bit);
        }
        else
        {
            uint64_t number = std::strtoull(buffer.c_str(), nullptr, 2);
            return NumberTokenByBit<false>(number, bit);
        }
    }

    template <bool is_signed, typename T>
    Token NumberTokenByBit(const T &number, const size_t &bit)
    {
        if constexpr (is_signed)
        {
            switch (bit)
            {
            case BIT8:
                return NumberToken(static_cast<int8_t>(number));
            case BIT16:
                return NumberToken(static_cast<int16_t>(number));
            case BIT32:
                return NumberToken(static_cast<int32_t>(number));
            default:
                return NumberToken(static_cast<int64_t>(number));
            }
        }
        else
        {
            switch (bit)
            {
            case BIT8:
                return NumberToken(static_cast<uint8_t>(number));
            case BIT16:
                return NumberToken(static_cast<uint16_t>(number));
            case BIT32:
                return NumberToken(static_cast<uint32_t>(number));
            default:
                return NumberToken(static_cast<uint64_t>(number));
            }
        }
    }

    Token OctNumberToken()
    {
        bool is_signed = true;
        size_t bit = 32;
        while (isxdigit(current))
        {
            if (isodigit(current))
            {
                buffer.push_back(current);
                current = Next();
            }
            else
            {
                throw ParseError("unexpected digit '") << current << "' in octal number literal at line:" << line << " column:" << column;
            }
        }
        bool should_parse_bit = false;
        if (isunsigned(current))
        {
            is_signed = false;
            should_parse_bit = true;
        }
        else if (issigned(current))
        {
            should_parse_bit = true;
        }
        else if (isidhead(current))
        {
            throw ParseError("unexpected character '") << current << "' after octal number literal at line:" << line << " column:" << column;
        }
        else if (current == '.')
        {
            throw ParseError("unexpected '.' in octal number literal at line:") << line << " column:" << column;
        }
        current = Next();
        if (should_parse_bit)
        {
            bit = ParseBit();
        }
        bool RangeError = false;
        errno = 0;
        if (is_signed)
        {
            int64_t number = std::strtoll(buffer.c_str(), nullptr, 8);
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else if (should_parse_bit)
            {
                RangeError = (GetDataBit<int64_t, int32_t, INT8_MAX, INT16_MAX, INT32_MAX>(number) > bit);
            }
            else
            {
                bit = GetDataBit<int64_t, int32_t, INT8_MAX, INT16_MAX, INT32_MAX>(number);
                bit = std::max(bit, BIT32);
            }
            if (!RangeError)
            {
                return NumberTokenByBit<true>(number, bit);
            }
        }
        else
        {
            uint64_t number = std::strtoull(buffer.c_str(), nullptr, 8);
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                RangeError = (GetDataBit<uint64_t, uint32_t, UINT8_MAX, UINT16_MAX, UINT32_MAX>(number) > bit);
            }
            if (!RangeError)
            {
                return NumberTokenByBit<false>(number, bit);
            }
        }
        errno = 0;
        throw ParseError("octal number literal is out of range at line:") << line << " column:" << column;
    }

    Token DecNumberToken()
    {
        return NumberToken<int32_t>(10);
    }

    Token HexNumberToken()
    {
        return NumberToken<int32_t>(16);
    }

    template <typename Number64T, typename Number32T, Number32T Number8Max, Number32T Number16Max, Number32T Number32Max>
    size_t GetDataBit(const Number64T &number)
    {
        if (number <= static_cast<Number64T>(Number8Max))
        {
            return BIT8;
        }
        else if (number <= static_cast<Number64T>(Number16Max))
        {
            return BIT16;
        }
        else if (number <= static_cast<Number64T>(Number32Max))
        {
            return BIT32;
        }
        else
        {
            return BIT64;
        }
    }

    size_t ParseBit()
    {
        char next = Next();
        bool nextnext = true;
        size_t bit = 32;

        if (current == '8')
        {
            bit = BIT8;
            nextnext = false;
        }
        else if (current == '1' && next == '6')
        {
            bit = BIT16;
        }
        else if (current == '3' && next == '2')
        {
            bit = BIT32;
        }
        else if (current == '6' && next == '4')
        {
            bit = BIT64;
        }
        else
        {
            throw ParseError("unexpected postfix bit after number literal at line:") << line << " column:" << column;
        }
        if (nextnext)
        {
            next = Next();
        }
        if (isidbody(next))
        {
            throw ParseError("unexpected postfix character '") << next << "' after number literal at line:" << line << " column:" << column;
        }
        else
        {
            current = next;
        }
        return bit;
    }

    template <typename T>
    Token NumberToken(T t)
    {
        Token token = NormalToken(Token::Number);
        Token::NumberValue value;
        value.number = t;
        value.type = Token::NumberType<T>::value;
        token.value = value;
        return token;
    }
};

std::unique_ptr<Lexer> Lexer::GetLexer(std::istream &code)
{
    return std::make_unique<LexerImpl>(code);
}
} // namespace cdscript

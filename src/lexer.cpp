// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "lexer.hpp"
#include <algorithm>
#include <cctype>
#include <cerrno>
#include <iostream>
#include <unordered_map>
#include "utils.hpp"

namespace cd::script
{
static const std::string disallowed = R"#($()\`@)#";
static const size_t BIT8 = 8;
static const size_t BIT16 = 16;
static const size_t BIT32 = 32;
static const size_t BIT64 = 64;
enum class ERadix : int
{
    Dec = 10,
    Hex = 16
};

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
inline bool isdigit(char ch, ERadix radix) { return (radix == ERadix::Dec) ? isdigit(ch) : isxdigit(ch); }
inline bool isexponent(char ch, ERadix radix) { return (radix == ERadix::Dec) ? isexponent(ch) : isxexponent(ch); }

static std::unordered_map<std::string, token_t> KeyWords = {
    {"null", Token::Null},
    {"true", Token::True},
    {"false", Token::False},
    {"if", Token::If},
    {"else", Token::Else},
    {"for", Token::For},
    {"while", Token::While},
    {"in", Token::In},
    {"break", Token::Break},
    {"continue", Token::Continue},
    {"return", Token::Return},
    {"fun", Token::Function},
    {"try", Token::Try},
    {"catch", Token::Catch},
    {"throw", Token::Throw},
    {"class", Token::Class},
    {"interface", Token::Interface},
    {"is", Token::Is},
    {"object", Token::Object},
    {"this", Token::This},
    {"super", Token::Super},
    {"any", Token::Any},
};

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
                auto next = Next();
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
                else if (next == '=')
                {
                    current = Next();
                    return NormalToken(Token::DivideEqual);
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
            case '<':
            {
                return XOrXXOrXEqualToken('<', Token::LeftShift, Token::LessEqual);
            }
            case '>':
            {
                return XOrXXOrXEqualToken('>', Token::RightShift, Token::GreatEqual);
            }
            case '+':
            {
                return XOrXXOrXEqualToken('+', Token::PlusPlus, Token::PlusEqual);
            }
            case '-':
            {
                return XOrXXOrXEqualToken('-', Token::MinusMinus, Token::MinusEqual);
            }
            case '*':
            {
                return XOrXEqualToken('*', Token::MultiplyEqual);
            }
            case '%':
            {
                return XOrXEqualToken('%', Token::ModuloEqual);
            }
            case '&':
            {
                return XOrXXOrXEqualToken('&', Token::And, Token::AndEqual);
            }
            case '|':
            {
                return XOrXXOrXEqualToken('|', Token::Or, Token::OrEqual);
            }
            case '^':
            {
                return XOrXEqualToken('^', Token::ExclusiveOrEqual);
            }
            case '!':
            {
                return XOrXEqualToken('!', Token::NotEqual);
            }
            case '=':
            {
                return XOrXEqualToken('=', Token::Equal);
            }
            case '?':
            case ':':
            case ';':
            case ',':
            case '@':
            case '#':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            {
                auto token = current;
                current = Next();
                return NormalToken(token);
            }
            case '.':
            {
                auto next = Next();
                if (next == '.')
                {
                    next = Next();
                    if (next == '.')
                    {
                        current = Next();
                        return NormalToken(Token::VarArg);
                    }
                    else
                    {
                        current = next;
                        return NormalToken(Token::Concat);
                    }
                }
                else
                {
                    current = next;
                    return NormalToken('.');
                }
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
        auto next = Next();
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
                throw Exception("incomplete string at <eof>");
            }

            if (current == '\r' || current == '\n')
            {
                throw Exception("incomplete string at line:", line, " column:", column);
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
                    throw Exception("unexpected character after '\\x' line:", line, " column:", column);
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
                    throw Exception("decimal escape too large near \\", result, " line:", line, " column:", column);
                }
                buffer.push_back(static_cast<char>(result));
                return;
            }
            else
            {
                throw Exception("unexpected character after '\\' line:", line, " column:", column);
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
            throw Exception("unexpected character :'", current, "' line:", line, " column:", column);
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

        auto itrKW = KeyWords.find(buffer);
        if (itrKW != KeyWords.end())
        {
            return NormalToken(itrKW->second);
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
            throw Exception("raw string delimiter longer than ", max_delimiter_length, " characters : line:", line, " column:", column);
        }
        if (current != '(')
        {
            throw Exception("invalid character in raw string delimiter :", current, " line:", line, " column:", column);
        }
        delimiter.push_back('"');
        current = Next();
        bool finished = false;
        while (!finished)
        {
            if (current == EOF)
            {
                throw Exception("incomplete raw string at <eof>");
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
                auto next = Next();
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
            throw Exception("comment unclosed at <eof>");
        }
        return CommentToken(buffer);
    }

    Token NumberToken()
    {
        buffer.clear();
        if (current == '0')
        {
            auto next = Next();
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
                return DecHexNumberToken(ERadix::Hex);
            }
            else if (isdigit(next))
            {
                buffer.push_back(current);
                current = next;
                return OctNumberToken();
            }
            else
            {
                buffer.push_back(current);
                current = next;
            }
        }
        return DecHexNumberToken(ERadix::Dec);
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
                should_push = (should_push || current == '1');  // trim 0
                if (should_push)
                {
                    buffer.push_back(current);
                }
                current = Next();
            }
            else
            {
                throw Exception("unexpected digit '", current, "' in binary number literal at line:", line, " column:", column);
            }
        }

        if (!has_digit)
        {
            throw Exception("expect digit after binary number literal prefix at line:", line, " column:", column);
        }
        else if (buffer.empty())
        {
            buffer.push_back('0');
        }

        bool should_match_bit = false;
        if (isunsigned(current))
        {
            is_signed = false;
            should_match_bit = true;
        }
        else if (issigned(current))
        {
            should_match_bit = true;
        }
        else if (isidhead(current))
        {
            throw Exception("unexpected character '", current, "' after binary number literal at line:", line, " column:", column);
        }
        else if (current == '.')
        {
            throw Exception("unexpected '.' in binary number literal at line:", line, " column:", column);
        }
        current = Next();
        if (should_match_bit)
        {
            bit = ParseBit();
            if (buffer.length() > bit)
            {
                throw Exception("binary number literal is out of range at line:", line, " column:", column);
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
                throw Exception("binary number literal is out of range at line:", line, " column:", column);
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
                throw Exception("unexpected digit '", current, "' in octal number literal at line:", line, " column:", column);
            }
        }
        bool should_match_bit = false;
        if (isunsigned(current))
        {
            is_signed = false;
            should_match_bit = true;
        }
        else if (issigned(current))
        {
            should_match_bit = true;
        }
        else if (isidhead(current))
        {
            throw Exception("unexpected character '", current, "' after octal number literal at line:", line, " column:", column);
        }
        else if (current == '.')
        {
            throw Exception("unexpected '.' in octal number literal at line:", line, " column:", column);
        }
        current = Next();
        if (should_match_bit)
        {
            bit = ParseBit();
        }
        bool RangeError = false;
        errno = 0;

        if (!should_match_bit)
        {
            uint64_t number = std::strtoull(buffer.c_str(), nullptr, 8);
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                return AutoIntTypeNumber(number);
            }
        }
        else if (is_signed)
        {
            int64_t number = std::strtoll(buffer.c_str(), nullptr, 8);
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                RangeError = (GetDataBit<int8_t, int16_t, int32_t, int64_t>(number) > bit);
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
                RangeError = (GetDataBit<uint8_t, uint16_t, uint32_t, uint64_t>(number) > bit);
            }
            if (!RangeError)
            {
                return NumberTokenByBit<false>(number, bit);
            }
        }
        errno = 0;
        throw Exception("octal number literal is out of range at line:", line, " column:", column);
    }

    Token DecHexNumberToken(ERadix radix)
    {
        bool has_point = false;
        bool has_exponent = false;
        bool is_float = false;
        bool is_signed = true;
        bool should_match_bit = false;
        size_t bit = 32;
        while (isdigit(current, radix) || current == '.')
        {
            if (current == '.')
            {
                if (has_point)
                {
                    throw Exception("multiple '.' in number literal at line:", line, " column:", column);
                }
                else
                {
                    has_point = true;
                    buffer.push_back(current);
                    current = Next();
                }
            }
            else
            {
                buffer.push_back(current);
                current = Next();
            }
        }

        if (isexponent(current, radix))
        {
            buffer.push_back(current);
            current = Next();
            if (current == '-' || current == '+')
            {
                buffer.push_back(current);
                current = Next();
            }
            while (isdigit(current, radix))
            {
                buffer.push_back(current);
                current = Next();
                has_exponent = true;
            }
            if (!has_exponent)
            {
                throw Exception("expect exponent digit at line:", line, " column:", column);
            }
        }
        if (isfloat(current))
        {
            if (has_point || has_exponent)
            {
                is_float = true;
                current = Next();
            }
            else
            {
                throw Exception("unexpected '", current, "' after integer literal at line:", line, " column:", column);
            }
        }
        else if (isunsigned(current))
        {
            if (has_point || has_exponent)
            {
                throw Exception("unexpected '", current, "' after float literal at line:", line, " column:", column);
            }
            is_signed = false;
            should_match_bit = true;
        }
        else if (issigned(current))
        {
            if (has_point || has_exponent)
            {
                throw Exception("unexpected '", current, "' after float literal at line:", line, " column:", column);
            }
            should_match_bit = true;
        }
        else if (isidhead(current))
        {
            throw Exception("unexpected '", current, "' after number literal at line:", line, " column:", column);
        }
        if (should_match_bit)
        {
            current = Next();
            bit = ParseBit();
        }
        bool RangeError = false;
        errno = 0;
        if (is_float)
        {
            float number = std::strtof(buffer.c_str(), nullptr);
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                return NumberToken<float>(number);
            }
        }
        else if (has_point || has_exponent)
        {
            double number = std::strtod(buffer.c_str(), nullptr);
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                return NumberToken<double>(number);
            }
        }
        else if (!should_match_bit)
        {
            uint64_t number = std::strtoull(buffer.c_str(), nullptr, static_cast<int>(radix));
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                return AutoIntTypeNumber(number);
            }
        }
        else if (is_signed)
        {
            int64_t number = std::strtoll(buffer.c_str(), nullptr, static_cast<int>(radix));
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                RangeError = (GetDataBit<int8_t, int16_t, int32_t, int64_t>(number) > bit);
            }
            if (!RangeError)
            {
                return NumberTokenByBit<true>(number, bit);
            }
        }
        else
        {
            uint64_t number = std::strtoull(buffer.c_str(), nullptr, static_cast<int>(radix));
            if (errno == ERANGE)
            {
                RangeError = true;
            }
            else
            {
                RangeError = (GetDataBit<uint8_t, uint16_t, uint32_t, uint64_t>(number) > bit);
            }
            if (!RangeError)
            {
                return NumberTokenByBit<false>(number, bit);
            }
        }
        errno = 0;
        throw Exception("number literal is out of range at line:", line, " column:", column);
    }

    template <typename Number8T, typename Number16T, typename Number32T, typename Number64T>
    size_t GetDataBit(const Number64T &number)
    {
        if (number <= static_cast<Number64T>(std::numeric_limits<Number8T>::max()))
        {
            return BIT8;
        }
        else if (number <= static_cast<Number64T>(std::numeric_limits<Number16T>::max()))
        {
            return BIT16;
        }
        else if (number <= static_cast<Number64T>(std::numeric_limits<Number32T>::max()))
        {
            return BIT32;
        }
        else
        {
            return BIT64;
        }
    }

    Token AutoIntTypeNumber(const uint64_t &number)
    {
        if (number <= static_cast<uint64_t>(std::numeric_limits<int32_t>::max()))
        {
            return NumberToken(static_cast<int32_t>(number));
        }
        else if (number <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
        {
            return NumberToken(static_cast<int64_t>(number));
        }
        else
        {
            return NumberToken(number);
        }
    }

    size_t ParseBit()
    {
        auto next = Next();
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
            throw Exception("unexpected postfix bit after number literal at line:", line, " column:", column);
        }
        if (nextnext)
        {
            next = Next();
        }
        if (isidbody(next))
        {
            throw Exception("unexpected postfix character '", next, "' after number literal at line:", line, " column:", column);
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
        value.set(t);
        token.value = value;
        return token;
    }

    Token XOrXXOrXEqualToken(token_t XType, token_t XXType, token_t XEqualType)
    {
        auto next = Next();
        if (next == current)
        {
            current = Next();
            return NormalToken(XXType);
        }
        else if (next == '=')
        {
            current = Next();
            return NormalToken(XEqualType);
        }
        else
        {
            current = next;
            return NormalToken(XType);
        }
    }

    Token XOrXEqualToken(token_t XType, token_t XEqualType)
    {
        auto next = Next();
        if (next == '=')
        {
            current = Next();
            return NormalToken(XEqualType);
        }
        else
        {
            current = next;
            return NormalToken(XType);
        }
    }
};

std::unique_ptr<Lexer> Lexer::GetLexer(std::istream &code)
{
    return std::make_unique<LexerImpl>(code);
}
}  // namespace cd::script

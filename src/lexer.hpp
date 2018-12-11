// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include "token.hpp"
#include <sstream>

namespace cdscript
{

namespace exception
{
class LexerError : public std::exception
{
  public:
    LexerError(std::string &&info) noexcept
    {
        ss += info;
    }

    template <typename T>
    LexerError &operator<<(T t) noexcept
    {
        std::ostringstream os;
        os << t;
        ss += os.str();
        return *this;
    }

    virtual const char *what() const noexcept override
    {
        return ss.c_str();
    }
    std::string ss;
};

} // namespace exception

class Lexer
{
  public:
    virtual ~Lexer(){};
    [[nodiscard]] virtual Token GetToken() = 0;
    [[nodiscard]] static std::unique_ptr<Lexer> GetLexer(std::istream &code);
};

} // namespace cdscript

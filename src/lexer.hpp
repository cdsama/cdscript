// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include "token.hpp"
#include <memory>
#include <sstream>

namespace cd::script
{
class Lexer
{
  public:
    class ParseError : public std::exception
    {
      public:
        ParseError(std::string &&info) noexcept
        {
            ss += info;
        }

        template <typename T>
        ParseError &operator<<(T t) noexcept
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
    virtual ~Lexer(){};
    [[nodiscard]] virtual Token GetToken() = 0;
    [[nodiscard]] static std::unique_ptr<Lexer> GetLexer(std::istream &code);
};

} // namespace cd::script

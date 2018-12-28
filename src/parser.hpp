// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <sstream>
#include "syntax.hpp"

namespace cdscript
{
class Parser
{
  public:
    class SyntaxError : public std::exception
    {
      public:
        SyntaxError(std::string &&info) noexcept
        {
            ss += info;
        }

        template <typename T>
        SyntaxError &operator<<(T t) noexcept
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
    virtual ~Parser(){};
    [[nodiscard]] virtual std::unique_ptr<class Syntax> GetAbstractSyntaxTree() = 0;
    [[nodiscard]] static std::unique_ptr<Parser> GetParser(std::unique_ptr<class Lexer> &lexer);
};
} // namespace cdscript

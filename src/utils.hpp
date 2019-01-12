// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <sstream>
namespace cd
{
class Exception : public std::exception
{
  public:
    template <typename... Args>
    Exception(Args &&... args) noexcept
    {
        std::ostringstream os;
        (os << ... << std::forward<Args>(args));
        ss = os.str();
    }

    virtual const char *what() const noexcept override
    {
        return ss.c_str();
    }
    std::string ss;
};
} // namespace cd

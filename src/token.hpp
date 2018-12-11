// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <variant>
#include "cdscript.hpp"
namespace cdscript
{
using token_t = int32_t;
struct Token
{
    enum type
    {
        Integer = 256, // <integer>
        Number,        // <number>
        String,        // <string>
        EndOfFile,     // <EOF>
    };
    token_t token = 0;
    size_t line = 0;
    size_t column = 0;
    std::variant<integer_t, number_t, std::string> value;
};

} // namespace cdscript